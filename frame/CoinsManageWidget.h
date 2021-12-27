#ifndef COINSMANAGEWIDGET_H
#define COINSMANAGEWIDGET_H

#include <QWidget>
#include "logger.h"
#include "propertyconfigurator.h"

namespace Ui {
class CoinsManageWidget;
}

typedef enum {
    ADD_COIN = 1,      // 加币
    RETURN_COIN = 0    // 退币
} WORK_TYPE;


class CoinsManageWidget : public QWidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit CoinsManageWidget(QWidget *parent = nullptr);
    ~CoinsManageWidget();

    void initShow(int type, long amount);     // 操作金额：1-投币 0-退币
    void onAutoStopPaying(int amount);      // 自动停止-检测到金额足够
    void onStopPaying();                    // 手动停止
    void showCheckState(int state, int amount);

signals:
    void sigWorkOk(bool);         // 操作结束
    void sigWorStart(int,bool);       // 控制操作开始与结束

private:
    void activeWork();                            // 启用
    void startWork();                             // 开始
    void startAddCoin();   // 加币
    void stopAddCoin();

    void continueWork();                          // 继续
    void amountCheck();                           // 金额检查
    void onReturnMoney();                         // 退币 or 关闭

    void onAmountConfirm(int amount);     // 金额确认
    void addAmountConfirm(int amount);    // 加币金额确认
    void returnAmountConfirm(int amount);  // 已退币金额确认

    void showInfo(QString info);                    // 操作信息显示
    void setNeedReturn(bool);                       // 设置是否需要退币
    void logForCashbox(QString line);               // 钱箱日志

private:
    /* 操作参数 */
    int        m_type;                  // 操作类型  1-投币 0-退币
    int        m_difference;            // 需补足的差额
    int        m_amount;                // 操作金额

    bool       m_isNeedReturn;          // 是否需要退币
    int        m_payingState;           // -1:初始状态  0：开始投币  1：继续投币  2：停止投币

private:
    Ui::CoinsManageWidget *ui;
};

#endif // COINSMANAGEWIDGET_H
