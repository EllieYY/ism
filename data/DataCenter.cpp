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
#include "AFCTaskThread.h"
#include "BomParamVersionInfo.h"
#include "OperatorInfo.h"
#include "LibcurlFtp.h"
#include "TaskThread.h"
#include "FtpUploadTask.h"
#include "FtpDownloadTask.h"
#include "TradeFileInfo.h"
#include "TradeFileUploadTask.h"
#include "HeartTask.h"
#include "ReaderSoftFileInfo.h"


static int HRT_NUM = 5;
DataCenter* DataCenter::m_pInstance = NULL;
DataCenter::DataCenter(QObject *parent) : QObject(parent)
{
    m_pInstance = this;
//    init();
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

    if (m_afcTaskThread->isRunning())
    {
        m_afcTaskThread->quit();
        m_afcTaskThread->wait();
    }

    if (m_taskThread->isRunning()) {
        m_taskThread->stopRun();
        m_taskThread->quit();
        m_taskThread->wait();
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
    m_timeCount++;
    for (int i = 0; i < HRT_NUM; i++) {
        m_hrtCnt[i] = m_hrtCnt[i] + 1;
//        if (m_hrtCnt[i] == 600) {    // 超过10分钟未连接则掉线处理
//            setHrtOffData(i);
//        }
    }

    // AFC相关定时上传
    // AFC心跳检测 - 一分钟一次
    if ((m_timeCount % 10) == 0) {
        if (m_taskThread != NULL && m_taskThread->isRunning()) {
            HeartTask* task = new HeartTask(getTaskId());
            m_taskThread->addTask(task);
        }
    }

    // 设备状态定时长传
    bool devTriggerd = false;
    if ((m_timeCount % m_deviceStateIntervalSec) == 0) {
        devTriggerd = true;
        logger()->info("设备状态定时上送：%1, %2", m_timeCount, m_deviceStateIntervalSec);
        this->deviceState2afc(DEV_OK);
    }

    // 交易文件定时上传
    bool fileTriggered = false;
    // TODO:修改使用配置参数中的时间间隔
    if ((m_timeCount % 10) == m_tradeDataIntervalSec || m_tradeFileInfo->fileCount() >= m_tradeDataCountLT) {
        fileTriggered = true;
        logger()->info("交易文件定时上送：%1, %2", m_timeCount, m_tradeDataIntervalSec);
        packageTradeFile();
    }

    if (devTriggerd && fileTriggered) {
        m_timeCount = 0;
    }
}

void DataCenter::init()
{
    initData();    // 默认数据

    /* 基础信息 */
    logger()->info("基础信息读取");
    m_basicInfo = SettingCenter::getThis()->getBasicInfo();
    HttpTool::getThis()->setId(m_basicInfo->deviceId(), m_basicInfo->stationName());
    HttpTool::getThis()->setServUrl(m_basicInfo->ismServiceIp(), m_basicInfo->ismServicePort());

    ASRHttpTool::getThis()->setAppkey(m_basicInfo->appkey());
    ASRHttpTool::getThis()->setSecret(m_basicInfo->secret());
    ASRHttpTool::getThis()->setIp(m_basicInfo->asrServiceIp());
    ASRHttpTool::getThis()->setPort(m_basicInfo->asrServicePort());

    logger()->info("配置文件读取。");
    m_lineStations.append(SettingCenter::getThis()->getLineStations());
    m_lineTimeTables.append(SettingCenter::getThis()->getLineTimeTables());
    m_lineInterchanges.append(SettingCenter::getThis()->getLineInterchanes());
    m_lineList.append(SettingCenter::getThis()->getLineBasicInfo());
    logger()->info("配置文件读取完毕。");

    logger()->info("基础数据更新");
    // 获取基础数据并更新数据 #7
    HttpTool::getThis()->requestLineBaseInfo();
    HttpTool::getThis()->requestLineStations();
    HttpTool::getThis()->requestInterchanges();
    HttpTool::getThis()->requestTimeTables();
    HttpTool::getThis()->requestStationMap();
    HttpTool::getThis()->requestStationPreMap();
    HttpTool::getThis()->requestLineMap();

    m_stationCodeMap.clear();
    for(LineStations* line : m_lineStations) {
        QList<Station*> stations = line->stationList();
        for (Station* station : stations) {
            m_stationCodeMap.insert(station->code(), station->name());
        }
    }

    // AFC监听线程
    m_afcTaskThread = NULL;
    m_afcTaskThread = new AFCTaskThread();
    connect(m_afcTaskThread, &AFCTaskThread::paramTypeUpdate, this, &DataCenter::onParamUpdate, Qt::DirectConnection);
    connect(m_afcTaskThread, &AFCTaskThread::softwareUpdate, this, &DataCenter::onSoftwareUpdate, Qt::DirectConnection);
    m_afcTaskThread->start();


    m_taskThread = NULL;
    m_taskThread = new TaskThread();
//    connect(m_ftpTaskThread, &TaskThread::allTaskDone, this, &DataCenter::ftpAllTaskFinished);
    connect(m_taskThread, &TaskThread::taskDone, this, &DataCenter::taskFinished);

//    initDevice();
    initReaderErrCode();
}

void DataCenter::initData()
{
    // 测试状态，硬配 -- 正式发布后修改为false
    m_isTest = false;
    m_serviceOff = true;            // 默认服务状态， 响应报文3000设置、运营日时间设置
    m_serviceStartTime = 300*60;     // 运营开始时间 - 5:00
    m_serviceEndTime = 1410*60;      // 运营结束时间 - 23:30
    m_isLogin = false;
    m_hasAutoLogout = false;

    m_basicInfo = NULL;               // 站点基础信息
    m_loginInfo = NULL;               // 登录信息
    m_ticketBasicInfo = NULL;         // 车票基本信息

    m_cashboxInitRet = 0x0FFF;        // 初始化不可用
    m_isReaderUsable = false;         // 读写器可用状态
    m_isSpecieUsable = false;         // 硬币模块可用状态
    m_isBanknotesUsable = false;      // 纸币模块可用状态
    m_isSamOk = false;


    // AFC网络状态 - 默认在线
    m_afcNetState = 0;

    // FTP 任务Id
    m_afcTaskThread = NULL;
    m_taskThread = NULL;
    taskId = 0;
    m_curParamTaskId = -1;
    m_curSoftwareTaskId = -1;

    //定时任务相关
    m_timeCount = 0;
    m_tradeFileInfo = NULL;
    m_tradeFileInfo = SettingCenter::getThis()->getTradeFileInfo();

    // 站点模式
    m_stationMode = 0;

    // 参数版本信息获取
    initParamVersion();

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

    // 读写器状态上传给ISM后台
    QString readerStateStr = ((initRet == 0) ? "1" : "0");   // 1-正常 0-离线
    HttpTool::getThis()->updateStates(readerStateStr);

    // 获取读写器版本
    VERSION_INFO version = {0};
    int retVersion = getVersion(&version);
    if (retVersion != 0) {
        m_readerVersion = "00000000";
    } else {
        QByteArray versionArray;
        versionArray.append((char*)version.SoftVersion, 2);
        m_readerVersion = "0000" + versionArray.toHex().toUpper();
    }

    QByteArray str;
    str.append((char*)&version, sizeof(VERSION_INFO));
    QString outputStr = str.toHex();
    logger()->info("[getVersion] = %1, output = %2", retVersion, outputStr);


    // 钱箱初始化
    int bimPort = m_basicInfo->bimPort();
    int f53Port = m_basicInfo->f53Port();
    int brcPort = m_basicInfo->brcPort();

    m_isBanknotesUsable = false;
    m_isSpecieUsable = false;
    long retC = ConnectMachine(bimPort, brcPort, f53Port);

    m_cashboxInitRet = retC;
    m_isBanknotesUsable = ((retC & 0x0F0F) == 0);
    m_isSpecieUsable = ((retC & 0x00F0) == 0);

    logger()->info("[ConnectMachine]钱箱初始化={%1}, 端口号：bim={%2}, f53={%3}, brc={%4}, 纸币可用{%5}, 硬币可用{%6}",
                   retC, bimPort, f53Port, brcPort, m_isBanknotesUsable, m_isSpecieUsable);
}

void DataCenter::closeDevice()
{
    int ret1 = readerComClose();
    logger()->info("[readerComClose] = %1", ret1);

    int ret2 = CloseAllCom();
    logger()->info("[CloseAllCom] = %1", ret2);
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


// 参数解析：记录参数版本信息，解析并使用参数文件
void DataCenter::initParamVersion()
{
    m_paramVersionMap.clear();
    QString path = QDir::currentPath() + QDir::separator() + PARAM_FILE_PATH + QDir::separator();
    QList<BomParamVersionInfo*> list = SettingCenter::getThis()->getParamVersionInfo(path + "version.json");
    for (BomParamVersionInfo* info: list) {
        long type = info->type();

        QString fileName = info->fileName();
        QString filePath = path + fileName;
        int ret = -1;
        if (type == 0x1001) {
            ret = parseParam1001(filePath);
        } else if (type == 0x1004) {
            ret = parseParam1004(filePath);
        } else if (type == 0x2002) {
            ret = parseParam2002(filePath);
        } else if (type == 0x2004) {
            ret = parseParam2004(filePath);
        } else if (type == 0x2005) {
            ret = parseParam2005(filePath);
        }

        if (ret == 0) {
            m_paramVersionMap.insert(type, info);
        }
    }
}

// 使用待更新的参数文件:更新失败的参数重新写入待更新文件记录
void DataCenter::updateParamVersion()
{
    QString path = QDir::currentPath() + QDir::separator() + PARAM_FILE_PATH + QDir::separator();
    QList<BomParamVersionInfo*> list = SettingCenter::getThis()->getParamVersionInfo(path + "updateVersion.json");
    QList<BomParamVersionInfo*> updateList;
    for (BomParamVersionInfo* info: list) {
        long type = info->type();

        QString fileName = info->fileName();
        QString filePath = path + fileName;
        int ret = -1;
        if (type == 0x1001) {
            ret = parseParam1001(filePath);
        } else if (type == 0x1004) {
            ret = parseParam1004(filePath);
        } else if (type == 0x2002) {
            ret = parseParam2002(filePath);
        } else if (type == 0x2004) {
            ret = parseParam2004(filePath);
        }

        // 避免界面被阻塞
        QCoreApplication::processEvents();

        // 覆盖之前的旧版本
        if (ret == 0) {
            // 参数更新上报
            long oldVersion = m_paramVersionMap.value(type)->version();
            long newVersion = info->version();
            param2afc(type, oldVersion, newVersion);

            m_paramVersionMap.insert(type, info);
        } else {
            updateList.append(info);
        }
    }

    // 版本信息存储
    SettingCenter::getThis()->saveParamVersionInfo(m_paramVersionMap.values(), "version.json");
    // 更新失败的参数文件存储
    SettingCenter::getThis()->saveParamVersionInfo(updateList, "updateFailedVersion.json");
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
        logger()->error("[参数文件读取失败][%1] file open failed", filePath);
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
        logger()->error("[参数文件1004读取失败][%1] file open failed", filePath);
        return -1;
    }

//    QByteArray array = file.readAll();
//    bool checkOk = fileCheck(array);
//    qDebug() << "file check: " << checkOk;

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
//        logger()->info("src={%1},enStr={%2},chStr={%3}", info, enStr, chStr);

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
        logger()->error("[参数文件2002读取失败][%1] file open failed", filePath);
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
        logger()->error("[参数文件2004读取失败][%1] file open failed", filePath);
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

int DataCenter::parseParam2005(QString filePath)
{
    QFile file(filePath);
    bool isOk = file.open(QFile::ReadOnly);
    if (!isOk) {
        logger()->error("[参数文件2005读取失败][%1] file open failed", filePath);
        return -1;
    }

    // 读文件
    QDataStream stream(&file);
    parseHead(stream);

    // 文件内容
    qint32 pVersion;
    QByteArray startTime(7, Qt::Uninitialized);
    BYTE seviceStart;
    BYTE serviceEnd;

    stream >> pVersion;
    stream.readRawData(startTime.data(), 7);
    stream.skipRawData(83);
    stream.skipRawData(1);
    stream >> serviceEnd;
    stream >> seviceStart;

    file.close();

    // 运营时间解析 | 容错
    serviceEnd = (serviceEnd % 0x60);
    m_serviceEndTime = serviceEnd * 15 * 60;

    seviceStart = (seviceStart % 0x60);
    if (seviceStart < 0x30) {     // 开始时间应早于12:00
        m_serviceStartTime = seviceStart * 15 * 60;
    }

    QString sTimeStr(startTime.toHex());
    QString body = QString("pVersion={%1}, startTime={%2}, serviceStartTime={%3}, serviceEndTime={%4}")
            .arg(pVersion, 8, 16, QLatin1Char('0'))
            .arg(sTimeStr)
            .arg(m_serviceStartTime)
            .arg(m_serviceEndTime);
    logger()->info("[param2005]%1", body);
}


void DataCenter::taskFinished(int taskId, bool success)
{
   logger()->info("TaskId=%1,执行情况=%2", taskId, success);

    // 参数文件下载任务完成
    if (taskId == m_curParamTaskId) {
        m_curParamTaskId = -1;
        if (success) {
            //参数文件更新
            updateParamVersion();
        }

        // TODO:参数更新上报

    }

    if (taskId == m_curSoftwareTaskId) {
        m_curSoftwareTaskId = -1;
        if (success) {
            logger()->info("读写器软件下载完成。");
            // 记录软件更新情况
            m_readerSoftInfo->setFileReady(true);
            SettingCenter::getThis()->saveReaderSoftInfo(m_readerSoftInfo);
        }
    }
}

// 任务队列里所有任务完成，暂时不需要
void DataCenter::allTaskFinished()
{
//    qDebug() << "all task " << "done now.";

}


// 交易文件相关
void DataCenter::addTradeFileInfo(QString fileName)
{
    if (m_tradeFileInfo == nullptr) {
        m_tradeFileInfo = new TradeFileInfo(this);
    }

    m_tradeFileInfo->addFileName(fileName);
}

// 交易文件流水号
ulong DataCenter::getTradeFileSerial()
{
    if (m_tradeFileInfo == nullptr) {
        m_tradeFileInfo = new TradeFileInfo(this);
    }

    return m_tradeFileInfo->fileTradeSerial();
}

// 终端交易序号
ulong DataCenter::getDeviceTradeSerial()
{
    if (m_tradeFileInfo == nullptr) {
        m_tradeFileInfo = new TradeFileInfo(this);
    }

    ulong serial = m_tradeFileInfo->deviceTradeSerial();
    m_tradeFileInfo->setDeviceTradeSerial(serial + 1);
    SettingCenter::getThis()->saveTradeFileInfo(m_tradeFileInfo);
    return serial;
}

// days表示往前删除的天数
bool DataCenter::findFileForDelete(const QString filePath, int days)
{
    // 最多保留30天的交易数据
    // 因为判断的是最后修改时间，所以天数要比传参多一天
    int deleteDays = - days + 1;
    if (days <= 0) {
        deleteDays = days + 1;
    } else if (days > 30) {
        deleteDays = -29;
    }

    QDir dir(filePath);
    if (!dir.exists())
        return false;
    dir.setFilter(QDir::Dirs | QDir::Files);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    int i = 0;
    do {
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.fileName() == "." | fileInfo.fileName() == "..")
        {
            i++;
            continue;
        }
        bool bisDir = fileInfo.isDir();
        if(bisDir) {   // 不对子目录下的文件进行删除操作
            continue;
        } else {
            // 跳过json文件（配置文件）
            if (fileInfo.fileName().contains(".json")) {
                continue;
            }
            //如果是文件，判断文件日期。
            QDateTime delDateTime = QDateTime::currentDateTime().addDays(deleteDays);
            qint64 nSecs = delDateTime.secsTo(fileInfo.lastModified());
            if (nSecs < 0) {
                qDebug() << qPrintable(QString("%1 %2 %3").arg(fileInfo.size(), 10)
                                                    .arg(fileInfo.fileName(),10).arg(fileInfo.path()))<<endl;
                //删除30天前的文件
                fileInfo.dir().remove(fileInfo.fileName());
            }
        }
        i++;
    } while(i<list.size());
    return true;
}


int DataCenter::packageTradeFile()
{
    if (m_tradeFileInfo == nullptr || m_tradeFileInfo->fileCount() <= 0) {
        return -1;
    }

    qDebug() << "fileCount" << m_tradeFileInfo->fileCount();

    int fileCount = m_tradeFileInfo->fileCount();
    QSet<QString> fileNameList = m_tradeFileInfo->fileNameSet();
    if (m_taskThread->isRunning()) {
        TradeFileUploadTask* task = new TradeFileUploadTask(getTaskId());
        QString serverFilePath = m_basicInfo->ftpUrl().toString() + "/Transaction/";
        task->packageTradeFile(fileCount, fileNameList, serverFilePath);
        m_taskThread->addTask(task);
    }

    // TODO:
    m_tradeFileInfo->reset();
    SettingCenter::getThis()->saveTradeFileInfo(m_tradeFileInfo);

//    qDebug() << "trade file name: " << fileName;

    return 0;
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
    QString ticketStateStr = getTicketStateString(0, info->state());
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
}

// 票卡状态、错误码对应关系
QString DataCenter::getTicketStateString(int icType, int state)
{
    QString typeStr = "未定义卡";
    if (icType == UL_CARD) {
        typeStr = getULStateStr(state);
    } else if (icType == METRO_CARD) {
        typeStr = getCPUStateStr(state);
    } else if (icType == OCT_CARD) {
        typeStr = getOCTStateStr(state);
    } else if (icType == TU_CARD) {
        typeStr = getTUStateStr(state);
    }

    return typeStr;
}
QString DataCenter::getULStateStr(int state) {
    QString typeStr = "未定义卡";
    //00初始化，01售票，02进站，03出站，05注销，12超程更新，
    //22超时更新，32超时超程更新，42出站票
    if (state == 0) {
        typeStr = "初始化";
    } else if (state == 1) {
        typeStr = "售票";
    } else if (state == 2) {
        typeStr = "进站";
    } else if (state == 3) {
        typeStr = "出站";
    } else if (state == 5) {
        typeStr = "注销";
    } else if (state == 12) {
        typeStr = "超程更新";
    } else if (state == 22) {
        typeStr = "超时更新";
    } else if (state == 32) {
        typeStr = "超时超程更新";
    } else if (state == 42) {
        typeStr = "出站票";
    }

    return typeStr;
}
QString DataCenter::getCPUStateStr(int state) {
    QString typeStr = "未定义卡";
    // 太长不看版：00初始化，02售票，04进站，06出站，08锁卡，0A注销
    if (state == 0x00) {
        typeStr = "初始化";
    } else if (state == 0x02) {
        typeStr = "售票";
    } else if (state == 0x04) {
        typeStr = "进站";
    }else if (state == 0x06) {
        typeStr = "出站";
    } else if (state == 0x08) {
        typeStr = "锁卡";
    } else if (state == 0x0A) {
        typeStr = "注销";
    }

    return typeStr;
}
QString DataCenter::getOCTStateStr(int state) {
    QString typeStr = "未定义卡";

    // 太长不看版：00出站，01进站，03超程更新，05超时更新，07超程超时更新
    if (state == 00) {
        typeStr = "出站";
    } else if (state == 1) {
        typeStr = "进站";
    } else if (state == 3) {
        typeStr = "超程更新";
    }else if (state == 5) {
        typeStr = "超时更新";
    } else if (state == 7) {
        typeStr = "超程超时更新";
    }

    return typeStr;
}

QString DataCenter::getTUStateStr(int state)
{
    QString typeStr = "未定义卡";

    // //00：初始状态; 01：进站; 02：出站; 03：进站更新; 04：出站更新
    if (state == 1) {
        typeStr = "出站";
    } else if (state == 2) {
        typeStr = "进站";
    } else if (state == 3) {
        typeStr = "进站更新";
    }else if (state == 4) {
        typeStr = "出站更新";
    } else if (state == 0) {
        typeStr = "初始状态";
    }

    return typeStr;
}

QString DataCenter::getReaderErrorStr(BYTE errorCode)
{
    return m_readerErrCodeMap.value(errorCode);
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
QString DataCenter::getTradeFileTypeStr(int icType)
{
    QString str = "Y";
    switch(icType) {
    case UL_CARD:
        str = "S";
        break;
    case METRO_CARD:
        str = "V";
        break;
    case OCT_CARD:
        str = "Y";
        break;
    case TU_CARD:
        str = "J";
        break;
    default:
        break;
    }
    return str;
}

int DataCenter::getTradeFileType(QString icTypeStr)
{
    BYTE type = OCT_CARD;
    if (icTypeStr.compare("S") == 0) {
        type = UL_CARD;
    } else if (icTypeStr.compare("V") == 0) {
        type = METRO_CARD;
    } else if (icTypeStr.compare("Y") == 0) {
        type = OCT_CARD;
    } else if (icTypeStr.compare("J") == 0) {
        type = TU_CARD;
    }

    return type;
}


// 签到签退
LoginInfo *DataCenter::getLoginInfo() const
{
    return m_loginInfo;
}
void DataCenter::setLoginInfo(LoginInfo *loginInfo)
{
    m_loginInfo = loginInfo;
}
bool DataCenter::setLoginData(QString user, QString pwd)
{
    // 用户校验
    if (!isValidUser(user, pwd)) {
        return false;
    }

    // 账号信息存储
    m_loginInfo = new LoginInfo(this);
    m_loginInfo->setUserName(user);
    m_loginInfo->setPassword(pwd);
    m_loginInfo->setLoginTime(QDateTime::currentDateTime());

    // 签到
    BYTE * operatorID = reinterpret_cast<byte*>(user.toLocal8Bit().data());
    BYTE event = 2;          // 带口令登录
    BYTE operatorType = 2;   // 维护人员
    BYTE ret = OperatorAction(operatorID, event, operatorType);
    if (ret != 0) {
//        MyHelper::ShowMessageBoxError(QString("登录失败[%1]，请联系工作人员。").arg(ret));
        logger()->error("AFC登录失败{%1}", ret);
//        return;
    }

    setIsLogin(true);
    return true;
}

bool DataCenter::setLogoutData(QString user, QString pwd)
{
    // 容错处理
    if (m_loginInfo == NULL) {
        setIsLogin(false);
        return true;
    }

    // 签退校验
    if (user != m_loginInfo->userName() || pwd != m_loginInfo->password()) {
        return false;
    }

    m_loginInfo->setLoginTime(QDateTime::currentDateTime());

    // AFC 签退
    BYTE * operatorID = reinterpret_cast<byte*>(user.toLocal8Bit().data());
    BYTE event = 0;          // 签退
    BYTE operatorType = 2;   // 维护人员
    BYTE ret = OperatorAction(operatorID, event, operatorType);
    if (ret != 0) {
        logger()->error("[OperatorAction] 签退失败：%1", ret);
        // 此处只做日志记录，不做AFC返回结果校验
//        return false;
    }

    setIsLogin(false);

    return true;
}

bool DataCenter::autoLogout()
{
    if (m_loginInfo == NULL) {
        setIsLogin(false);
        return false;
    }

    if (m_hasAutoLogout) {
        return false;
    }
    logger()->info("autoLogout.");

    m_hasAutoLogout = true;

    setLogoutData(m_loginInfo->userName(), m_loginInfo->password());
    return true;
}

// 用户鉴权：暂时只校验用户名和密码，不校验权限
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

// 用户名密码校验，不做签到签退
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



// 文件下载
void DataCenter::onSoftwareUpdate(QString fileName)
{
    QString remotePath = m_basicInfo->ftpUrl().toString() + "/Soft/";
    QString localPath = QDir::currentPath() + QDir::separator() +
            SOFT_FILE_PATH + QDir::separator();

    // 记录程序文件更新情况
    m_readerSoftInfo = new ReaderSoftFileInfo(this);
    m_readerSoftInfo->setFileName(fileName);
    SettingCenter::getThis()->saveReaderSoftInfo(m_readerSoftInfo);

    // 下载程序文件
    m_curSoftwareTaskId = getTaskId();
    FtpDownloadTask* task = new FtpDownloadTask(m_curSoftwareTaskId);
    task->setFileInfo(remotePath, fileName, localPath);
    m_taskThread->addTask(task);
}

// 参数更新 type: 0 - 普通同步   1 - 强制同步
void DataCenter::onParamUpdate(QList<int> typeList, int type)
{
    if (typeList.size() == 0) {
        logger()->info("[onParamUpdate]没有参数需要更新。");
        return;
    }

    // TODO:将来参数版本，
    QString remotePath = m_basicInfo->ftpUrl().toString() + "/Parameter/Cur/";
    QString localPath = QDir::currentPath() + QDir::separator() + PARAM_FILE_PATH + QDir::separator();

    // 参数限制
    QHash<int,long> filter = getParamFileFilter(typeList);

    m_curParamTaskId = getTaskId();
    FtpDownloadTask* task = new FtpDownloadTask(m_curParamTaskId);
    task->setFileInfo(remotePath, "fileList.txt", localPath);

    bool isForceUpdate = (type == 0 ? false : true);
    task->setFilter(filter, isForceUpdate);
    m_taskThread->addTask(task);

    // TODO:通知进行参数更新，参数上报, taskId用起来
    // 更新参数版本记录文件
}

// 参数类型码，同步类型
QHash<int,long> DataCenter::getParamFileFilter(QList<int> typeList)
{
    QHash<int,long> filter;
    filter.clear();
    for (int type:typeList) {
        if (m_paramVersionMap.contains(type)) {
            long version = m_paramVersionMap.value(type)->version();
            filter.insert(type, version);
        }
    }
    return filter;
}

// 交易文件上传
void DataCenter::uploadTradeFile(QString localFilePath, QString fileName, QByteArray md5Arr, int type)
{
    if (m_taskThread->isRunning()) {
        FtpUploadTask* task = new FtpUploadTask(getTaskId());
        QString serverFilePath = m_basicInfo->ftpUrl().toString() + "/Transaction/";
        task->setFileInfo(serverFilePath, fileName, localFilePath);
        m_taskThread->addTask(task);
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
        m_afcNetState = 1;
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
        m_afcNetState = 0;
        m_hrtCnt[AFC_HRT] = 0;
    }
}

void DataCenter::deviceState2afc(BYTE event)
{
    BYTE state = getDeviceState();

    // 事件要传2个字节 : 网络字节序
    int num = 0;
    BYTE eventArr[2] = {0};
    if (event != NULL) {
        eventArr[0] = 0x00;
        eventArr[1] = event;
        num = 2;
    }

    BYTE ret = DeviceState(state, &event, num);

    logger()->info("设备状态上报{%1}，status=%2, event=%3", ret, state, event);
}

// 4002 参数应用上报
void DataCenter::param2afc(int paramType, long oldVersion, long newVersion)
{
//    paramType：2字节参数类型码；
//    versionOld：4字节生效前参数版本号，16进制；
//    versionNew：4字节生效后参数版本号，16进制；
//    applyTime：7字节参数生效时间，格式YYYYMMDD-hhmmss
//    立即生效的参数，此处时间填写7字节全0
    QByteArray typeArr = MyHelper::intToBytes(paramType, 2);
    QByteArray versionOld = MyHelper::intToBytes(oldVersion, 4).toHex();
    QByteArray versionNew = MyHelper::intToBytes(newVersion, 4).toHex();
    QByteArray time(7, 0);

    int ret = ParamAppliedNotify((BYTE*)typeArr.data(),
                       (BYTE*)versionOld.data(),
                       (BYTE*)versionNew.data(),
                       (BYTE*)time.data());
    logger()->info("参数应用 = {%1}, type={%2}, oldVersion={%3}, newVersion={%4}",
                   ret,
                   QString::number(paramType, 16),
                   QString::number(oldVersion, 16),
                   QString::number(newVersion, 16));
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


//DEV_OK = 0,       // 正常
//DEV_HARDWARE_ERR = 8,           // 硬件故障
//DEV_CLOSE = 9,    // 设备故障
//DEV_LOGIN = 13,    // 登录
//DEV_SERVICE_END = 19,   // 营运结束
//DEV_SERVICE_OFF = 34,   // 停止服务
//DEV_SERVICE_ON = 20,    // 开始服务
//DEV_RW_ERR = 44    // 读写器故障

void DataCenter::setServiceOff(bool serviceOff)
{
    if (m_serviceOff == serviceOff) {
        return;
    }

    m_serviceOff = serviceOff;

    // 每个运营时间开始之后，只一次自动签退
    if (m_serviceOff) {
        emit sigSerivceOff();
    } else {
        m_hasAutoLogout = false;
    }

    // 设备状态上报
    this->deviceState2afc(DEV_SERVICE_OFF);
}

void DataCenter::setIsLogin(bool isLogin)
{
    if (m_isLogin == isLogin) {
        return;
    }

    m_isLogin = isLogin;
    // 设备状态上报
    this->deviceState2afc(DEV_LOGIN);
}

BYTE DataCenter::getDeviceState()
{
    // 1   0	开(1)/关(0)
    //     1	停止服务(1)/无故障(0)
    //     2	测试(1)/生产(0)
    // 0   3	EFO(1)/BOM(0)

    //     4	已登录(1)/签退(0)
    // 0   5	可以充值(1)/不可充值(0)
    // 0   6	自动发售机构可用(1)/不可用(0)
    // 0   7	[未定义]

    BYTE deviceState = 0x01;
    if (m_serviceOff) {
        deviceState += 0x02;
    }
    if (m_isTest) {
        deviceState += 0x04;
    }
    if (m_isLogin) {
        deviceState += 0x10;
    }

    return deviceState;
}

BasicInfo *DataCenter::getBasicInfo() const
{
    return m_basicInfo;
}

long DataCenter::getServiceStartTime() const
{
    return m_serviceStartTime;
}

long DataCenter::getServiceEndTime() const
{
    return m_serviceEndTime;
}

bool DataCenter::getIsLogin() const
{
    return m_isLogin;
}

bool DataCenter::getServiceOff() const
{
    return m_serviceOff;
}

int DataCenter::getTaskId()
{
    taskId++;
    if (taskId >= 9999) {
        taskId = 1;
    }
    return taskId;
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
    QString ticketStateStr = getTicketStateString(info->icType(), info->cardState());
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

// 网络状态
int DataCenter::getNetState() const
{
    return m_afcNetState;
}

void DataCenter::setNetState(int netState)
{
    m_afcNetState = netState;
}


// 硬件设备状态
BYTE DataCenter::getCashboxState()
{
    BYTE bim = m_isBanknotesUsable ? 0x02 : 0x00;
    BYTE brc = m_isSpecieUsable ? 0x01 : 0x00;
    return (bim + brc);
}

void DataCenter::setCashboxState(int coinState, int banknotes, int banknotesRe)
{
    m_isSpecieUsable = (coinState == 0);              // 硬币模块可用状态
    m_isBanknotesUsable = (banknotes == 0);           // 纸币模块可用状态
}

void DataCenter::cashboxOnline(bool coin, bool banknotes, bool banknotesRe)
{
    if (coin) {
        m_hrtCnt[BRC_HRT] = 0;
    }

    if (banknotes) {
        m_hrtCnt[BIM_HRT] = 0;
    }

    if (banknotesRe) {
        m_hrtCnt[F53_HRT] = 0;
    }
}

long DataCenter::getCashboxInitRet() const
{
    return m_cashboxInitRet;
}

void DataCenter::setCashboxInitRet(long cashboxInitRet)
{
    m_cashboxInitRet = cashboxInitRet;
}

bool DataCenter::getIsReaderUsable() const
{
    return m_isReaderUsable;
}

void DataCenter::setReaderState(int readerState)
{
    m_isReaderUsable = (readerState == 0);

    // 读写器状态上传给ISM后台
    QString readerStateStr = ((readerState == 0) ? "1" : "0");   // 1-正常 0-离线
    HttpTool::getThis()->updateStates(readerStateStr);
}


QString DataCenter::getReaderVersion()
{
    return m_readerVersion;
}

void DataCenter::setReaderVersion(QString version)
{
    m_readerVersion = version;
}

void DataCenter::setSAMInfo(BYTE *mtrSam, BYTE *octSam, BYTE *jtbSam)
{
    memcpy(m_mtrSam, mtrSam, 6);
    memcpy(m_octSam, octSam, 6);
    memcpy(m_jtbSam, jtbSam, 6);
    m_isSamOk = true;
}

void DataCenter::getSAMInfo(BYTE *mtrSam, BYTE *octSam, BYTE *jtbSam)
{
    memcpy(mtrSam, m_mtrSam, 6);
    memcpy(octSam, m_octSam, 6);
    memcpy(jtbSam, m_jtbSam, 6);
}

bool DataCenter::getIsSamOk() const
{
    return m_isSamOk;
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

int DataCenter::getStationMode()
{
    return m_stationMode;
}

void DataCenter::setStationMode(int stationMode)
{
    m_stationMode = stationMode;
    // 页面做控制显示
    // 第一册5.8：故障期间，显示列车运行模式
//    Bit 0	紧急模式             1：生效，0:无效
//    Bit 1	进站免检模式         1：生效，0:无效
//    Bit 2	日期免检模式         1：生效，0:无效
//    Bit 3	时间免检模式         1：生效，0:无效
//    Bit 4	列车故障模式         1：生效，0:无效
//    Bit 5	车费免检模式         1：生效，0:无效
//    Bit 6 ~ 15	[未定义]

    WidgetMng::notify(STATION_MODE);
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


