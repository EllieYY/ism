#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QWidget>
#include "logger.h"
#include "propertyconfigurator.h"

namespace Ui {
class LoginDlg;
}

class LoginDlg : public QWidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit LoginDlg(QWidget *parent = nullptr);
    ~LoginDlg();

    void initShow();

signals:
    void loginOk();     // 签到成功
    void sigExitISM();

private:
    void init();
    void initStyle();
    void login();       // 签到


private:
    Ui::LoginDlg *ui;
};

#endif // LOGINDLG_H
