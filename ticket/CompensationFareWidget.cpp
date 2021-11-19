#include "CompensationFareWidget.h"
#include "ui_CompensationFareWidget.h"
#include "BIM2020.h"
#include "CommonHead.h"
#include "MyHelper.h"
#include "AmountCheckTimer.h"

CompensationFareWidget::CompensationFareWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompensationFareWidget)
{
    ui->setupUi(this);
    init();
}

CompensationFareWidget::~CompensationFareWidget()
{
    if (m_timer != NULL) {
        m_timer->stopTimer();
        delete m_timer;
        m_timer = NULL;
    }
    delete ui;
}


void CompensationFareWidget::initShow(int difference, uchar devState)
{
    if (difference <= 0) {
        logger()->info("投币金额为%1，无需投币。", difference);
        emit supplementaryOk(true);
        return;
    }

    // 数值转换
    m_difference = difference;
    m_isBanknotesOn = devState & 0x02;
    m_isSpecieOn = devState & 0x01;

    // 页面显示控制
    ui->endBtn->setText("结束投币");
    ui->continueBtn->setText("开始投币");
    ui->infoLabel->setText(QString("请投币%1元。").arg(m_difference));
    ui->listWidget->clear();
    deviceStateShow();

    // TODO:
//    if (devState == 0x00) {
//        MyHelper::ShowMessageBoxError("硬件设备故障，无法投币，请联系工作人员。");
//        supplementaryOk(false);
//        close();
//        return;
//    }

    ui->continueBtn->setDisabled(false);
    ui->endBtn->setDisabled(true);
    ui->returnCashBtn->setDisabled(true);

    show();
}



// 开始投币
void CompensationFareWidget::startPaying()
{
    int ret = StartPutCoin(m_difference);
    if (ret == 3) {
        logger()->error("[StartPutCoin]开始投币={%1}", ret);
        MyHelper::ShowMessageBoxError("投币模块故障，暂时无法使用，请联系工作人员解决。");
        emit supplementaryOk(false);
        close();
        return;
    } else if (ret == 1) {
        QString amountStr = ui->infoLabel->text();
        ui->infoLabel->setText(amountStr + "当前只支持投入硬币");
    } else if (ret == 2) {
        QString amountStr = ui->infoLabel->text();
        ui->infoLabel->setText(amountStr + "当前只支持投入纸币");
    }

    ui->listWidget->insertItem(0, "请开始投币。");
    m_timer->startTimer(350);
    m_payingState = 0;
    ui->continueBtn->setDisabled(true);
    ui->endBtn->setDisabled(false);
}


// 继续投币
void CompensationFareWidget::continuePaying()
{
    m_timer->resumeTimer();
}


// 结束投币
void CompensationFareWidget::onStopPaying()
{
    if (m_payingState == 0) {
        m_payingState = 1;
    }
    // 暂停检测
    m_timer->pauseTimer();

    // 结束硬件投币
    int banknotes = 0;
    int coins = 0;
    int ret = StopPutCoin(&banknotes, &coins);
//    ui->listWidget->insertItem(0, "");

    onAmountConfirm(banknotes, coins);
}

void CompensationFareWidget::onAmountConfirm(int banknotes, int coins)
{
    m_timer->stopTimer();
    ui->listWidget->insertItem(0, "结束投币，请确认投币金额。");

    // 计算已投入金额
    int banknotesAmount = banknotes > 0 ? banknotes : 0;
    int coinsAmount = coins > 0 ? coins : 0;
    int amount = banknotesAmount + coinsAmount;

    // 金额不足提醒继续投币or退币放弃，金额超出则找零
    if (amount < m_difference) {
        QString info = QString("已投币金额%1元，不足以支付票卡更新费用（%2元），请继续投币或手动退币放弃更新。")
                .arg(amount).arg(m_difference);
        MyHelper::ShowMessageBoxInfo(info);
        ui->continueBtn->setText("继续投币");
        ui->continueBtn->setDisabled(false);
        ui->endBtn->setDisabled(true);
        ui->returnCashBtn->setDisabled(false);
        return;
    }

    m_payingState = 2;

    // 无需继续投币
    ui->continueBtn->setText(" 开始投币");
    ui->continueBtn->setDisabled(true);
    ui->endBtn->setDisabled(true);
    ui->returnCashBtn->setDisabled(false);

    // 金额提示及找零
    int changeAmount = amount > m_difference ? (amount - m_difference) : 0;
    QString info = QString("已投入纸币%1元，投入硬币%2元，无需找零，请确认！如有疑问，可手动退币或找人工客服解决。").arg(banknotes).arg(coins);
    if (changeAmount > 0) {
        info = QString("已投入纸币%1元，投入硬币%2元，应找零%3元，请确认！如有疑问，可手动退币或找人工客服解决。").arg(banknotes).arg(coins).arg(changeAmount);
    }
    int ret = MyHelper::ShowMessageBoxQuesion(info);
    if (ret != 0) {
        return;
    }

    // 找零
    if (changeAmount > 0) {
        int billMoney = 0;
        int coinMoney = 0;
        int changeRet = ChanceCoin(changeAmount, &billMoney, &coinMoney);
        if (changeRet != 0) {
            MyHelper::ShowMessageBoxError("钱箱余额不足，无法找零，请保留找零凭证，联系工作人员找零或手动退币。");
            return;
        }
    }

    // 钱箱收钱
    ResultOperate(1);
    emit supplementaryOk(true);
    close();
}


// 退币
void CompensationFareWidget::onReturnMoney()
{
    int ret = ResultOperate(0);
    if (ret != 0) {
        MyHelper::ShowMessageBoxError(QString("退币失败{%1}，请联系工作人员处理。").arg(ret));
    }

    emit supplementaryOk(false);
    close();
}


void CompensationFareWidget::init()
{
    setStyle();

    // 设置非互斥
    QButtonGroup* group = new QButtonGroup(this);
    group->setExclusive(false);
    group->addButton(ui->bimRadioBtn);
    group->addButton(ui->brcRadioBtn);
    group->addButton(ui->f53RadioBtn);

    ui->bimRadioBtn->setDisabled(true);
    ui->brcRadioBtn->setDisabled(true);
    ui->f53RadioBtn->setDisabled(true);

    connect(ui->continueBtn, &QPushButton::clicked, this, &CompensationFareWidget::activePaying);
    connect(ui->endBtn, &QPushButton::clicked, this, &CompensationFareWidget::onStopPaying);
    connect(ui->returnCashBtn, &QPushButton::clicked, this, &CompensationFareWidget::onReturnMoney);

    m_payingState = 2;
    m_timer = new AmountCheckTimer();
    connect(m_timer, &AmountCheckTimer::receiveOk, this, &CompensationFareWidget::stopPaying);
    connect(m_timer, &AmountCheckTimer::timeoutReceive, this, &CompensationFareWidget::stopPaying);
}


void CompensationFareWidget::deviceStateShow()
{
    ui->bimRadioBtn->setChecked(m_isBanknotesOn);
    ui->brcRadioBtn->setChecked(m_isSpecieOn);
}

void CompensationFareWidget::showInfo(QString info)
{
    ui->listWidget->insertItem(0, info);
}


void CompensationFareWidget::setStyle()
{
    // 设置窗体关闭时自动释放内存
    this->setAttribute(Qt::WA_DeleteOnClose);
    // 透明背景设置
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QScreen *screen = QGuiApplication::primaryScreen();   //获取当前屏幕的大小
    QRect mm = screen->availableGeometry();
    int screenWidth = mm.width();
    int screenHeight = mm.height();
    this->setFixedSize(screenWidth, screenHeight);
}

void CompensationFareWidget::activePaying()
{
    if (m_payingState == 1) {
        continuePaying();
    } else {
        startPaying();
    }
}




