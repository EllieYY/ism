#ifndef LOGOUTDLG_H
#define LOGOUTDLG_H

#include <QWidget>
#include "logger.h"
#include "propertyconfigurator.h"

namespace Ui {
class LogoutDlg;
}

class LogoutDlg : public QWidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit LogoutDlg(QWidget *parent = nullptr);
    ~LogoutDlg();

signals:
    void logoutOk();

private:
    void initStyle();
    void onLogout();

    void showBrcSet();
    void showF53Set();
    void hideSetFrame();

    bool userInfoCheck();
    void brcSetOk();
    void f53SetOk();

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

private:
    Ui::LogoutDlg *ui;
};

#endif // LOGOUTDLG_H
