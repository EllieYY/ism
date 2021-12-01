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


private:
    void initReader(int port, QString deviceId);    // 读写器初始化
    void initCashbox(int portCoin, int portBanknotes, int portBanknoteRe);   // 钱箱初始化
    void samInfo2Acc();     // 向ACC上传SAM卡信息


private:



private:
    // 设备状态 0 - 不可用   1 - 初始化成功  2 - 在线
    int m_rwState;
    int m_coinState;
    int m_banknotes;
    int m_banknotesRe;

signals:
    void readerInitOk();
    // 设备状态信息：读写器、硬币模块、纸币模块、纸币找零模块
    void deviceState(int rwState, int coinState, int banknotes, int banknotesRe);

};

#endif // DEVICEMANAGER_H
