#include "DeviceManager.h"
#include "NC_ReaderLib.h"
#include "BIM2020.h"
#include "NCNetwork_Lib.h"
#include "MyHelper.h"
#include "BasicInfo.h"
#include "DataCenter.h"
#include "TicketBasicInfo.h"
#include "TransactionInfo.h"
#include "SettingCenter.h"
#include "ReaderSoftFileInfo.h"
#include "BomParamVersionInfo.h"

#define RETRY_COUNT 3
#define MIN_5 5*60
#define MIN_1 60

DeviceManager::DeviceManager(QObject *parent) : QObject(parent)
{
    // 初始状态：设备未初始化
    m_rwState = -1;
    m_coinState = -1;
    m_banknotes = -1;
    m_banknotesRe = -1;

    // 钱箱检测
    m_onChecking = false;
    m_startTime = 0;

    // 票卡信息读取
    m_ticketInfoType = 0;   // 默认读取历史交易信息
    m_onReading = false;
    m_readStartTime = -1;

    // 定时器
    m_checkingTimerId = 0;
    m_hearTimerId = 0;
    m_readingTimerId = 0;
}

DeviceManager::~DeviceManager()
{
    qDebug() << "~DeviceManager()";
}


void DeviceManager::initDevice()
{
    qDebug() << "initDevice in " << QThread::currentThreadId();
    initCashbox();

    // 初始化读写器
    initReader();
}

// 设备更新
void DeviceManager::onDeviceUpdate()
{
    qDebug() << "onDeviceUpdate: " << QThread::currentThreadId();

    // 软件版本升级
    readerSoftUpdate();

    // 参数升级
    readerParamUpdate();
}

void DeviceManager::onCheckingCashbox(bool isOn)
{
    qDebug() << "onCheckingCashbox:" << isOn;
    m_onChecking = isOn;
}

void DeviceManager::onCashboxIn()
{
    logger()->info("thread:钱进钱箱");

    // 钱箱收钱
    int retR = ResultOperate(1);
    logger()->info("ResultOperate(1) = %1", retR);


    // 由设备心跳去检测设备故障
//    if (retR != 0) {
//        DataCenter::getThis()->setCashboxState(0, -1, 0, 0x03);    // 默认是纸币故障
//    }
}

void DeviceManager::startDeviceTimer()
{
    m_checkingTimerId = startTimer(300);
    m_hearTimerId = startTimer(5000);
    m_readingTimerId = startTimer(1000);
}


void DeviceManager::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_checkingTimerId) {    // 投币检测
        cashboxChecking();
    } else if (event->timerId() == m_readingTimerId) {   // 票卡读取
        ticketReading();
    } else if (event->timerId() == m_hearTimerId) {  // 设备心跳
        hearChecking();
    }
}

void DeviceManager::cashboxChecking()
{
    if (m_onChecking) {
        long currentTime = QDateTime::currentSecsSinceEpoch();
        if (m_startTime <= 0) {   // 初始化投币开始时间
            m_startTime = currentTime;
        }

        // 投币完成检测
        int bankNoteCount = 0;
        int coinCount = 0;
        int ret = CheckCoinStatus(&bankNoteCount, &coinCount);
        emit checkState(ret, bankNoteCount, coinCount);  // 检测状态检测，方便测试
        if (ret == 0) {
            m_onChecking = false;
            emit receiveOk(bankNoteCount, coinCount);
        } else {    // 检测超时：超时自动调用停止投币接口
            long diff = currentTime - m_startTime;

//            qDebug() << "overtime:" << diff;
//            //TODO: test code
//            if (diff > 10) {
//                m_onChecking = false;
//                emit receiveOk(5, 2);
////                emit checkState(0, 5, 5);
//                return;
//            }

            if (diff > MIN_5) {
                m_onChecking = false;
                m_startTime = -1;
                emit timeoutChecking();
            }
        }
    } else {
        m_startTime = -1;
    }
}

void DeviceManager::hearChecking()
{
    qDebug() << "[start]hearChecking......";
    // 钱箱心跳检测
    bool coin = (SimplePoll() == 0);
    bool banknotes = (BIM_Poll() == 0);
    bool banknotesRe = (F53Poll() == 0);

    // TODO:
//    coin = true;
//    banknotes = true;
//    banknotesRe = true;

    DataCenter::getThis()->cashboxOnline(coin, banknotes, banknotesRe);

    // 读写器心跳检测
    STATUS_INFO ReaderStatus = {0};
    BYTE ret = getStatus(&ReaderStatus);
    bool readerOn = (ret == 0);

    // TODO:
//    readerOn = true;

    DataCenter::getThis()->readerOnline(readerOn);
    qDebug() << "[start]hearChecking......";
}

void DeviceManager::ticketReading()
{
    if (!m_onReading) {
        return;
    }

    // 操作超时控制
    long currentTime = QDateTime::currentSecsSinceEpoch();
    if (m_readStartTime <= 0) {
        qDebug() << "m_readStartTime init";
        m_readStartTime = currentTime;
    }

    // 超时判断
    long diff = currentTime - m_readStartTime;

//    qDebug() << "cur=" << currentTime << ", m_readStartTime=" << m_readStartTime << ", diff=" << diff;
//    if (diff > MIN_1) {
    if (diff > 10) {
        m_readStartTime = -1;
        emit ticketRead(0x05);
        return;
    }

    // 读取卡信息
    if (m_ticketInfoType == 0) {
        readTransactionInfo();
    } else if (m_ticketInfoType == 1) {
        readReregisterInfo();
    }
}

void DeviceManager::readFinish(int ret)
{
    m_readStartTime = -1;
    emit ticketRead(ret);
}

void DeviceManager::readTransactionInfo()
{
    if (readBasicInfo() != 0) {
        return;
    }

    // 卡历史信息读取限制：排除token票
    int cardType = DataCenter::getThis()->getTicketBasicInfo()->icType();
    if (cardType != UL_CARD) {
        BYTE anti = DataCenter::getThis()->getAntiNo();
        int hisRet = readHistoryTrade(anti);
        if (hisRet != 0x00) {
            m_readStartTime = -1;
            emit ticketRead(hisRet);
            return;
        }
    }

    m_readStartTime = -1;
    qDebug() << "[readTransactionInfo]m_readStartTime = -1   #1";
    emit ticketRead(0);
}

void DeviceManager::readReregisterInfo()
{
    if (readBasicInfo() != 0) {
        return;
    }

    m_readStartTime = -1;
    qDebug() << "[readReregisterInfo]m_readStartTime = -1";
    emit ticketRead(0);
}

int DeviceManager::readBasicInfo()
{
    BYTE anti = DataCenter::getThis()->getAntiNo();

    // 票卡信息获取
    int ret = readTicketInfo(anti);

//    qDebug() << "[readTicketInfo]=" << ret;
//    // TODO: 使用测试数据
//    ret = 0x06;
//    setTestData();

    // 找不到卡的情况下继续读卡，其他错误直接提示
    if (ret == 0x05 || ret == 0x06) {
        qDebug() << "[readTicketInfo]=" << ret;
        return -1;
    } else if (ret != 0x00) {
        m_readStartTime = -1;
        qDebug() << "其他错误[readTicketInfo]=" << ret;
        emit ticketRead(ret);
        return -2;
    }

    return ret;
}

uchar DeviceManager::readTicketInfo(uchar anti)
{
    BYTE zone = DataCenter::getThis()->isPayZone() ? PAY : FREE;
    ANALYSECARD_RESP analyseInfo = {0};
    uchar ret = cardAnalyse(anti, zone, &analyseInfo);
    if (ret != 0) {
        return ret;
    }

//    // 结果打印
//    QByteArray resArr = QByteArray((char*)&analyseInfo, sizeof(ANALYSECARD_RESP));
//    QString resStr = resArr.toHex();
//    logger()->info("[cardAnalyse] %1", resStr);

    /* 字段解析 ------------*/
    // 卡类型 | 逻辑卡号 | 发卡时间 | 有效期 | 卡状态 | 旅程状态 | 余额（分）
    int typeNum = analyseInfo.ticketType;
    QString type = DataCenter::getThis()->getTicketTypeString(typeNum);
    QString number = QByteArray((char*)analyseInfo.logicID, 10).toHex().toUpper();
    QString startDate = QByteArray((char*)analyseInfo.issueStartDate, 4).toHex();
    QString validDate = QByteArray((char*)analyseInfo.issueOutDate, 4).toHex();

    int state = analyseInfo.ticketStatus;
    int tripState = analyseInfo.ticketStatus;   // TODO:旅程状态待定
    bool ok;
    long balance = QByteArray((char*)analyseInfo.balance, 4).toHex().toLong(&ok, 16);

    // 相同卡且状态相同，不做数据更新
    TicketBasicInfo* preTicket = DataCenter::getThis()->getTicketBasicInfo();
    if (preTicket != NULL && number.compare(preTicket->number()) == 0 &&
            preTicket->cardState() == state) {
        return ret;
    }

    // 允许更新 | 卡扣更新 | 更新类型 | 应收费用（分）
    bool isAllowUpdate = analyseInfo.isAllowUpdate;
    bool isAllowOctPay = analyseInfo.isAllowOctPay;
    int updateType = analyseInfo.UpdateType;
    uint amount = analyseInfo.UpdateAmount;   // 单位是分

    // 进站车站 | 进站时间 | 出站车站 | 出站时间
    QString enStation = QByteArray((char*)analyseInfo.lastEnrtyStation, 2).toHex().toUpper();
    QString exStation = QByteArray((char*)analyseInfo.lastExitStation, 2).toHex().toUpper();
    QString enTime = QByteArray((char*)analyseInfo.lastEntryTime, 7).toHex();
    QString exTime = QByteArray((char*)analyseInfo.lastExitTime, 7).toHex();

    // 原始字节结果打印
    QByteArray resArr = QByteArray((char*)&analyseInfo, sizeof(ANALYSECARD_RESP));
    QString resStr = resArr.toHex();
    logger()->info("[cardAnalyse] %1", resStr);

    TicketBasicInfo* ticket = new TicketBasicInfo(
                typeNum, type, number, startDate, validDate, state, tripState, balance);
    ticket->setIcType(analyseInfo.ICType);
    ticket->setErrorCode(analyseInfo.errCode);
    ticket->setIsAllowOctPay(isAllowOctPay);
    ticket->setIsAllowUpdate(isAllowUpdate);
    ticket->setUpdateType(updateType);
    ticket->setEnStationCode(enStation);
    ticket->setEnTime(enTime);
    ticket->setExStationCode(exStation);
    ticket->setExTime(exTime);
    ticket->setUpdateAmount(amount);

    DataCenter::getThis()->setTicketBasicInfo(ticket);

    return ret;
}

uchar DeviceManager::readHistoryTrade(uchar anti)
{
    HISTORY_RESP cardHistory = {0};
    uchar hisRet = readCardHistory(anti, &cardHistory);
    if (hisRet != 0x00) {
        return hisRet;
    }

    // 结果打印
    QByteArray resArr = QByteArray((char*)&cardHistory, sizeof(HISTORY_RESP));
    QString resStr = resArr.toHex();
//    logger()->info("[readCardHistory] %1", resStr);

    // 交易条数
    QList<TransactionInfo*> transList;
    int count = cardHistory.HistoryCount;
    for (int i = 0; i < count; i++) {
        CARD_HISTORY item = cardHistory.HistoryInfo[i];
        // 交易时间 | 交易金额（分） | 交易类型 | 终端SAM卡号
        bool ok;
        QString time = QByteArray((char*)item.TradeDate, 7).toHex();
        long amount = QByteArray((char*)item.TradeAmount, 4).toHex().toLong(&ok, 16);
        int type = item.TradeType;
        QString devCode = QByteArray((char*)item.PsamID, 6).toHex().toUpper();

        transList.append(new TransactionInfo(time, type, amount, devCode));
    }

    DataCenter::getThis()->setTransInfoList(transList);
    return hisRet;
}

// 0 - 初始化成功
void DeviceManager::initReader()
{
    BasicInfo* basicInfo = DataCenter::getThis()->getBasicInfo();
    int port = basicInfo->readerPort();
    QString deviceIdStr = DataCenter::getThis()->getDeviceId();

    QByteArray devByteArray = MyHelper::hexStrToByte(deviceIdStr);
    BYTE* deviceId = (BYTE*)devByteArray.data();

    int tryTime = 0;
    while (tryTime++ < RETRY_COUNT) {
        // 打开串口
        int initRet = -1;
        int ret = (ushort)readerComOpen(port);
        if (ret == 0) {
            // 设备初始化、自检
            BYTE producter = 0;
            initRet = readerInit(deviceId, producter);
            if (initRet == 0) {
                m_rwState = 0;
                DataCenter::getThis()->setReaderState(m_rwState);
                readReaderVersion();
                readSamInfo();
                break;
            }
        }
        logger()->info("[readerComOpen]读写器open={%1}, 端口号={%2}; [readerInit]={%3}",
                       ret, port, initRet);
    }
}


// 0 - 初始化成功  2 - 在线
void DeviceManager::initCashbox()
{
    BasicInfo* basicInfo = DataCenter::getThis()->getBasicInfo();
    int portBanknotes = basicInfo->bimPort();
    int portCoin = basicInfo->brcPort();
    int portBanknoteRe = basicInfo->f53Port();

    int tryTime = 0;
    while (tryTime++ < RETRY_COUNT) {
        // 端口：纸币、硬币、纸币找零器
        int retC = ConnectMachine(portBanknotes, portCoin, portBanknoteRe);
        m_banknotes = (retC & 0x000F);
        m_coinState = (retC & 0x00F0) >> 4;
        m_banknotesRe = (retC & 0x0F00) >> 8;

        logger()->info("[ConnectMachine]钱箱初始化={%1}, 端口号：billPort={%2}, coinPort={%3}, changePort={%4}, state={%5 %6 %7}",
                       retC, portBanknotes, portCoin, portBanknoteRe,
                       QString::number(m_coinState, 16),
                       QString::number(m_banknotes, 16),
                       QString::number(m_banknotesRe, 16));

        DataCenter::getThis()->setCashboxInitRet(retC);

        // 初始化成功，不必重试
        if (retC == 0) {
            break;
        }
    }

    DataCenter::getThis()->setCashboxState(m_coinState, m_banknotes, m_banknotesRe, 0x07);
}

void DeviceManager::readSamInfo()
{
    BYTE mtrSam[6] = {0};
    BYTE octSam[6] = {0};
    BYTE jtbSam[6] = {0};

    STATUS_INFO ReaderStatus = {0};
    BYTE ret = getStatus(&ReaderStatus);
    if (ret == 0) {
        int count = ReaderStatus.SAMCount;
        for (int i = 0; i < count; i++) {
            int type = ReaderStatus.SAMInfo[i].SAM_Type;
            BYTE* cardId = ReaderStatus.SAMInfo[i].SAMID;
            if (type == 0) {
                memcpy(mtrSam, ReaderStatus.SAMInfo[i].SAMID, 6);
            } else if (type == 1) {
                memcpy(octSam, ReaderStatus.SAMInfo[i].SAMID, 6);
            } else {
                memcpy(jtbSam, ReaderStatus.SAMInfo[i].SAMID, 6);
            }
        }
        DataCenter::getThis()->setSAMInfo(mtrSam, octSam, jtbSam);
    }

    QString mtrStr = QByteArray((char*)mtrSam, 6).toHex().toUpper();
    QString octStr = QByteArray((char*)octSam, 6).toHex().toUpper();
    QString jtbStr = QByteArray((char*)jtbSam, 6).toHex().toUpper();
    logger()->info("读写器获取SAM卡号信息[getStatus]={%1}, SAM卡号：地铁{%2}，洪城一卡通{%3}，交通部{%4}",
                   ret, mtrStr, octStr, jtbStr);
}

void DeviceManager::readReaderVersion()
{
    // 获取读写器版本
    QString versionStr = "00000000";
    VERSION_INFO version = {0};
    int retVersion = getVersion(&version);
    if (retVersion == 0) {
        QByteArray versionArray;
        versionArray.append((char*)version.SoftVersion, 2);
        versionStr = "0000" + versionArray.toHex().toUpper();
    }

    logger()->info("[getVersion]=%1, version=%2", retVersion, versionStr);
    DataCenter::getThis()->setReaderVersion(versionStr);
}

// 读写器程序升级 --- 升级完成之后会自动重启，
void DeviceManager::readerSoftUpdate()
{
    // 升级之前先限制读写器功能的使用
    DataCenter::getThis()->setReaderState(-1);

    ReaderSoftFileInfo* info = SettingCenter::getThis()->getReaderSoftInfo();
    // 无需升级
    if (info == NULL || !info->getFileReady() || info->getHasUpdated()) {
        DataCenter::getThis()->setReaderState(0);
        return;
    }

    QString fileName = info->getFileName();
    QString filePath = QDir::currentPath() + QDir::separator() + QString(SOFT_FILE_PATH) + QDir::separator() + fileName;

    QByteArray nameArray = fileName.toLatin1();
    char* nameStr = nameArray.data();
    QByteArray pathArray = filePath.toLatin1();
    char* pathStr = pathArray.data();

    int tryTime = 0;
    while (tryTime++ < RETRY_COUNT) {
        int ret = fileDownload(pathStr, nameStr);
        logger()->info("读写器程序升级[fileDownload]={%1}， filePath={%2}",
                       ret, filePath);
        if (ret == 0) {
            break;
        }
    }
}


// 读写器参数更新
void DeviceManager::readerParamUpdate()
{
    QString path = QDir::currentPath() + QDir::separator() + PARAM_FILE_PATH + QDir::separator();
    QList<BomParamVersionInfo*> list = SettingCenter::getThis()->getParamVersionInfo(path + "version.json");
    QList<BomParamVersionInfo*> newList;
    for (BomParamVersionInfo* info: list) {
        if (!info->readerUsed() || info->readerUpdated()) {
            newList.append(info);
            continue;
        }

        QString fileName = info->fileName();
        QString filePath = path + fileName;

        QByteArray nameArray = fileName.toLatin1();
        char* nameStr = nameArray.data();
        QByteArray pathArray = filePath.toLatin1();
        char* pathStr = pathArray.data();

        bool updated = false;
        // 参数文件写入 -- 重试3次
        int tryTime = 0;
        while (tryTime++ < RETRY_COUNT) {
            int ret = fileDownload(pathStr, nameStr);
            logger()->info("读写器参数文件升级[fileDownload]={%1}， filePath={%2}",
                           ret, filePath);
            if (ret == 0) {
                updated = true;
                break;
            }
        }

        info->setReaderUpdated(updated);
        newList.append(info);
    }

    // 版本信息存储
    SettingCenter::getThis()->saveParamVersionInfo(newList, "version.json");
}

void DeviceManager::setOnReading(bool onReading, int type)
{
//    qDebug() << "开始读卡: " << onReading;
    m_onReading = onReading;
    m_ticketInfoType = type;

    if (m_onReading) {
        m_readStartTime = -1;
        qDebug() << "[setOnReading]m_readStartTime = -1";
    }
}


// 设备复位  device - [bit0]:brc [bit1]:bim [bit2]:f53
void DeviceManager::onCashboxReset(uchar device)
{
    logger()->info("设备复位[onCashboxReset]");
    BasicInfo* basicInfo = DataCenter::getThis()->getBasicInfo();

    if (device == 0x01) {    // 硬币器复位
        int portCoin = basicInfo->brcPort();
        int ret1 = BRC_Connect(portCoin);
        if (ret1 != 0) {
            logger()->error("[onCashboxReset]设备复位失败，BRC_Connect(%2)={%1}", ret1, portCoin);
            return;
        }
        int ret2 = BRC_Reset();
        if (ret2 != 0) {
            logger()->error("[onCashboxReset]设备复位失败，BRC_Reset={%1}", ret2);
            return;
        }
        int ret3 = Perform_Self_Check();
        if (ret3 != 0) {
            logger()->error("[onCashboxReset]设备复位失败，Perform_Self_Check={%1}", ret3);
            return;
        }
        // 成功状态设置
        DataCenter::getThis()->setCashboxState(0, 0, 0, 0x01);

    } else if (device == 0x02) {   // 纸币器复位
        int portBanknotes = basicInfo->bimPort();
        int ret1 = BIM_Connect(portBanknotes);
        if (ret1 != 0) {
            logger()->error("[onCashboxReset]设备复位失败，BIM_Connect(%2)={%1}", ret1, portBanknotes);
            return;
        }
        int ret2 = BIM_Reset();
        if (ret2 != 0) {
            logger()->error("[onCashboxReset]设备复位失败，BIM_Reset={%1}", ret2);
            return;
        }
        int ret3 = BIM_Initial();
        if (ret3 != 0) {
            logger()->error("[onCashboxReset]设备复位失败，BIM_Initial={%1}", ret3);
            return;
        }

        // 成功状态设置
        DataCenter::getThis()->setCashboxState(0, 0, 0, 0x02);

    } else if (device == 0x04) {   // 纸币找零器复位
        int portBanknoteRe = basicInfo->f53Port();
        int ret1 = F53Bill_Connect(portBanknoteRe);
        if (ret1 != 0) {
            logger()->error("[onCashboxReset]设备复位失败，F53Bill_Connect(%2)={%1}", ret1, portBanknoteRe);
            return;
        }
        int ret2 = F53Bill_Setting(5, 10);
        if (ret2 != 0) {
            logger()->error("[onCashboxReset]设备复位失败，F53Bill_Setting(5, 10)={%1}", ret2);
            return;
        }

        // 成功状态设置
        DataCenter::getThis()->setCashboxState(0, 0, 0, 0x04);
    }
}

void DeviceManager::onReaderReset()
{
    logger()->info("设备复位[onReaderReset]");
    initReader();
}

void DeviceManager::setTestData()
{
    int typeNum = 0x80;
    QString type = DataCenter::getThis()->getTicketTypeString(typeNum);
    TicketBasicInfo* ticket = new TicketBasicInfo(
                0x85, type, "30010088562007", "20200901", "20231001", 1, 1, 500);
    ticket->setIsAllowOctPay(false);
    ticket->setIsAllowUpdate(true);
    ticket->setUpdateType(OVER_TIME);
    ticket->setEnStationCode("0203");
    ticket->setEnTime("20211115212305");
    ticket->setExStationCode("0306");
    ticket->setExTime("20211115212606");
    ticket->setUpdateAmount(700);
    ticket->setIcType(UL_CARD);
    ticket->setBalance(200);

    DataCenter::getThis()->setTicketBasicInfo(ticket);
}
