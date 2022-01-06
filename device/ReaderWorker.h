#ifndef READERWORKER_H
#define READERWORKER_H

#include <QObject>
#include "logger.h"
#include "propertyconfigurator.h"

class ReaderWorker : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit ReaderWorker(QObject *parent = nullptr);

    void onResetDevice();
    void onReading(bool onReading, int type);  // 票卡信息读取设置
    void onDeviceUpdate();

signals:
    void ticketRead(int ret);  // 票卡信息读取结果反馈

protected:
    void timerEvent(QTimerEvent* event);

private:
    void initReader();    // 设备初始化
    void hearChecking();  // 心跳检测

    void ticketReading();    // 票卡信息读取
    void readTransactionInfo();     // 读取交易信息
    void readReregisterInfo();      // 读取票卡更新信息
    int readBasicInfo();           // 票卡基本信息读取

    uchar readTicketInfo(uchar anti);    // 票卡信息读取
    uchar readHistoryTrade(uchar anti);   // 卡片历史交易信息

    void setTestData();
    void setTestData1();

    // 信息读取
    void readSamInfo();          // 读取SAM卡信息
    void readReaderVersion();    // 读写器版本信息获取
    void readerSoftUpdate();    // 读写器程序升级
    void readerParamUpdate();   // 读写器参数升级

private:
    // 票卡信息读取
    bool m_onReading;      // 读卡状态开启
    int m_ticketInfoType;  // 读取票卡内容： 0-历史交易信息  1-票卡更新信息
    long m_readStartTime;  // 投币时间控制

    int m_testFlag;

    // 定时器id
    int m_hearTimerId;        // 设备心跳定时器
    int m_readingTimerId;     // 票卡读取定时器
};

#endif // READERWORKER_H
