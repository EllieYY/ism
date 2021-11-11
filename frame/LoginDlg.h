#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QWidget>

namespace Ui {
class LoginDlg;
}

class LoginDlg : public QWidget
{
    Q_OBJECT

public:
    explicit LoginDlg(QWidget *parent = nullptr);
    ~LoginDlg();

    void initShow();

signals:
    void loginOk();     // 签到成功

private:
    void init();
    void initStyle();
    void login();       // 签到


private:
    Ui::LoginDlg *ui;
};

#endif // LOGINDLG_H
