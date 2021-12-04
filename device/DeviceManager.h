#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

/* 硬件设备的封装：读写器和钱箱
 * 通过moveToThread将阻塞操作放到子线程中去执行
 * 注意：放到线程中执行的操作，要通过信号槽的方式进行
 *
*/


#include <QObject>
#include "logger.h"
#include "propertyconfigurator.h"

class DeviceManager : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit DeviceManager(QObject *parent = nullptr);
    ~DeviceManager();

public:    // slots
    void initDevice();
    void onCheckingCashbox(bool isOn);      // 钱箱开始检测
    void startDeviceTimer();

signals:
    // 投币检测相关
    void receiveOk(int banknotes, int coins);    // 自动检测结束投币
    void timeoutChecking();                      // 投币检测超时
    void checkState(int, int, int);

protected:
    void timerEvent(QTimerEvent* event);
    void cashboxChecking();
    void hearChecking();

private:
    void initReader(int port, QString deviceId);    // 读写器初始化
    void initCashbox(int portBanknotes, int portCoin, int portBanknoteRe);   // 钱箱初始化
    void readSamInfo();          // 读取SAM卡信息
    void readReaderVersion();    // 读写器版本信息获取

private:
    // 设备状态 0 - 不可用   1 - 初始化成功  2 - 在线
    int m_rwState;
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

#endif // DEVICEMANAGER_H
