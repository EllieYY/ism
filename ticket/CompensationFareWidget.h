#ifndef COMPENSATIONFAREWIDGET_H
#define COMPENSATIONFAREWIDGET_H

#include <QWidget>
#include "logger.h"
#include "propertyconfigurator.h"

namespace Ui {
class CompensationFareWidget;
}

class CompensationFareWidget : public QWidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit CompensationFareWidget(QWidget *parent = nullptr);
    ~CompensationFareWidget();

    void secEvent();

    void getFare(double difference, uchar devState);
    void setAmout(double amout);

    void initShow();

signals:
    void stopPaying();
    void change(int amount);
    void supplementaryOk(bool);     // 补缴结束

private:
    void init();
    void initDevice();          // 设备初始化
    void setStyle();

    void deviceCheck();         // 投币检测
    void specieReturnCheck();   // 硬币退币检测

    bool deviceOpen();          // 投币闸门打开
    bool deviceClose();         // 投币闸门关闭

    void deviceStateShow();     // 设备在线状态显示
    int requestBanknotesIn();   // 查询接收到的纸币金额

    void onAmountConfirm();     // 投币金额确认
    void onReturnMoney();       // 手动退币

    void onStopPaying();        // 停止投币
    bool onChange(int amount);   // 找零

private:
    int        m_difference;            // 需补足的差额
    int        m_amount;                // 投币金额
    int        m_banknotesAmount;       // 收到的纸币金额
    int        m_specieAmount;          // 收到的硬币金额
    int        m_reSpecieAmount;        // 待退回的硬币金额
    int        m_initSpecieAmount;      // 找零开始时，硬币器中余额
    bool       m_startPaying;           // 开始投币
    bool       m_isSpecieReturn;        // 硬币找零 or退币
    int        m_sReturnRetryCnt;        // 硬币余额读取重试次数

    bool       m_successFlag;           // 操作成功标记

    // 阀门打开重试次数
    int        m_sDoorOpenCount;           // 硬币
    int        m_bDoorOpenCount;           // 纸币

    bool       m_isBanknotesOn;            // 纸币模块状态
    bool       m_isBanknotesReturnOn;      // 纸币找零模块状态
    bool       m_isSpecieOn;               // 硬币模块状态

private:
    Ui::CompensationFareWidget *ui;
};

#endif // COMPENSATIONFAREWIDGET_H
