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

    static DeviceManager* getThis();		// 获取数据类
    static DeviceManager* m_pInstance;		// 数据类的指针

public:    // slots
    void initDevice();

    void onCheckingCashbox(bool isOn);      // 钱箱开始检测

    void onStartTimer();

protected:
    void timerEvent(QTimerEvent* event);


private:
    void initReader(int port, QString deviceId);    // 读写器初始化
    void initCashbox(int portBanknotes, int portCoin, int portBanknoteRe);   // 钱箱初始化
    void samInfo2Acc();     // 向ACC上传SAM卡信息


private:



private:
    // 设备状态 0 - 不可用   1 - 初始化成功  2 - 在线
    int m_rwState;
    int m_coinState;
    int m_banknotes;
    int m_banknotesRe;

    // 现金支付相关
    bool m_onChecking;     // 投币检测开始标识位
    long m_startTime;      // 投币时间控制

signals:
    void readerInitOk();

    // 设备状态信息：读写器、硬币模块、纸币模块、纸币找零模块
    // -1：未初始化，0：正常可用，其他值含义按接口定义来
    void readerState(int readerState);
    void cashboxState(int coinState, int banknotes, int banknotesRe);

    // 设备心跳
    void readerOnline();
    void cashboxOnline();

    // 投币检测相关
    void receiveOk(int banknotes, int coins);    // 自动检测结束投币
    void timeoutChecking();                      // 投币检测超时
    void checkState(int, int, int);

};

#endif // DEVICEMANAGER_H
