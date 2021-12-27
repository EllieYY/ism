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
    void onDeviceUpdate();
    void onCheckingCashbox(bool isOn);      // 钱箱开始检测
    void onCashboxIn();                     // 钱进钱箱
    void startDeviceTimer();

    void setOnReading(bool onReading, int type);    // 票卡信息读取设置

    // 硬币器操作 - 全部退币
    void onReturnAllCoins();

    // 设备重新初始化
    void onCashboxReset(uchar device);      // 钱箱复位
    void onReaderReset();                   // 读写器复位 [bit0]:brc [bit1]:bim [bit2]:f53

signals:
    // 投币检测相关
    void receiveOk(int banknotes, int coins);    // 自动检测结束投币
    void timeoutChecking();                      // 投币检测超时
    void checkState(int, int, int);
    void ticketRead(int ret);             // 票卡信息读取结果反馈

    // 硬币维护相关
    void sigReturnCoinValid(int ret);    // 是否可以退币  0:可用， 其他：不可用
    void sigReturnCoinsState(bool isOk, int balance);   // isOk:全部退币操作是否完成  balance:找零器中余额

protected:
    void timerEvent(QTimerEvent* event);

private:
    void cashboxChecking();
    void hearChecking();

    void returnChecking();     // 退币检测

    void ticketReading();    // 票卡信息读取
    void readTransactionInfo();     // 读取交易信息
    void readReregisterInfo();      // 读取票卡更新信息
    int readBasicInfo();           // 票卡基本信息读取

    uchar readTicketInfo(uchar anti);    // 票卡信息读取
    uchar readHistoryTrade(uchar anti);   // 卡片历史交易信息

    void setTestData();

private:
    void initReader();    // 读写器初始化
    void initCashbox();   // 钱箱初始化
    void readSamInfo();          // 读取SAM卡信息
    void readReaderVersion();    // 读写器版本信息获取
    void readerSoftUpdate();    // 读写器程序升级
    void readerParamUpdate();   // 读写器参数升级

private:
    // 设备状态 0 - 不可用   1 - 初始化成功  2 - 在线
    int m_rwState;
    int m_coinState;
    int m_banknotes;
    int m_banknotesRe;

    // 现金支付相关
    bool m_onChecking;     // 投币检测开始标识位
    long m_startTime;      // 投币时间控制

    // 票卡信息读取
    bool m_onReading;      // 读卡状态开启
    int m_ticketInfoType;  // 读取票卡内容： 0-历史交易信息  1-票卡更新信息
    long m_readStartTime;      // 投币时间控制

    // 硬币退币
    bool m_onReturnCoins;   // 开始退币
    long m_startReturnTime;   // 退币时间控制

    // 定时器id
    int m_checkingTimerId;    // 投币检测定时器
    int m_hearTimerId;        // 设备心跳定时器
    int m_readingTimerId;     // 票卡读取定时器
    int m_returnTimerId;      // 退币定时器
};

#endif // DEVICEMANAGER_H
