#ifndef CASHBOXWORKER_H
#define CASHBOXWORKER_H

#include <QObject>
#include "logger.h"
#include "propertyconfigurator.h"

class CashboxWorker : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit CashboxWorker(QObject *parent = nullptr);

    void onCheckingCashbox(bool isOn);      // 钱箱开始检测
    void onCashboxReset(uchar device);      // 钱箱复位
    void onCashboxIn();                     // 钱进钱箱
    void onInitDevice();

signals:
    void receiveOk(int banknotes, int coins);    // 自动检测结束投币
    void timeoutChecking();                      // 投币检测超时
    void checkState(int, int, int);

protected:
    void timerEvent(QTimerEvent* event);

private:
    void initCashbox();   // 钱箱初始化
    void hearChecking();
    void cashboxChecking();

private:
    // 设备状态 0 - 不可用   1 - 初始化成功  2 - 在线
    int m_coinState;
    int m_banknotes;
    int m_banknotesRe;

    // 现金支付相关
    bool m_onChecking;     // 投币检测开始标识位
    long m_startTime;      // 投币时间控制

    // 定时器id
    int m_checkingTimerId;    // 投币检测定时器
    int m_hearTimerId;        // 设备心跳定时器
};

#endif // CASHBOXWORKER_H
