#include "LoginDlg.h"
#include "ui_LoginDlg.h"
#include <QScreen>
#include <QAction>
#include "MyHelper.h"
#include "DataCenter.h"
#include "NCNetwork_Lib.h"

LoginDlg::LoginDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginDlg)
{
    ui->setupUi(this);

    init();
}

LoginDlg::~LoginDlg()
{
    delete ui;
}

void LoginDlg::initShow()
{
    ui->userLineEdit->setText("");
    ui->pwdLineEdit->setText("");
    ui->loginBtn->setDisabled(false);

    // TODO:测试数据
//    ui->userLineEdit->setText("04326688");
//    ui->pwdLineEdit->setText("123456");
    show();
}


void LoginDlg::init()
{
    initStyle();

    connect(ui->loginBtn, &QPushButton::clicked, this, &LoginDlg::login);
}

void LoginDlg::initStyle()
{
    // 页面显示设置
    this->setWindowFlags(Qt::FramelessWindowHint);
//    this->setAttribute(Qt::WA_DeleteOnClose);
    QScreen *screen = QGuiApplication::primaryScreen();   //获取当前屏幕的大小
    QRect mm = screen->geometry();
    int screenWidth = mm.width();
    int screenHeight = mm.height();
    this->setFixedSize(screenWidth, screenHeight);

    QAction *pLeadindAction_user = new QAction(this);
    QImage userIcon("images/login/user.png");
    pLeadindAction_user->setIcon(QIcon(QPixmap::fromImage(userIcon)));
    ui->userLineEdit->addAction(pLeadindAction_user, QLineEdit::LeadingPosition);
    ui->userLineEdit->setPlaceholderText("请输入用户名");
    ui->userLineEdit->installEventFilter(this);
    ui->userLineEdit->setFocusPolicy(Qt::ClickFocus);

    QAction *pLeadindAction_pwd = new QAction(this);
    QImage pwdIcon("images/login/pwd.png");
    pLeadindAction_pwd->setIcon(QIcon(QPixmap::fromImage(pwdIcon)));
    ui->pwdLineEdit->addAction(pLeadindAction_pwd, QLineEdit::LeadingPosition);
    ui->pwdLineEdit->setPlaceholderText("请输入密码");
    ui->pwdLineEdit->installEventFilter(this);
    ui->pwdLineEdit->setFocusPolicy(Qt::ClickFocus);
}

void LoginDlg::login()
{
    // 记录签到信息
    QString userName = ui->userLineEdit->text();
    QString pwd = ui->pwdLineEdit->text();
    if (userName.isEmpty()) {
        MyHelper::ShowMessageBoxError("请输入用户名。");
        return;
    }
    if (pwd.isEmpty()) {
        MyHelper::ShowMessageBoxError("请输入密码。");
        return;
    }

    // 退出程序
    if (userName == "000000" && pwd == "1234qwer.") {
        emit sigExitISM();
        this->close();
        return;
    }


    // 密码校验
    ui->loginBtn->setDisabled(true);
    if (!DataCenter::getThis()->setLoginData(userName, pwd)) {
        MyHelper::ShowMessageBoxError("用户名和密码错误，请核对后重新输入。");
        ui->loginBtn->setDisabled(false);
        return;
    }

//    // 密码校验
//    bool isValidUser = DataCenter::getThis()->isValidUser(userName, pwd);
//    // 第三册 7.3.3.2.4操作员表
//    if (!isValidUser) {
//        MyHelper::ShowMessageBoxError(QString("用户名或密码错误，请核对后重新输入。"));
//        return;
//    }

//    // 登录
//    BYTE * operatorID = reinterpret_cast<byte*>(userName.toLocal8Bit().data());
//    BYTE event = 2;          // 带口令登录
//    BYTE operatorType = 2;   // 维护人员
//    BYTE ret = OperatorAction(operatorID, event, operatorType);
//    if (ret != 0) {
////        MyHelper::ShowMessageBoxError(QString("登录失败[%1]，请联系工作人员。").arg(ret));
//        logger()->error("[OperatorAction]AFC登录失败{%1}", ret);
////        return;
//    }

//    DataCenter::getThis()->setLoginData(userName, pwd);

    ui->loginBtn->setDisabled(false);
    this->hide();
    emit loginOk();
    this->close();
}
