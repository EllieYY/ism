#include "CashboxWorker.h"
#include "BIM2020.h"
#include "BasicInfo.h"
#include "DataCenter.h"

#define RETRY_COUNT 3
#define MIN_5 5*60
#define MIN_3 3*60

CashboxWorker::CashboxWorker(QObject *parent) : QObject(parent)
{
    m_coinState = -1;
    m_banknotes = -1;
    m_banknotesRe = -1;

    // 钱箱检测
    m_onChecking = false;
    m_startTime = 0;

    m_checkingTimerId = startTimer(400);
    m_hearTimerId = startTimer(60000);     // 每分钟检测心跳
}


void CashboxWorker::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_checkingTimerId) {    // 投币检测
        cashboxChecking();
    } else if (event->timerId() == m_hearTimerId) {  // 设备心跳
        hearChecking();
    }
}

void CashboxWorker::onCheckingCashbox(bool isOn)
{
    logger()->info("[onCheckingCashbox]开始投币检测:%1", isOn);
    m_onChecking = isOn;
}

void CashboxWorker::onCashboxIn()
{
    logger()->info("[onCashboxIn]:钱进钱箱");

    // 钱箱收钱
    int retR = ResultOperate(1);
    logger()->info("ResultOperate(1) = %1", retR);
}

void CashboxWorker::onInitDevice()
{
    initCashbox();
}

// 设备复位  device - [bit0]:brc [bit1]:bim [bit2]:f53
void CashboxWorker::onCashboxReset(uchar device)
{
    logger()->info("[onCashboxReset]设备复位 device=%1", device);
    BasicInfo* basicInfo = DataCenter::getThis()->getBasicInfo();

    if (device == 0x01) {    // 硬币器复位
        int portCoin = basicInfo->brcPort();

        logger()->info("[BRC_Connect]硬币串口连接");
        int ret1 = BRC_Connect(portCoin);
        if (ret1 != 0) {
            logger()->error("[onCashboxReset]串口连接失败，BRC_Connect(%2)={%1}", ret1, portCoin);
            return;
        }

        logger()->info("[BRC_Reset]硬币复位");
        int ret2 = BRC_Reset();
        if (ret2 != 0) {
            logger()->error("[onCashboxReset]硬币复位失败，BRC_Reset={%1}", ret2);
            return;
        }

        logger()->info("[Perform_Self_Check]硬币自检");
        int ret3 = Perform_Self_Check();
        if (ret3 != 0) {
            logger()->error("[onCashboxReset]硬币自检失败，Perform_Self_Check={%1}", ret3);
            return;
        }
        // 成功状态设置
        DataCenter::getThis()->setCashboxState(0, 0, 0, 0x01);

    } else if (device == 0x02) {   // 纸币器复位
        int portBanknotes = basicInfo->bimPort();

        logger()->info("[BIM_Connect]纸币串口连接");
        int ret1 = BIM_Connect(portBanknotes);
        if (ret1 != 0) {
            logger()->error("[onCashboxReset]纸币串口连接失败，BIM_Connect(%2)={%1}", ret1, portBanknotes);
            return;
        }

        logger()->info("[BIM_Reset]纸币复位");
        int ret2 = BIM_Reset();
        if (ret2 != 0) {
            logger()->error("[onCashboxReset]纸币复位失败，BIM_Reset={%1}", ret2);
            return;
        }

        logger()->info("[BIM_Initial]纸币初始化");
        int ret3 = BIM_Initial();
        if (ret3 != 0) {
            logger()->error("[onCashboxReset]纸币初始化失败，BIM_Initial={%1}", ret3);
            return;
        }

        // 成功状态设置
        DataCenter::getThis()->setCashboxState(0, 0, 0, 0x02);

    } else if (device == 0x04) {   // 纸币找零器复位
        int portBanknoteRe = basicInfo->f53Port();

        logger()->info("[F53Bill_Connect]找零器串口连接");
        int ret1 = F53Bill_Connect(portBanknoteRe);
        if (ret1 != 0) {
            logger()->error("[onCashboxReset]找零器串口连接失败，F53Bill_Connect(%2)={%1}", ret1, portBanknoteRe);
            return;
        }

        logger()->info("[F53Bill_Setting]找零器参数设置");
        int ret2 = F53Bill_Setting(5, 10);
        if (ret2 != 0) {
            logger()->error("[onCashboxReset]找零器参数设置失败，F53Bill_Setting(5, 10)={%1}", ret2);
            return;
        }

        // 成功状态设置
        DataCenter::getThis()->setCashboxState(0, 0, 0, 0x04);
    }
}

void CashboxWorker::hearChecking()
{
    // 钱箱检测的时候不检测心跳
    if (m_onChecking) {
        DataCenter::getThis()->cashboxOnline(true, true, true);
        return;
    }

    logger()->info("[hearChecking]-cashbox:start");

    // 钱箱心跳检测
    logger()->info("[hearChecking]-cashbox:[SimplePoll]硬币心跳");
    bool coin = (SimplePoll() == 0);

    logger()->info("[hearChecking]-cashbox:[BIM_Poll]纸币心跳");
    bool banknotes = (BIM_Poll() == 0);

    logger()->info("[hearChecking]-cashbox:[F53Poll]找零器心跳");
    bool banknotesRe = (F53Poll() == 0);

    // TODO:
//    coin = true;
//    banknotes = true;
//    banknotesRe = true;

    logger()->info("[hearChecking]-cashbox:end");

    DataCenter::getThis()->cashboxOnline(coin, banknotes, banknotesRe);
}

void CashboxWorker::cashboxChecking()
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

            //TODO: test code
            if (diff > 10) {
                m_onChecking = false;
                emit receiveOk(5, 5);
//                emit checkState(0, 5, 5);
                return;
            }

            if (diff > MIN_3) {
                m_onChecking = false;
                m_startTime = -1;
                emit timeoutChecking();
            }
        }
    } else {
        m_startTime = -1;
    }
}


// 0 - 初始化成功  2 - 在线
void CashboxWorker::initCashbox()
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



