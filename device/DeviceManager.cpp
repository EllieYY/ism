#include "DeviceManager.h"
#include "NC_ReaderLib.h"
#include "BIM2020.h"
#include "NCNetwork_Lib.h"
#include "MyHelper.h"
#include "BasicInfo.h"
#include "DataCenter.h"

#define RETRY_COUNT 3
#define MIN_5 5*60

DeviceManager::DeviceManager(QObject *parent) : QObject(parent)
{
    // 初始状态：设备未初始化
    m_rwState = -1;
    m_coinState = -1;
    m_banknotes = -1;
    m_banknotesRe = -1;

    m_onChecking = false;
    m_startTime = 0;

    m_checkingTimerId = 0;
    m_hearTimerId = 0;
}

DeviceManager::~DeviceManager()
{
    qDebug() << "~DeviceManager()";
}


void DeviceManager::initDevice()
{
    qDebug() << "initDevice in " << QThread::currentThreadId();

    BasicInfo* basicInfo = DataCenter::getThis()->getBasicInfo();
    int bimPort = basicInfo->bimPort();
    int f53Port = basicInfo->f53Port();
    int brcPort = basicInfo->brcPort();

    initCashbox(bimPort, brcPort, f53Port);

    // 初始化读写器
    int readerPort = basicInfo->readerPort();
    QString deviceIdStr = DataCenter::getThis()->getDeviceId();
    initReader(readerPort, deviceIdStr);

}

void DeviceManager::onCheckingCashbox(bool isOn)
{
    qDebug() << "onCheckingCashbox:" << isOn;
    m_onChecking = isOn;
}

void DeviceManager::startDeviceTimer()
{
    m_checkingTimerId = startTimer(300);
    m_hearTimerId = startTimer(1000);
}


void DeviceManager::timerEvent(QTimerEvent *event)
{
//    qDebug() << "DeviceManager timer: " << QThread::currentThreadId() <<
//                ", onChecking=" << m_onChecking;
//    if (m_rwState == 0) {
//        // 读写器心跳
//    }
//    if (m_coinState == 0) {
//    }
//    if (m_banknotes == 0) {
//    }
//    if (m_banknotesRe == 0) {
//    }

    // 投币检测
    if (event->timerId() == m_checkingTimerId) {
        cashboxChecking();
    } else if (event->timerId() == m_hearTimerId) {
        hearChecking();
    }
}

void DeviceManager::cashboxChecking()
{
    if (m_onChecking) {
        qDebug() << "cashbox checking.....";
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

//            //TODO: test code
//            if (diff > 20) {
//                m_onChecking = false;
//                emit receiveOk(10, 1);
//                return;
//            }

            if (diff > MIN_5) {
                m_onChecking = false;
                emit timeoutChecking();
            }
        }
    } else {
        m_startTime = 0;
    }
}

void DeviceManager::hearChecking()
{
    qDebug() << "device heart checking.....";

    // 钱箱心跳检测
    bool coin = (SimplePoll() == 0);
    bool banknotes = (BIM_Poll() == 0);
    bool banknotesRe = (F53Poll() == 0);
    DataCenter::getThis()->cashboxOnline(coin, banknotes, banknotesRe);

    // TODO:读写器心跳检测

}

// 0 - 不可用   1 - 初始化成功
void DeviceManager::initReader(int port, QString deviceIdStr)
{
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
                m_rwState = 1;
                readReaderVersion();
                readSamInfo();
                break;
            }
        }
        logger()->info("[readerComOpen]读写器open={%1}, 端口号={%2}; [readerInit]={%3}",
                       ret, port, initRet);
    }

    DataCenter::getThis()->setReaderState(m_rwState);
}


// 0 - 不可用   1 - 初始化成功  2 - 在线
void DeviceManager::initCashbox(int portBanknotes, int portCoin, int portBanknoteRe)
{
    int tryTime = 0;
    while (tryTime++ < RETRY_COUNT) {
        // 端口：纸币、硬币、纸币找零器
        int retC = ConnectMachine(portBanknotes, portCoin, portBanknoteRe);
        m_banknotes = (retC & 0x000F);
        m_coinState = (retC & 0x00F0);
        m_banknotesRe = (retC & 0x0F00);

        logger()->info("[ConnectMachine]钱箱初始化={%1}, 端口号：billPort={%2}, coinPort={%3}, changePort={%4}, state={%5 %6 %7}",
                       retC, portBanknotes, portCoin, portBanknoteRe, m_coinState, m_banknotes, m_banknotesRe);

        DataCenter::getThis()->setCashboxInitRet(retC);

        // 初始化成功，不必重试
        if (retC == 0) {
            break;
        }
    }

    DataCenter::getThis()->setCashboxState(m_coinState, m_banknotes, m_banknotesRe);
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
