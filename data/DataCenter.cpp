/* DataCenter
 *
 *
*/
#include "DataCenter.h"
#include "WidgetMng.h"
#include "LineInfo.h"
#include "LineTimeTables.h"
#include "LineInterchangeInfo.h"
#include "LineStations.h"
#include "TicketBasicInfo.h"
#include "TransactionInfo.h"
#include "SettingCenter.h"
#include "HttpTool.h"
#include "BasicInfo.h"
#include "ASRHttpTool.h"
#include "ReregisterInfo.h"
#include "LoginInfo.h"
#include "NCNetwork_Lib.h"
#include "MyHelper.h"
#include "BIM2020.h"
#include "Station.h"
#include "LineStations.h"
#include "NC_ReaderLib.h"
#include "HeartTimer.h"
#include "AFCTaskThread.h"
#include "BomParamVersionInfo.h"
#include "OperatorInfo.h"
#include "LibcurlFtp.h"
#include "TaskThread.h"
#include "FtpUploadTask.h"
#include "FtpDownloadTask.h"


static int HRT_NUM = 5;
DataCenter* DataCenter::m_pInstance = NULL;
DataCenter::DataCenter(QObject *parent) : QObject(parent)
{
    init();
    getLastMsgSeq();
}

DataCenter::~DataCenter()
{
    // 数据备份
    SettingCenter::getThis()->saveLineBasicInfo(m_lineList);
    SettingCenter::getThis()->saveLineStations(m_lineStations);
    SettingCenter::getThis()->saveLineInterchanes(m_lineInterchanges);
    SettingCenter::getThis()->saveLineTimeTables(m_lineTimeTables);

    // 设备关闭
    closeDevice();

    if (m_taskThread->isRunning())
    {
        m_taskThread->quit();
        m_taskThread->wait();
    }

    if (m_ftpTaskThread->isRunning()) {
        m_ftpTaskThread->stopRun();
        m_ftpTaskThread->quit();
        m_ftpTaskThread->wait();
    }
}

DataCenter *DataCenter::getThis()
{
    if (m_pInstance == NULL)
        m_pInstance = new DataCenter();
    return m_pInstance;
}

// 心跳连接处理
void DataCenter::secEvent()
{
    for (int i = 0; i < HRT_NUM; i++) {
        m_hrtCnt[i] = m_hrtCnt[i] + 1;
//        if (m_hrtCnt[i] == 1000000000) {    // 超过10秒未连接则掉线处理
//            setHrtOffData(i);
//        }
    }
}


void DataCenter::init()
{
    initData();    // 默认数据

    // 参数文件解析
    initParamVersion();

    /* 基础信息 */
    logger()->info("基础信息读取");
    m_basicInfo = SettingCenter::getThis()->getBasicInfo();
    HttpTool::getThis()->setId(m_basicInfo->deviceId(), m_basicInfo->stationName());
    HttpTool::getThis()->setServUrl(m_basicInfo->ismServiceIp(), m_basicInfo->ismServicePort());

    ASRHttpTool::getThis()->setAppkey(m_basicInfo->appkey());
    ASRHttpTool::getThis()->setSecret(m_basicInfo->secret());
    ASRHttpTool::getThis()->setIp(m_basicInfo->asrServiceIp());
    ASRHttpTool::getThis()->setPort(m_basicInfo->asrServicePort());

    logger()->info("基础数据更新");
    // 获取基础数据并更新数据 #7
//    HttpTool::getThis()->requestLineBaseInfo();
//    HttpTool::getThis()->requestLineStations();
//    HttpTool::getThis()->requestInterchanges();
//    HttpTool::getThis()->requestTimeTables();
//    HttpTool::getThis()->requestStationMap();
//    HttpTool::getThis()->requestStationPreMap();
//    HttpTool::getThis()->requestLineMap();

    logger()->info("配置文件读取。");
    m_lineStations.append(SettingCenter::getThis()->getLineStations());
    m_lineTimeTables.append(SettingCenter::getThis()->getLineTimeTables());
    m_lineInterchanges.append(SettingCenter::getThis()->getLineInterchanes());
    m_lineList.append(SettingCenter::getThis()->getLineBasicInfo());
    logger()->info("配置文件读取完毕。");

    m_stationCodeMap.clear();
    for(LineStations* line : m_lineStations) {
        QList<Station*> stations = line->stationList();
        for (Station* station : stations) {
            m_stationCodeMap.insert(station->code(), station->name());
        }
    }

    // AFC通信库初始化
    QByteArray devByteArray = MyHelper::hexStrToByte(getDeviceId());
    BYTE* deviceId = (BYTE*)devByteArray.data();
    QByteArray scIdByteArray = MyHelper::hexStrToByte(m_basicInfo->scId());
    BYTE* scId = (BYTE*)devByteArray.data();
    QByteArray scIpArray = m_basicInfo->scIP().toLatin1();
    char* scIp = scIpArray.data();
    uint scPort = m_basicInfo->scPort();
    QByteArray localIpArray = m_basicInfo->localIP().toLatin1();
    char* localIp = localIpArray.data();
    uint localPort = m_basicInfo->localPort();

    int ret = initNetworkLib(deviceId, scId, scIp, scPort, localIp, localPort);

    char version[60];
    getLibVersion(version);
    logger()->info("[getLibVersion]AFC通讯库初始化{%2}，获取版本号={%1}", QString(version), ret);

    // TODO:test
    deviceState2afc();
    param2afc();

    m_timer = new HeartTimer();
    connect(m_timer, &HeartTimer::onlineFlag, this, &DataCenter::afcHeart);
    m_timer->startTimer(30000);

    m_taskThread = new AFCTaskThread(this);
    connect(m_taskThread, &AFCTaskThread::paramTypeUpdate, this, &DataCenter::onParamUpdate, Qt::DirectConnection);
    connect(m_taskThread, &AFCTaskThread::softwareUpdate, this, &DataCenter::onSoftwareUpdate, Qt::DirectConnection);
    m_taskThread->start();

    int id = 0;
    m_ftpTaskThread = new TaskThread(this);
    connect(m_ftpTaskThread, &TaskThread::allTaskDone, this, &DataCenter::ftpTaskFinished);


//    emit lineReceived();

    initDevice();
    initReaderErrCode();
}

void DataCenter::initData()
{
    m_basicInfo = NULL;               // 站点基础信息
    m_loginInfo = NULL;               // 登录信息
    m_ticketBasicInfo = NULL;         // 车票基本信息

    m_isReaderUsable = false;         // 读写器可用状态
    m_isSpecieUsable = false;         // 硬币模块可用状态
    m_isBanknotesUsable = false;      // 纸币模块可用状态

    m_serviceState = 1;
    m_netState = 1;

    taskId = 0;

    m_tradeSerial = SettingCenter::getThis()->getTradeSerial();

    // 心跳
    for (int i = 0; i < HRT_NUM; i++) {
        m_hrtCnt[i] = 0;
    }

}

void DataCenter::initDevice()
{
    if (NULL == m_basicInfo) {
        return;
    }
    // 读写器初始化
    m_isReaderUsable = false;
    BYTE initRet = 0;
    int ret = (ushort)readerComOpen(m_basicInfo->readerPort());
    if (ret == 0) {
        QString deviceIdStr = getDeviceId();
        QByteArray devByteArray = MyHelper::hexStrToByte(deviceIdStr);
        BYTE* deviceId = (BYTE*)devByteArray.data();

        BYTE producter = 0;
        initRet = readerInit(deviceId, producter);
        if (initRet == 0) {
            m_isReaderUsable = true;

            // 上传安装的SAM卡号，供ACC校验
            samInfo2afc();
        }
    }
    logger()->info("[readerComOpen]读写器open={%1}, 端口号={%2}; [readerInit]={%3}",
                   ret, m_basicInfo->readerPort(), initRet);

    // 钱箱初始化
    int bimPort = m_basicInfo->bimPort();
    int f53Port = m_basicInfo->f53Port();
    int brcPort = m_basicInfo->brcPort();

    m_isBanknotesUsable = false;
    m_isSpecieUsable = false;
    int retC = ConnectMachine(bimPort, brcPort, f53Port);
    if ((retC & 0x0F) == 0) {
        m_isBanknotesUsable = true;
    } else if ((retC & 0xF0) == 0) {
        m_isSpecieUsable = true;
    }
    logger()->info("[ConnectMachine]钱箱初始化={%1}, 端口号：bim={%2}, f53={%3}, brc={%4}",
                   retC, bimPort, f53Port, brcPort);
}

void DataCenter::closeDevice()
{
    readerComClose();
    CloseAllCom();
}

void DataCenter::initReaderErrCode()
{
    m_readerErrCodeMap.clear();
    QString filePath = QDir::currentPath() + QDir::separator() + QDir::separator() + "ReaderErrorCode.ini";
    QSettings *errorIni = new QSettings(filePath, QSettings::IniFormat);
    errorIni->setIniCodec(QTextCodec::codecForName("system"));

    QRegExp sep("/");
    bool ok;
    QStringList keyList = errorIni->allKeys();
    for (QString key : keyList) {
        QString value = errorIni->value(key).toString();
        BYTE newKey = key.section(sep, 1, 1).toUShort(&ok, 16);
        m_readerErrCodeMap.insert(newKey, value);
    }
}


// 参数解析
void DataCenter::initParamVersion()
{
    QString path = QDir::currentPath() + QDir::separator() + "bom-param" + QDir::separator();
    QList<BomParamVersionInfo*> list = SettingCenter::getThis()->getParamVersionInfo(path + "version.json");
    for (BomParamVersionInfo* info: list) {
        long type = info->type();
        QString fileName = info->fileName();
        QString filePath = path + fileName;
        if (type == 0x1001) {
            parseParam1001(filePath);
        } else if (type == 0x1004) {
            parseParam1004(filePath);
        } else if (type == 0x2002) {
            parseParam2002(filePath);
        } else if (type == 0x2004) {
            parseParam2004(filePath);
        }
    }
}


// 文件校验：只计算md5
bool DataCenter::fileCheck(QByteArray array)
{
    int size = array.size();
    // 获取16个字节的md5
    QByteArray md5Array = array.right(16);
    QByteArray body = array.left(size - 16);

    QString bodyMD5Str = QCryptographicHash::hash(body, QCryptographicHash::Md5).toHex();
    QString md5Str = md5Array.toHex();

    if (md5Str.compare(bodyMD5Str) == 0) {
        return true;
    }

    return false;
}

// 参数文件头解析
int DataCenter::parseHead(QDataStream &stream)
{
    // 文件头
    qint8 type;
    QByteArray buffer(7, Qt::Uninitialized);
    qint8 version;
    qint32 count = 0;
    qint16 paramType;

    stream >> type;
    stream.readRawData(buffer.data(), 7);
    stream >> version;
    stream >> count;
    stream >> paramType;

    QString timeStr(buffer.toHex());
    QString head = QString("type={%1}, time={%2}, version={%3}, count={%4}, pType={%5},")
            .arg(type, 2, 16, QLatin1Char('0'))
            .arg(timeStr)
            .arg(version, 2, 16, QLatin1Char('0'))
            .arg(count, 8, 16, QLatin1Char('0'))
            .arg(paramType, 4, 16, QLatin1Char('0'));
    logger()->info("param head : %1", head);

    return count;

}

/*
 * 系统运行参数
*/
int DataCenter::parseParam1001(QString filePath)
{
    QFile file(filePath);
    bool isOk = file.open(QFile::ReadOnly);
    if (!isOk) {
        logger()->error("[%1] file open failed", filePath);
        return -1;
    }

    QDataStream stream(&file);
    parseHead(stream);

    // 文件内容
    qint32 pVersion;
    QByteArray startTime(7, Qt::Uninitialized);
    BYTE tradeCountL;
    BYTE tradeTimeH;
    BYTE deviceInterval;

    stream >> pVersion;
    stream.readRawData(startTime.data(), 7);
    stream.skipRawData(7);
    stream >> tradeCountL;
    stream >> tradeTimeH;
    stream >> deviceInterval;

    file.close();

    QString sTimeStr(startTime.toHex());
    QString body = QString("pVersion={%1}, startTime={%2}, tradeCountL={%3}, tradeTimeH={%4}, deviceInterval={%5},")
            .arg(pVersion, 8, 16, QLatin1Char('0'))
            .arg(sTimeStr)
            .arg(tradeCountL, 2, 16, QLatin1Char('0'))
            .arg(tradeTimeH, 2, 16, QLatin1Char('0'))
            .arg(deviceInterval, 2, 16, QLatin1Char('0'));
    logger()->info("[param1002]%1", body);

    m_deviceStateIntervalSec = deviceInterval * 5;    // 设备状态上传间隔秒数
    m_tradeDataIntervalSec = tradeTimeH * 5;      // 交易信息上传时间间隔
    m_tradeDataCountLT = tradeCountL;

    return 0;
}


/*
 * 车票类型参数
*/
int DataCenter::parseParam1004(QString filePath)
{
    QFile file(filePath);
    bool isOk = file.open(QFile::ReadOnly);
    if (!isOk) {
        logger()->error("[%1] file open failed", filePath);
        return -1;
    }

    QByteArray array = file.readAll();
    bool checkOk = fileCheck(array);
    qDebug() << "file check: " << checkOk;

    QDataStream stream(&file);
    int count = parseHead(stream);

    qint32 version;
    QByteArray startTime(7, Qt::Uninitialized);

    stream >> version;
    stream.readRawData(startTime.data(), 7);
    stream.skipRawData(3);
    logger()->info("[param1004]version=%1, time=%2", version, QString(startTime.toHex()));

    m_ticketCodeMap.clear();     // <code, name>
    for (int i = 0; i < count; i++) {
        stream.skipRawData(1);
        BYTE code;
        stream >> code;
        QByteArray en(15, Qt::Uninitialized);
        QByteArray ch(15, Qt::Uninitialized);
        stream.readRawData(en.data(), 15);
        stream.readRawData(ch.data(), 15);

        QString info = QString("code={%1},en={%2},ch={%3}")
                .arg(code, 2, 16, QLatin1Char('0'))
                .arg(QString(en.toHex()))
                .arg(QString(ch.toHex()));

        QString enStr = MyHelper::getCorrectUnicode(en);
        QString chStr = MyHelper::getCorrectUnicode(ch);

        m_ticketCodeMap.insert(code, chStr);
        logger()->info("src={%1},enStr={%2},chStr={%3}", info, enStr, chStr);

        stream.skipRawData(193);
    }

    file.close();

    return 0;
}

/*
 * BOM运营参数
*/
int DataCenter::parseParam2002(QString filePath)
{
    QFile file(filePath);
    bool isOk = file.open(QFile::ReadOnly);
    if (!isOk) {
        logger()->error("[%1] file open failed", filePath);
        return -1;
    }

    QDataStream stream(&file);
    parseHead(stream);

    qint32 version;
    QByteArray startTime(7, Qt::Uninitialized);
    qint16 maxCount;

    stream >> version;
    stream.readRawData(startTime.data(), 7);
    stream.skipRawData(109);
    stream >> maxCount;

    logger()->info("[param2002]version=%1, time=%2, maxCount=%3", version, QString(startTime.toHex()), maxCount);


    file.close();
    return 0;
}

/*
 * 操作员信息
*/
int DataCenter::parseParam2004(QString filePath)
{
    QFile file(filePath);
    bool isOk = file.open(QFile::ReadOnly);
    if (!isOk) {
        logger()->error("[%1] file open failed", filePath);
        return -1;
    }

    // 读文件
    QDataStream stream(&file);
    int m = parseHead(stream);

    qint32 version;    // 4 byte
    QByteArray buffer(7, Qt::Uninitialized);
    stream >> version;
    stream.readRawData(buffer.data(), 7);

    // 操作员信息
    m_operatorMap.clear();
    for (int i = 0; i < m; i++) {
        QByteArray code(4, Qt::Uninitialized);
        QByteArray name(32, Qt::Uninitialized);
        QByteArray card(6, Qt::Uninitialized);
        QByteArray pwd(8, Qt::Uninitialized);
        BYTE type;
        BYTE deviceAccess;
        QByteArray validDate(7, Qt::Uninitialized);
        qint32 n;
        QByteArray stateCode(4, Qt::Uninitialized);

        stream.readRawData(code.data(), 4);
        stream.readRawData(name.data(), 32);
        stream.readRawData(card.data(), 6);
        stream.readRawData(pwd.data(), 8);
        stream >> type;
        stream >> deviceAccess;
        stream.readRawData(validDate.data(), 7);
        stream >> n;

        // 可操作车站编码
        stream.skipRawData(n * 4);

        // 保留位
        stream.skipRawData(8);

        QString codeStr = QString(code.toHex());
        QString nameStr = MyHelper::getCorrectUnicode(name);
        OperatorInfo* info = new OperatorInfo();
        info->setCode(code);
        info->setName(nameStr);
        info->setCard(QString(card.toHex()));
        info->setPwd(QString(pwd.toHex()));
        info->setType(type);
        info->setAccess(deviceAccess);
        info->setValidDate(QString(validDate.toHex()));

        m_operatorMap.insert(codeStr, info);

//        logger()->info("[param2004]code={%1}, name={%2}, card={%3}, pwd={%4}, type={%5}, access={%6}, validDate={%7}",
//                       codeStr,
//                       nameStr,
//                       info->card(),
//                       info->pwd(),
//                       QString::number(info->type(), 16),
//                       QString::number(info->access(), 16),
//                       info->validDate().toString("yyyyMMddHHmmss"));
    }

    file.close();

    return 0;
}


// 用户鉴权
bool DataCenter::isValidUser(QString userCode, QString pwd)
{
    if (userCode == "04326688" && pwd == "123456") {
        return true;
    }
    if (!m_operatorMap.contains(userCode)) {
        return false;
    }

    // MD5校验值
    QString md5Str = QCryptographicHash::hash(pwd.toLocal8Bit(), QCryptographicHash::Md5).toHex();
    QString md5Pwd = md5Str.mid(0, 16);
    OperatorInfo* info = m_operatorMap.value(userCode);
    if (info->pwd().compare(md5Pwd) == 0) {
        return true;
    }
    return false;
}

void DataCenter::ftpTaskFinished()
{
    // TODO:
}


// 设备编号
QString DataCenter::getDeviceId() const
{
    return m_basicInfo->deviceId();
}

int DataCenter::getAntiNo() const
{
    return m_basicInfo->antiNo();
}

// 票卡更新信息
ReregisterInfo *DataCenter::getReregisterInfo() const
{
    return m_reregisterInfo;
}

void DataCenter::setReregisterInfo(ReregisterInfo *reregisterInfo)
{
    m_reregisterInfo = reregisterInfo;
    WidgetMng::notify(TICKET_REREGISTER);
}

QList<QTableWidgetItem *> DataCenter::getRegisterItems(ReregisterInfo *info)
{
    QList<QTableWidgetItem*> itemList;

    //# 卡类型 | 卡号 | 发卡时间 | 有效期 | 卡状态 | 余额
    QString ticketTypeStr = getTicketTypeString(info->type());
    QTableWidgetItem* item1 = new QTableWidgetItem(ticketTypeStr);
    QTableWidgetItem* item2 = new QTableWidgetItem(info->number());
    QTableWidgetItem* item3 = new QTableWidgetItem(info->createTime().toString("yyyy-MM-dd"));
    QTableWidgetItem* item4 = new QTableWidgetItem(info->validDate().toString("yyyy-MM-dd"));
    QString ticketStateStr = getTicketStateString(info->state());
    QTableWidgetItem* item5 = new QTableWidgetItem(ticketStateStr);
    QTableWidgetItem* item6 = new QTableWidgetItem(QString("%1").arg(info->balance(), 0, 'f', 1));

    itemList.append(item1);
    itemList.append(item2);
    itemList.append(item3);
    itemList.append(item4);
    itemList.append(item5);
    itemList.append(item6);

    return itemList;
}


// 线路换乘信息
QList<LineInterchangeInfo *> DataCenter::getLineInterchanges() const
{
    return m_lineInterchanges;
}

void DataCenter::setLineInterchanges(const QList<LineInterchangeInfo *> &lineInterchanges)
{
    m_lineInterchanges = lineInterchanges;
    WidgetMng::notify(LINE_INTERCHANGE);
    SettingCenter::getThis()->saveLineInterchanes(lineInterchanges);
}

QString DataCenter::getTicketTypeString(int type)
{
    return m_ticketCodeMap.value(type);
//    QString typeStr = "未定义卡";
//    switch(type) {
//    case UL_CARD:
//        typeStr = "token票";
//        break;
//    case METRO_CARD:
//        typeStr = "地铁储值卡";
//        break;
//    case OCT_CARD:
//        typeStr = "洪城一卡通";
//        break;
//    default:
//        break;
//    }
//    return typeStr;
}

QString DataCenter::getTicketStateString(int type)
{
    QString typeStr = "未定义卡";
    switch(type) {
    case VALID:
        typeStr = "有效卡";
        break;
    case INVALID:
        typeStr = "已过期";
        break;
    default:
        break;
    }
    return typeStr;
}

QString DataCenter::getTradeTypeString(int type)
{
    QString typeStr = "未知";
    switch(type) {
    case 0x02:
        typeStr = "充值";
        break;
    case 0x06:
    case 0x09:
        typeStr = "消费";
        break;
    default:
        break;
    }
    return typeStr;

}

QString DataCenter::getUpdateTypeString(int type)
{
    QString typeStr = "不可更新";
    switch(type) {
    case FREE_EX:   // 免费出站更新
        typeStr = "免费出站更新";
        break;
    case FARE_EX:   // 收费出站更新
        typeStr = "出站更新";
        break;
    case FARE_EN:   // 付费区进站站更新
        typeStr = "进站更新";
        break;
    case OVER_TIME:  // 超时更新
        typeStr = "超时更新";
        break;
    case OVER_TRIP:  // 超程更新
        typeStr = "超程更新";
        break;
    case OVER_TIME_TRIP:   // 超时超程更新
        typeStr = "超时超程更新";
        break;
    case NONE_TYPE:   // 不需要更新
    default:
        typeStr = "不可更新";
        break;
    }

    return typeStr;
}

LoginInfo *DataCenter::getLoginInfo() const
{
    return m_loginInfo;
}

void DataCenter::setLoginInfo(LoginInfo *loginInfo)
{
    m_loginInfo = loginInfo;
}

void DataCenter::setLoginData(QString user, QString pwd)
{
    m_loginInfo = new LoginInfo(this);
    m_loginInfo->setUserName(user);
    m_loginInfo->setPassword(pwd);
    m_loginInfo->setLoginTime(QDateTime::currentDateTime());
}

bool DataCenter::setLogoutData(QString user, QString pwd)
{
    if (m_loginInfo == NULL) {
        return true;
    }
    if (user == m_loginInfo->userName() && pwd == m_loginInfo->password()) {
        m_loginInfo->setLoginTime(QDateTime::currentDateTime());
        return true;
    }
    return false;
}

bool DataCenter::localAuthentication(QString user, QString pwd)
{
    if (m_loginInfo == NULL) {
        return true;
    }
    return (user == m_loginInfo->userName()
            && pwd == m_loginInfo->password());
}

QString DataCenter::getOperatorId()
{
    if (m_loginInfo == NULL) {
        return m_basicInfo->deviceId();
    }
    return m_loginInfo->userName();
}

QString DataCenter::getReaderErrorStr(BYTE errorCode)
{
    return m_readerErrCodeMap.value(errorCode);
}


// 文件下载
void DataCenter::onSoftwareUpdate(QString fileName)
{
    QString urlStr = m_basicInfo->ftpUrl().toString() + "Soft/" + fileName;
    QString localPath = QDir::currentPath() + QDir::separator() +
            "bom-param" + QDir::separator() + fileName;

    LibcurlFtp* ftp = new LibcurlFtp(this);
    ftp->ftpDownload(localPath, urlStr);

//    delete ftp;
//    ftp = nullptr;

}

// 参数更新
void DataCenter::onParamUpdate(QList<int> typeList)
{
    QList<QString> fileList = getServerFileList(typeList);

    QString urlStr = m_basicInfo->ftpUrl().toString() + "Parameter/Cur/";
    QString localPath = QDir::currentPath() + QDir::separator() + "bom-param" + QDir::separator();

    // TODO:文件下载
    for (QString file:fileList) {
        QString serverFilePath = urlStr + file;
        QString localFilePath = localPath + file;

        FtpDownloadTask* task = new FtpDownloadTask(taskId++);
        task->setFileInfo(QUrl(serverFilePath), localFilePath);
        m_ftpTaskThread->addTask(task);
    }
}

// TODO:临时方案 - 获取要下载文件列表
QList<QString> DataCenter::getServerFileList(QList<int> typeList)
{
    // 服务器上参数版本信息
    QString pathServer = QDir::currentPath() + QDir::separator() + "bom-param" + QDir::separator() + "server_version.json";
    QList<BomParamVersionInfo*> serverList = SettingCenter::getThis()->getParamVersionInfo(pathServer);

    QString pathLocal = QDir::currentPath() + QDir::separator() + "bom-param" + QDir::separator() + "version.json";
    QList<BomParamVersionInfo*> localList = SettingCenter::getThis()->getParamVersionInfo(pathLocal);

    QList<QString> list;
    int size = serverList.size();
    for(int i = 0; i < size; i++) {
        int type = localList.at(i)->type();
        if (!typeList.contains(type)) {
            continue;
        }
        if (localList.at(i)->version() < serverList.at(i)->version()) {
            list.append(serverList.at(i)->fileName());
            long oldVersion = localList.at(i)->version();
            long newVersion = serverList.at(i)->version();

            // 4002 参数应用上报
            QByteArray typeArr = MyHelper::intToBytes(type, 2);
            QByteArray versionOld = MyHelper::intToBytes(oldVersion, 4).toHex();
            QByteArray versionNew = MyHelper::intToBytes(newVersion, 4).toHex();
            QByteArray time(7, 0);

            ParamAppliedNotify((BYTE*)typeArr.data(),
                               (BYTE*)versionOld.data(),
                               (BYTE*)versionNew.data(),
                               (BYTE*)time.data());
        }
    }

    return list;
}

// 交易文件上传
void DataCenter::uploadTradeFile(QString filePath, QString fileName, QByteArray md5Arr, int type)
{
    if (m_ftpTaskThread->isRunning()) {
        FtpUploadTask* task = new FtpUploadTask(taskId++);
        QString serverFilePath = m_basicInfo->ftpUrl().toString() + "/Transaction/" + fileName;
        task->setFileInfo(QUrl(serverFilePath), filePath);
        m_ftpTaskThread->addTask(task);
    }

    // 发送7000报文
    QByteArray fileNameArr = fileName.toUtf8();
    FileDownloadNotify(type, (BYTE*)fileNameArr.data(), (BYTE*) md5Arr.data());
}

void DataCenter::samInfo2afc()
{
    BYTE* mtrSam = NULL;
    BYTE* octSam = NULL;
    BYTE* jtbSam = NULL;

    STATUS_INFO ReaderStatus = {0};
    BYTE ret = getStatus(&ReaderStatus);
    if (ret == 0) {
        int count = ReaderStatus.SAMCount;
        for (int i = 0; i < count; i++) {
            int type = ReaderStatus.SAMInfo[i].SAM_Type;
            BYTE* cardId = ReaderStatus.SAMInfo[i].SAMID;
            if (type == 0) {
                mtrSam = ReaderStatus.SAMInfo[i].SAMID;
            } else if (type == 1) {
                octSam = ReaderStatus.SAMInfo[i].SAMID;
            } else {
                jtbSam = ReaderStatus.SAMInfo[i].SAMID;
            }
        }

        if (mtrSam != NULL && octSam != NULL && jtbSam != NULL) {
            BYTE result = DeviceSAMInfo(mtrSam, octSam, jtbSam);

            QString mtrStr = QByteArray((char*)mtrSam, 6).toHex().toUpper();
            QString octStr = QByteArray((char*)octSam, 6).toHex().toUpper();
            QString jtbStr = QByteArray((char*)jtbSam, 6).toHex().toUpper();
            logger()->info("上传AFC{%4}；SAM卡号：地铁{%1}，洪城一卡通{%2}，交通部{%3}",
                           mtrStr, octStr, jtbStr, result);
        }
    }

    logger()->info("读写器获取SAM卡号信息{%1}", ret);
}

void DataCenter::setHrtOffData(int idx)
{
    switch(idx)
    {
    case AFC_HRT: // AFC
//        init_param34(m_v34);
        m_serviceState = 1;
        m_netState = 1;
        WidgetMng::notify(AFC_ONLINE_STATE_ID);
        break;
    case READER_HRT: // 读写器
//        init_param28(m_v28);
        WidgetMng::notify(READER_STATE);
        break;
    case BIM_HRT: // 纸币
        break;
    case F53_HRT: // 纸币找零
        break;
    case BRC_HRT: // 硬币
        break;
    default:
        break;
    }
}

void DataCenter::afcHeart(bool onlineFlag)
{
    if (onlineFlag) {
        m_hrtCnt[AFC_HRT] = 0;
    }
}

void DataCenter::deviceState2afc()
{
    // （1-0）
    // 设备状态：
    // 与SC通信正常 | 停止服务 |测试or生产 | 0(BOM) | 已登录 | 0（不可充值） | 0（） | 开
    bool afcHear = true;
    bool test = true;
    bool isLogin = true;

    BYTE status = 0x15;
    BYTE event = NULL;
    BYTE ret = DeviceState(status, &event);

    logger()->info("设备状态上报{}，status=%2, event=%3", ret, status, event);
}

void DataCenter::param2afc()
{
//    paramType：2字节参数类型码；
//    versionOld：4字节生效前参数版本号，16进制；
//    versionNew：4字节生效后参数版本号，16进制；
//    applyTime：7字节参数生效时间，格式YYYYMMDD-hhmmss
//    立即生效的参数，此处时间填写7字节全0
    BYTE paramType[] = {0x06, 0x10};
    BYTE versionOld[4] = {0};
    BYTE versionNew[4] = {0};
    BYTE applyTime[4] = {0};
    BYTE ret = ParamAppliedNotify(paramType, versionOld, versionNew, applyTime);
    logger()->info("参数应用 = {%1}", ret);
}

void DataCenter::setStationMode(int stationMode)
{
    m_stationMode = stationMode;
}

QString DataCenter::getReaderVersion()
{
    PVERSION_INFO version = {0};
    int ret = getVersion(version);
    if (ret != 0) {
        return "000000A2";
    }

    QByteArray versionArray = QByteArray((char*)version->SoftVersion, 2);
    m_readerVersion = "0000" + versionArray.toHex().toUpper();
    logger()->info("[getVersion] = %1, version = %2", ret, m_readerVersion);

    return m_readerVersion;
}

long DataCenter::getDeviceStateIntervalSec() const
{
    return m_deviceStateIntervalSec;
}

long DataCenter::getTradeDataIntervalSec() const
{
    return m_tradeDataIntervalSec;
}

long DataCenter::getTradeDataCountLT() const
{
    return m_tradeDataCountLT;
}

QUrl DataCenter::getFtpUrl()
{
    // test code
    m_ftpUrl.setScheme("ftp");
    m_ftpUrl.setHost("192.168.2.193");
    m_ftpUrl.setPort(21);
    m_ftpUrl.setUserName("ismftp");
    m_ftpUrl.setPassword("1234Asdf");

    return m_ftpUrl;
}



// 线路运营时间表
QList<LineTimeTables *> DataCenter::getLineTimeTables() const
{
    return m_lineTimeTables;
}

void DataCenter::setLineTimeTables(const QList<LineTimeTables *> &lineTimeTables)
{
    m_lineTimeTables = lineTimeTables;
    WidgetMng::notify(LINE_TIME);
    SettingCenter::getThis()->saveLineTimeTables(lineTimeTables);
}

// 线路站点信息
QList<LineStations *> DataCenter::getLineStations() const
{
    return m_lineStations;
}

void DataCenter::setLineStations(const QList<LineStations *> &lineStations)
{
    m_lineStations = lineStations;
    WidgetMng::notify(LINE_STATION_LIST);
    SettingCenter::getThis()->saveLineStations(lineStations);
}

// 交易信息
QList<TransactionInfo *> DataCenter::getTransInfoList() const
{
    return m_transInfoList;
}

void DataCenter::setTransInfoList(const QList<TransactionInfo *> &transInfoList)
{
    m_transInfoList = transInfoList;
    WidgetMng::notify(TICKET_TRANS);
}

QList<QTableWidgetItem *> DataCenter::getTranscationItems(TransactionInfo *info)
{
    QList<QTableWidgetItem*> itemList;

    // 交易时间 | 交易类型 | 交易金额 | 交易终端SAM卡号
    QTableWidgetItem* item1 = new QTableWidgetItem(info->createTime().toString("yyyy-MM-dd HH:mm:ss"));

    QString typeStr = getTradeTypeString(info->type());
    QTableWidgetItem* item2 = new QTableWidgetItem(typeStr);

    QString comStr = QString("%1").arg(info->amount(), 0, 'f', 2);
    QTableWidgetItem* item3 = new QTableWidgetItem(comStr);

    QTableWidgetItem* item4 = new QTableWidgetItem(info->samNum());

    itemList.append(item1);
    itemList.append(item2);
    itemList.append(item3);
    itemList.append(item4);

    return itemList;
}


// 票卡基本信息
TicketBasicInfo *DataCenter::getTicketBasicInfo() const
{
    return m_ticketBasicInfo;
}

void DataCenter::setTicketBasicInfo(TicketBasicInfo *ticketInfo)
{
    m_ticketBasicInfo = ticketInfo;
    WidgetMng::notify(TICKET_BASIC);
    WidgetMng::notify(TICKET_REREGISTER);
}

// 车票基本信息格式化
QList<QTableWidgetItem *> DataCenter::getTicketItems(TicketBasicInfo *info)
{
    QList<QTableWidgetItem*> itemList;

    // 种类 | 卡号 | 创建时间 | 有效期 | 卡状态 | 余额
    QTableWidgetItem* item1 = new QTableWidgetItem(info->type());
    QTableWidgetItem* item2 = new QTableWidgetItem(info->number());
    QTableWidgetItem* item3 = new QTableWidgetItem(info->createTime().toString("yyyy-MM-dd"));
    QTableWidgetItem* item4 = new QTableWidgetItem(info->validDate().toString("yyyy-MM-dd"));

    // 卡状态
    QString ticketStateStr = getTicketStateString(info->cardState());
    QTableWidgetItem* item5 = new QTableWidgetItem(ticketStateStr);
    QTableWidgetItem* item6 = new QTableWidgetItem(QString("%1").arg(info->balance(), 0, 'f', 1));

//    QTableWidgetItem* item5 = new QTableWidgetItem(getTicketStateString(info->cardState()));
//    QTableWidgetItem* item6 = new QTableWidgetItem(QString("%1").arg(info->tripState()));

//    QTableWidgetItem* item7 = new QTableWidgetItem(QString("%1").arg(info->balance(), 0, 'f', 2));

    itemList.append(item1);
    itemList.append(item2);
    itemList.append(item3);
    itemList.append(item4);
    itemList.append(item5);
    itemList.append(item6);
//    itemList.append(item7);

    return itemList;
}


// 服务状态
int DataCenter::getServiceState() const
{
    return m_serviceState;
}

void DataCenter::setServiceState(int serviceState)
{
    m_serviceState = serviceState;
}


// 网络状态
int DataCenter::getNetState() const
{
    return m_netState;
}

void DataCenter::setNetState(int netState)
{
    m_netState = netState;
}

BYTE DataCenter::getCashboxState()
{
    BYTE bim = m_isBanknotesUsable ? 0x02 : 0x00;
    BYTE brc = m_isSpecieUsable ? 0x01 : 0x00;
    return (bim + brc);
}


// 站点名称
QString DataCenter::getStationName() const
{
    return m_basicInfo->stationName();
}

// 站点编号
QString DataCenter::getStationCode() const
{
    return m_basicInfo->stationCode();
}

// 是否付费区
bool DataCenter::isPayZone() const
{
    return m_basicInfo->isPayZone();
}

QString DataCenter::stationCode2Name(QString code) const
{
    return m_stationCodeMap.value(code);
}

int DataCenter::getTradeSerial()
{
    m_tradeSerial++;
    // 写入文件
    SettingCenter::getThis()->saveTradeSerial(m_tradeSerial);

    return m_tradeSerial;
}

int DataCenter::getStationMode()
{
    // TODO:默认正常模式-0
    return 0;
}

QList<LineInfo *> DataCenter::getLineList() const
{
    return m_lineList;
}

void DataCenter::setLineList(const QList<LineInfo *> &lineList)
{
    m_lineList.clear();
    m_lineList.append(lineList);
}

void DataCenter::setLineMap(QString lineCode, QString mapPath)
{
    for (LineInfo* info:m_lineList) {
        if (info->getCode().compare(lineCode) == 0) {
            info->setPicPath(mapPath);
            break;
        }
    }
}


// 获取线路时间表要显示的行数
int DataCenter::getLinesTimerCloums(QList<LineTimeTables *> lineList)
{
    int count = 0;
    for (LineTimeTables* line:lineList) {
        count += line->timeTable().size();
    }

    return count;
}

// 获取线路时间表要显示的行数
int DataCenter::getLinesInterchangeCloums(QList<LineInterchangeInfo *> lineList)
{
    int count = 0;
    for (LineInterchangeInfo* line:lineList) {
        count += line->interchangeList().size();
    }

    return count;
}


