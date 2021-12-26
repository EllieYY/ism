#ifndef LOGOUTDLG_H
#define LOGOUTDLG_H

#include <QWidget>
#include "logger.h"
#include "propertyconfigurator.h"

namespace Ui {
class LogoutDlg;
}

class CompensationFareWidget;
class LogoutDlg : public QWidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit LogoutDlg(QWidget *parent = nullptr);
    ~LogoutDlg();

    void setFareWidget(CompensationFareWidget *fareWidget);


signals:
    void logoutOk();

private:
    void initStyle();
    void onLogout();

    void onSupplementaryOk(bool isOk);

    void onAddCoins();        // 加币
    void onReturnAllCoins();   // 退出所有硬币

    // 显示控制
    void showBrcSet();
    void showF53Set();
    void hideSetFrame();

    // 设置按钮
    bool userInfoCheck();
    void brcSetOk();
    void f53SetOk();

    // 数量控制
    void brcAdd();
    void brcSub();
    void f1Add();
    void f1Sub();
    void f2Add();
    void f2Sub();
    void setBrcValue(int val);
    void setF1Value(int val);
    void setF2Value(int val);
    int getDenomination(int index);

    // 钱箱日志
    void logForCashbox(QString line);      // 钱箱日志

private:
    CompensationFareWidget* m_fareWidget;    // 投币窗口

private:
    Ui::LogoutDlg *ui;
};

#endif // LOGOUTDLG_H
