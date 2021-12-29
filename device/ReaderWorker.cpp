﻿#include "ReaderWorker.h"
#include "NC_ReaderLib.h"
#include "NCNetwork_Lib.h"
#include "MyHelper.h"
#include "BasicInfo.h"
#include "DataCenter.h"
#include "TicketBasicInfo.h"
#include "TransactionInfo.h"
#include "SettingCenter.h"
#include "ReaderSoftFileInfo.h"
#include "BomParamVersionInfo.h"

#include <QDebug>

#define RETRY_COUNT 3
#define MIN_5 5*60
#define MIN_1 60

ReaderWorker::ReaderWorker(QObject *parent) : QObject(parent)
{
    // 票卡信息读取
    m_ticketInfoType = 0;   // 默认读取历史交易信息
    m_onReading = false;
    m_readStartTime = -1;

    m_hearTimerId = startTimer(60000);     // 1分钟检测一次
    m_readingTimerId = startTimer(1000);   // 秒级读卡
}

void ReaderWorker::onResetDevice()
{
    initReader();
}

void ReaderWorker::onReading(bool onReading, int type)
{
    m_onReading = onReading;
    m_ticketInfoType = type;

    if (m_onReading) {
        m_readStartTime = -1;
    }
}

void ReaderWorker::timerEvent(QTimerEvent *event)
{
    qDebug() << "Thread心跳[ReaderManager]...";
    if (event->timerId() == m_readingTimerId) {   // 票卡读取
        ticketReading();
    } else if (event->timerId() == m_hearTimerId) {  // 设备心跳
        hearChecking();
    }
}

void ReaderWorker::initReader()
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
                DataCenter::getThis()->setReaderState(0);
                readReaderVersion();
                readSamInfo();
                break;
            }
        }
        logger()->info("[readerComOpen]读写器open={%1}, 端口号={%2}; [readerInit]={%3}",
                       ret, port, initRet);
    }
}

void ReaderWorker::hearChecking()
{
    qDebug() << "[hearChecking]-reader:start";

    // 读写器心跳检测
    STATUS_INFO ReaderStatus = {0};
    BYTE ret = getStatus(&ReaderStatus);

    qDebug() << "[hearChecking]-reader:end";

    bool readerOn = (ret == 0);
    // TODO:
//    readerOn = true;

    DataCenter::getThis()->readerOnline(readerOn);
}

void ReaderWorker::ticketReading()
{
    qDebug() << "ticketReading=======";
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

void ReaderWorker::readTransactionInfo()
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

void ReaderWorker::readReregisterInfo()
{
    if (readBasicInfo() != 0) {
        return;
    }

    m_readStartTime = -1;
    qDebug() << "[readReregisterInfo]m_readStartTime = -1";
    emit ticketRead(0);
}

int ReaderWorker::readBasicInfo()
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

uchar ReaderWorker::readTicketInfo(uchar anti)
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

uchar ReaderWorker::readHistoryTrade(uchar anti)
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

void ReaderWorker::readSamInfo()
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

void ReaderWorker::readReaderVersion()
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
void ReaderWorker::readerSoftUpdate()
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
void ReaderWorker::readerParamUpdate()
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