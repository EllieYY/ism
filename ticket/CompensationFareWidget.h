#ifndef COMPENSATIONFAREWIDGET_H
#define COMPENSATIONFAREWIDGET_H

#include <QWidget>
#include "logger.h"
#include "propertyconfigurator.h"

namespace Ui {
class CompensationFareWidget;
}

class AmountCheckTimer;
class CompensationFareWidget : public QWidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit CompensationFareWidget(QWidget *parent = nullptr);
    ~CompensationFareWidget();

    void initShow(int difference, uchar devState);
    void onAutoStopPaying(int bankNoteCount, int coinCount);      // 自动停止投币-检测到金额足够
    void onStopPaying();
    void showCheckState(int state, int bankNoteCount, int coinCount);

signals:
    void change(int amount);
    void supplementaryOk(bool);     // 补缴结束
    void startChecking(bool);

private:
    void init();
    void setStyle();

    void activePaying();                            // 启用投币
    void startPaying();                             // 开始投币
    void continuePaying();                          // 继续投币
    void amountCheck();                             // 投币金额检查
//    void onAutoStopPaying(int bankNoteCount, int coinCount);      // 自动停止投币-检测到金额足够
//    void onStopPaying();                            // 手动停止投币
    void onReturnMoney();                           // 退币
    void onAmountConfirm(int banknotes, int coins); // 投币金额确认

    void onMoneyIn();    // 强制钱进钱箱

    void deviceStateShow();                         // 设备在线状态显示
    void showInfo(QString info);                    // 操作信息显示


private:
    /* 操作参数 */
    int        m_difference;            // 需补足的差额
    int        m_income;                // 投币金额

    bool  m_isNeedReturn;

    /* 设备状态 */
    bool       m_isBanknotesOn;         // 纸币模块状态
    bool       m_isSpecieOn;            // 硬币模块状态

    int        m_payingState;           // 0-开始投币   1-继续投币   2-停止投币

    AmountCheckTimer*  m_timer;

private:
    Ui::CompensationFareWidget *ui;
};

#endif // COMPENSATIONFAREWIDGET_H
