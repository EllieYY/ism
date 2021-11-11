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
#include "ReaderLib.h"
#include "MyHelper.h"
#include "BIM2020.h"


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
}

DataCenter *DataCenter::getThis()
{
    if (m_pInstance == NULL)
        m_pInstance = new DataCenter();
    return m_pInstance;
}

// 心跳连接可以在这里处理
void DataCenter::secEvent()
{
//    emit afcHeartDetect();

    for (int i = 0; i < HRT_NUM; i++) {
        m_hrtCnt[i] = m_hrtCnt[i] + 1;
        if (m_hrtCnt[i] == 10) {    // 超过10秒未连接则掉线处理
            setHrtOffData(i);
        }
    }
}


void DataCenter::init()
{
    initData();    // 默认数据
    connect(this, &DataCenter::afcHeartDetect, this, &DataCenter::afcHeart);

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

    initNetworkLib(deviceId, scId, scIp, scPort, localIp, localPort);

    char version[60];
    getLibVersion(version);
    logger()->info("[getLibVersion]AFC通讯库初始化，获取版本号={%1}", QString(version));


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


// 设备编号
QString DataCenter::getDeviceId() const
{
    return m_basicInfo->deviceId();
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
    QString typeStr = "未定义卡";
    switch(type) {
    case UL_CARD:
        typeStr = "token票";
        break;
    case METRO_CARD:
        typeStr = "地铁储值卡";
        break;
    case OCT_CARD:
        typeStr = "洪城一卡通";
        break;
    default:
        break;
    }
    return typeStr;
}

QString DataCenter::getReregisterTypeString(int type)
{
    QString typeStr = "";
    switch(type) {
    case NONE_TYPE:
        typeStr = "无需更新。";
        break;
    case EN_LACK:
        typeStr = "进站站点信息缺失。";
        break;
    case EX_LACK:
        typeStr = "出站站点信息缺失。";
        break;
    case OVERTIME:
        typeStr = "站内停留时间超过规定时长，需补超时费。";
        break;
    case OVER_TRIP:
        typeStr = "旅程费用超过token票支付范围。";
        break;
    case OVER_TIME_TRIP:
        typeStr = "旅程费用超过token票支付范围，且站内停留时间超过规定时长。";
        break;
    default:
        break;
    }
    return typeStr;

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

QString DataCenter::getReaderErrorStr(BYTE errorCode)
{
    return m_readerErrCodeMap.value(errorCode);
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

void DataCenter::afcHeart()
{
    // AFC 心跳
    BYTE afcHeart = TcpLinkTest();
    if (afcHeart == 0) {
        m_hrtCnt[AFC_HRT] = 0;
    }
}

void DataCenter::deviceState2afc()
{
    // 接收储值票 | 接收单程票 | 已登录 | 双向模式 | 出站 | 测试 | 停止服务 | 开
    BYTE status = 0x31;
    BYTE event = NULL;
    BYTE ret = DeviceState(status, &event);

    logger()->info("设备状态上报{}，status=%2, event=%3", ret, status, event);
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
}

// 车票基本信息格式化
QList<QTableWidgetItem *> DataCenter::getTicketItems(TicketBasicInfo *info)
{
    QList<QTableWidgetItem*> itemList;

    QTableWidgetItem* item1 = new QTableWidgetItem(info->type());
    QTableWidgetItem* item2 = new QTableWidgetItem(info->number());
    QTableWidgetItem* item3 = new QTableWidgetItem(info->createTime().toString("yyyy-MM-dd"));
    QTableWidgetItem* item4 = new QTableWidgetItem(info->validDate().toString("yyyy-MM-dd"));

    // TODO:状态文字
    QTableWidgetItem* item5 = new QTableWidgetItem(QString("%1").arg(info->cardState()));
    QTableWidgetItem* item6 = new QTableWidgetItem(QString("%1").arg(info->tripState()));

    QTableWidgetItem* item7 = new QTableWidgetItem(QString("%1").arg(info->balance(), 0, 'f', 2));

    itemList.append(item1);
    itemList.append(item2);
    itemList.append(item3);
    itemList.append(item4);
    itemList.append(item5);
    itemList.append(item6);
    itemList.append(item7);

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


