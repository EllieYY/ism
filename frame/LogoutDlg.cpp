#include "LogoutDlg.h"
#include "ui_LogoutDlg.h"
#include <QAction>
#include "DataCenter.h"
#include "LoginInfo.h"
#include "MyHelper.h"
#include "NCNetwork_Lib.h"
#include <QAbstractSlider>
#include "BIM2020.h"

LogoutDlg::LogoutDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogoutDlg)
{
    ui->setupUi(this);
    initStyle();

    connect(ui->logoutCloseBtn, &QPushButton::clicked, this, &LogoutDlg::close);
    connect(ui->logoutBtn, &QPushButton::clicked, this, &LogoutDlg::onLogout);

    // 显示控制按钮
    connect(ui->f53SetBtn, &QPushButton::clicked, this, &LogoutDlg::showF53Set);
    connect(ui->brcSetBtn, &QPushButton::clicked, this, &LogoutDlg::showBrcSet);
    connect(ui->hideBtn, &QPushButton::clicked, this, &LogoutDlg::hideSetFrame);

    // 信息确认
    connect(ui->f53OkBtn, &QPushButton::clicked, this, &LogoutDlg::f53SetOk);
    connect(ui->brcOkBtn, &QPushButton::clicked, this, &LogoutDlg::brcSetOk);

    // 数值设置
    connect(ui->horizontalSlider_0, &QAbstractSlider::valueChanged, this, &LogoutDlg::setBrcValue);
    connect(ui->horizontalSlider_1, &QAbstractSlider::valueChanged, this, &LogoutDlg::setF1Value);
    connect(ui->horizontalSlider_2, &QAbstractSlider::valueChanged, this, &LogoutDlg::setF2Value);

    connect(ui->add_0, &QPushButton::clicked, this, &LogoutDlg::brcAdd);
    connect(ui->add_1, &QPushButton::clicked, this, &LogoutDlg::f1Add);
    connect(ui->add_2, &QPushButton::clicked, this, &LogoutDlg::f2Add);
    connect(ui->sub_0, &QPushButton::clicked, this, &LogoutDlg::brcSub);
    connect(ui->sub_1, &QPushButton::clicked, this, &LogoutDlg::f1Sub);
    connect(ui->sub_2, &QPushButton::clicked, this, &LogoutDlg::f2Sub);


    // 默认填入用户名，且不让更改
    ui->userLineEdit->setDisabled(true);
    LoginInfo* info = DataCenter::getThis()->getLoginInfo();
    if (NULL != info) {
        QString userName = info->userName();
        if (!userName.isEmpty()) {
            ui->userLineEdit->setText(userName);
            ui->userLineEdit->setDisabled(true);
        }
    }

}

LogoutDlg::~LogoutDlg()
{
    delete ui;
}


void LogoutDlg::initStyle()
{
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint);
    //设置窗体关闭时自动释放内存
    this->setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_ShowModal, true);


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

    // 设置滑块数据范围
    ui->horizontalSlider_0->setRange(0, 500);
    ui->horizontalSlider_1->setRange(0, 200);
    ui->horizontalSlider_2->setRange(0, 200);

    ui->comboBox_1->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(1);

    ui->frame->hide();
    ui->hideBtn->hide();
}

void LogoutDlg::onLogout()
{
    QString password = ui->pwdLineEdit->text();
    QString userName = ui->userLineEdit->text();

    if (userName.isEmpty()) {
        MyHelper::ShowMessageBoxError("请输入用户名。");
        return;
    }
    if (password.isEmpty()) {
        MyHelper::ShowMessageBoxError("请输入密码。");
        return;
    }

    // 密码校验
    if (!DataCenter::getThis()->setLogoutData(userName, password)) {
        MyHelper::ShowMessageBoxError("用户名和密码错误，请核对后重新输入。");
        return;
    }

    // AFC 签退
    BYTE * operatorID = reinterpret_cast<byte*>(userName.toLocal8Bit().data());
    BYTE event = 0;          // 签退
    BYTE operatorType = 2;   // 维护人员
    BYTE ret = OperatorAction(operatorID, event, operatorType);
    if (ret != 0) {
//        MyHelper::ShowMessageBoxError(QString("签退失败[%1]，请联系工作人员。").arg(ret));
        logger()->error("[OperatorAction] 签退失败：%1", ret);
//        return;
    }


    emit logoutOk();
    close();
}

void LogoutDlg::showBrcSet()
{
    ui->frame->show();
    ui->groupBox->show();
    ui->groupBox_2->hide();
    ui->hideBtn->show();
}

void LogoutDlg::showF53Set()
{
    ui->frame->show();
    ui->groupBox->hide();
    ui->groupBox_2->show();
    ui->hideBtn->show();
}

void LogoutDlg::hideSetFrame()
{
    ui->frame->hide();
    ui->hideBtn->hide();
}

bool LogoutDlg::userInfoCheck()
{
    QString password = ui->pwdLineEdit->text();
    QString userName = ui->userLineEdit->text();

    if (password.isEmpty()) {
        MyHelper::ShowMessageBoxError("请输入密码。");
        return false;
    }

    // 用户校验
    if (!DataCenter::getThis()->localAuthentication(userName, password)) {
        MyHelper::ShowMessageBoxError("用户名登录信息错误，请核对后重新输入。");
        return false;
    }

    return true;
}

void LogoutDlg::brcSetOk()
{
    // 用户校验
    if (!userInfoCheck())
        return;

    // 获取数值
    int num = ui->unitLabel_0->text().toInt();
    int ret = Modify_Hopper_Balance(num);
    if (ret != 0) {
        MyHelper::ShowMessageBoxError("设置失败，请重试。");
        return;
    }

    // 参数数值生效，用户提醒
    ui->frame->hide();
    ui->hideBtn->hide();
}

void LogoutDlg::f53SetOk()
{
    // 用户校验
    userInfoCheck();

    // 获取数值
    int denomination1 = getDenomination(ui->comboBox_1->currentIndex());
    int denomination2 = getDenomination(ui->comboBox_2->currentIndex());
    int num1 = ui->unitLabel_1->text().toInt();
    int num2 = ui->unitLabel_2->text().toInt();

    int ret = F53Bill_Setting(denomination1, denomination2);
    if (ret != 0) {
        MyHelper::ShowMessageBoxError("设置失败，请重试。");
        return;
    }

    // 参数数值生效，用户提醒
    ui->frame->hide();
    ui->hideBtn->hide();
}

void LogoutDlg::brcAdd()
{
    int pos = ui->horizontalSlider_0->value();
    pos++;
    ui->horizontalSlider_0->setValue(pos);
    ui->unitLabel_0->setText(QString::number(pos));
}
void LogoutDlg::brcSub() {
    int pos = ui->horizontalSlider_0->value();
    pos--;
    if (pos <= 0) {
        return;
    }
    ui->horizontalSlider_0->setValue(pos);
    ui->unitLabel_0->setText(QString::number(pos));
}
void LogoutDlg::f1Add() {
    int pos = ui->horizontalSlider_1->value();
    pos++;
    ui->horizontalSlider_1->setValue(pos);
    ui->unitLabel_1->setText(QString::number(pos));
}
void LogoutDlg::f1Sub() {
    int pos = ui->horizontalSlider_1->value();
    pos--;
    if (pos <= 0) {
        return;
    }
    ui->horizontalSlider_1->setValue(pos);
    ui->unitLabel_1->setText(QString::number(pos));
}
void LogoutDlg::f2Add() {
    int pos = ui->horizontalSlider_2->value();
    pos++;
    ui->horizontalSlider_2->setValue(pos);
    ui->unitLabel_2->setText(QString::number(pos));
}
void LogoutDlg::f2Sub() {
    int pos = ui->horizontalSlider_2->value();
    pos--;
    if (pos <= 0) {
        return;
    }
    ui->horizontalSlider_2->setValue(pos);
    ui->unitLabel_2->setText(QString::number(pos));
}

void LogoutDlg::setBrcValue(int val) {
    ui->horizontalSlider_0->setValue(val);
    ui->unitLabel_0->setText(QString::number(val));
}
void LogoutDlg::setF1Value(int val) {
    ui->horizontalSlider_1->setValue(val);
    ui->unitLabel_1->setText(QString::number(val));
}
void LogoutDlg::setF2Value(int val) {
    ui->horizontalSlider_2->setValue(val);
    ui->unitLabel_2->setText(QString::number(val));
}

int LogoutDlg::getDenomination(int index)
{
    int denom = 0;
    switch(index) {
    case 0:
        denom = 5;
        break;
    case 1:
        denom = 10;
        break;
    default:
        denom = 0;
        break;
    }

    return denom;
}
