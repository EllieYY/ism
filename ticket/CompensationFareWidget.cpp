#include "CompensationFareWidget.h"
#include "ui_CompensationFareWidget.h"
#include "BIM2020.h"
#include "CommonHead.h"
#include "MyHelper.h"
#include "DataCenter.h"
#include <QDateTime>
#include "DeviceManager.h"
#include "TicketBasicInfo.h"

CompensationFareWidget::CompensationFareWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompensationFareWidget)
{
    ui->setupUi(this);
    init();
}

CompensationFareWidget::~CompensationFareWidget()
{

    delete ui;
}


void CompensationFareWidget::initShow(long difference, uchar devState)
{
    emit stopReading(false, 1);
    ui->listWidget->clear();

    //TODO: test code
//    qDebug() << "现金补缴费用：" << difference;
//    emit supplementaryOk(true);
//    emit stopReading(true, 1);
//    close();
//    return;

    long ret = DataCenter::getThis()->getCashboxInitRet();
    QString info1 = QString("投币金额%1元，设备初始化返回%2，设备状态%3")
            .arg(difference).arg(QString::number(ret, 16)).arg(QString::number(devState, 16));
    showInfo(info1);

    if (difference <= 0) {
        MyHelper::ShowMessageBoxInfo("无需投币");
        logger()->info("投币金额为%1，无需投币。", difference);
        emit supplementaryOk(true);

        emit stopReading(true, 1);
        close();
        return;
    }

    // 数值初始化
    setNeedReturn(false);
    m_difference = difference;
    m_income = 0;
    m_inCoins = 0;
    m_inBanknotes = 0;
    m_payingState = -1;    // 初始投币状态

    // 数值转换
    m_isBanknotesOn = devState & 0x02;
    m_isSpecieOn = devState & 0x01;

    QString info2 = QString("m_isBanknotesOn=%1, m_isSpecieOn=%2").arg(m_isBanknotesOn).arg(m_isSpecieOn);
    showInfo(info2);

    // TODO: 限制打开
    // 设备状态检查，限制功能使用
    if (devState == 0x00) {
        MyHelper::ShowMessageBoxError("硬件设备故障，无法投币，请联系工作人员。");
        logger()->error("硬件设备故障，无法投币，请联系工作人员。");
        supplementaryOk(false);
        close();
        return;
    }

//    int coinNum = Request_Cashbox_Counter();
    QString logInfo = QString("需投币%1元").arg(m_difference);
    logForCashbox(logInfo);

    deviceStateShow();
    ui->infoLabel->setText(QString("请投币%1元。").arg(m_difference));

    // 页面显示控制
    ui->endBtn->setText("结束投币");
    ui->continueBtn->setText("开始投币");
    ui->returnCashBtn->setText("关闭");

    ui->continueBtn->setDisabled(false);
    ui->endBtn->setDisabled(true);
    ui->returnCashBtn->setDisabled(false);

    show();
}

void CompensationFareWidget::activePaying()
{
    // 0-开始投币   1-继续投币   2-停止投币
    if (m_payingState == -1) {
        showInfo("请开始投币。");
        startPaying();
    } else {
        showInfo("请继续投币。");
        continuePaying();
    }

    ui->continueBtn->setText("继续投币");
    ui->continueBtn->setDisabled(true);
    ui->endBtn->setDisabled(false);
}

// 开始投币
void CompensationFareWidget::startPaying()
{
    int ret = StartPutCoin(m_difference);
    QString info3 = QString("StartPutCoin(%1)=%2").arg(m_difference).arg(ret);
    showInfo(info3);

    // TODO:
//    ret = 0;

    if (ret == 1) {
        QString amountStr = ui->infoLabel->text();
        ui->infoLabel->setText(amountStr + "当前只支持投入硬币");
    } else if (ret == 2) {
        QString amountStr = ui->infoLabel->text();
        ui->infoLabel->setText(amountStr + "当前只支持投入纸币");
    } else if (ret != 0) {
        logger()->error("[StartPutCoin]开始投币={%1}", ret);
        MyHelper::ShowMessageBoxError("投币模块故障，暂时无法使用，请联系工作人员解决。");
        DataCenter::getThis()->setCashboxState(-1, -1, 0, 0x03);
        logForCashbox(QString("阀门打开故障[%1]，投币结束。").arg(ret));
        emit supplementaryOk(false);
        emit stopReading(true, 1);
        close();
        return;
    }

    emit startChecking(true);
    m_payingState = 0;
}


// 继续投币
void CompensationFareWidget::continuePaying()
{
    m_payingState = 1;
    emit startChecking(true);
}

void CompensationFareWidget::onAutoStopPaying(int bankNoteCount, int coinCount)
{
    m_payingState = 2;
    ui->continueBtn->setDisabled(true);
    ui->endBtn->setDisabled(true);

    QString info3 = QString("onAutoStopPaying, 投入纸币%2，投入硬币%3").arg(bankNoteCount).arg(coinCount);
    logForCashbox(info3);
    showInfo(info3);

    m_inCoins = coinCount;
    m_inBanknotes = bankNoteCount;
    onAmountConfirm(bankNoteCount, coinCount);
}


// 结束投币
void CompensationFareWidget::onStopPaying()
{
    // 暂停检测
    emit startChecking(false);
    m_payingState = 2;

    // 结束硬件投币
    int banknotes = 0;
    int coins = 0;
    int ret = StopPutCoin(&banknotes, &coins);

    m_inCoins = coins;
    m_inBanknotes = banknotes;

    QString info3 = QString("StopPutCoin()=%1，投入纸币%2，投入硬币%3").arg(ret).arg(banknotes).arg(coins);
    logForCashbox(info3);
    showInfo(info3);

    if (m_income > 0 && (banknotes + coins) == 0) {
        showInfo("StopPutCoin接口返回的投币金额为0时，使用投币状态检查接口返回的投币金额。");
        onAmountConfirm(m_inBanknotes, m_inCoins);
    } else {
        onAmountConfirm(banknotes, coins);
    }
}

void CompensationFareWidget::onAmountConfirm(int banknotes, int coins)
{
    int amount1 = Request_money_in();   //查询接收到的币数
    int amount2 = Request_Hopper_Balance();   // 检查找找零器中的硬币余额
    int amount3 = Request_Cashbox_Counter();   //查询钱箱中的硬币数量
    QString amountInfo = QString("接收到的硬币数[Request_money_in]=%1，\n找零器中硬币余额[Request_Hopper_Balance]=%2,\n钱箱中硬币数量[Request_Cashbox_Counter]=%3")
            .arg(amount1).arg(amount2).arg(amount3);

    showInfo(amountInfo);
    showInfo("结束投币，请确认投币金额。");
    // 计算已投入金额
    int banknotesAmount = banknotes > 0 ? banknotes : 0;
    int coinsAmount = coins > 0 ? coins : 0;
    m_income = banknotesAmount + coinsAmount;
    logForCashbox(QString("投币总额：%1元，硬币%2元，纸币%3元").arg(m_income).arg(m_inCoins).arg(m_inBanknotes));

    QString info1 = QString("投入总金额：%1元").arg(m_income);
    showInfo(info1);

    if (m_income <= 0) {
        setNeedReturn(false);
    } else {
        setNeedReturn(true);
    }
    ui->returnCashBtn->setDisabled(false);

    // 金额不足提醒继续投币or退币放弃，金额超出则找零
    if (m_income < m_difference) {
        QString actionInfo = "请继续投币或关闭页面放弃更新";
        if (m_income > 0) {
            actionInfo = "请继续投币或手动退币放弃更新";
        }
        QString info = QString("已投币金额%1元，不足以支付票卡更新费用（%2元），%3。")
                .arg(m_income).arg(m_difference).arg(actionInfo);
        MyHelper::ShowMessageBoxInfo(info);
        ui->continueBtn->setText("继续投币");
        ui->continueBtn->setDisabled(false);
        ui->endBtn->setDisabled(true);

        return;
    }

    // 无需继续投币
    ui->continueBtn->setText("开始投币");
    ui->continueBtn->setDisabled(true);
    ui->endBtn->setDisabled(true);

    // 投币金额及找零金额，用户确认
    int changeAmount = m_income > m_difference ? (m_income - m_difference) : 0;
    QString info = QString("已投入纸币%1元，投入硬币%2元，无需找零，点击确认按钮完成投币！").arg(banknotes).arg(coins);
    if (changeAmount > 0) {
        info = QString("已投入纸币%1元，投入硬币%2元，应找零%3元，点击确认按钮开始找零！\n对金额有疑问，请手动退币或联系工作人员。")
                .arg(banknotes).arg(coins).arg(changeAmount);
    }
    showInfo(info);
    int ret = MyHelper::ShowMessageBoxQuesion(info);
    if (ret != 1) {
        qDebug() << "用户对金额有疑问，";
        return;
    }

    // 找零
    QString inCashboxInfo = QString("已投入金额%1元，纸币%2元，硬币%3元。").arg(m_income).arg(m_inBanknotes).arg(m_inCoins);
    if (changeAmount > 0) {
        int billMoney = 0;
        int coinMoney = 0;
        showInfo("开始找零");
        int changeRet = ChanceCoin(changeAmount, &billMoney, &coinMoney);

//        // TODO:test code
//        changeRet = 0;
//        billMoney = 0;
//        coinMoney = 3;

        QString info2 = QString("钱箱找零：[找零金额]%1, [纸币]%2, [硬币]%3，[结果]%4")
                .arg(changeAmount).arg(billMoney).arg(coinMoney).arg(changeRet);
        showInfo(info2);
        logForCashbox(info2);

        // 一旦找零了，就不能退币
        if ((billMoney + coinMoney) != 0) {
            setNeedReturn(false);
        }

        if (changeRet != 0) {
            QString changeInfo = QString("钱箱余额不足，无法找零。用户投入金额%1元，已找零硬币%2元，纸币%3元。\n%4")
                    .arg(m_income).arg(coinMoney).arg(billMoney)
                    .arg("请保留此找零凭证，联系工作人员找零或手动退币。");

            showInfo(changeInfo);
            MyHelper::ShowMessageBoxError(changeInfo);
            return;
        }

        inCashboxInfo = QString("%1已找零金额%2元，找零硬币%3元，找零纸币%4元").arg(inCashboxInfo)
                .arg(changeAmount).arg(coinMoney).arg(billMoney);
        int ret2 = MyHelper::ShowMessageBoxQuesion(QString("%1请确认。").arg(inCashboxInfo));
        if (ret2 != 1) {
    //        qDebug() << "钱进钱箱提问";
            return;
        }
    }

    emit sigCashboxIn();
    showInfo("钱进钱箱");
    logForCashbox(QString("钱进钱箱"));

//    // 钱箱收钱
//    int retR = ResultOperate(1);
//    QString info3 = QString("ResultOperate(1) = %1").arg(retR);
//    showInfo(info3);

//    if (retR == 0) {
//        logForCashbox(QString("钱进钱箱成功，%1").arg(inCashboxInfo));
//    } else {
//        DataCenter::getThis()->setCashboxState(0, -1, 0, 0x03);    // 默认是纸币故障
//        logForCashbox(QString("钱进钱箱失败。%1").arg(inCashboxInfo));
//    }

    setNeedReturn(false);

    emit supplementaryOk(true);
    emit stopReading(true, 1);
    close();
}


// 关闭 | 退币：手动点此按钮时，都视为现金支付失败
void CompensationFareWidget::onReturnMoney()
{
    if (m_isNeedReturn) {
        int ret = ResultOperate(0);

        if (ret != 0) {
            DataCenter::getThis()->setCashboxState(-1, -1, 0, 0x03);
            MyHelper::ShowMessageBoxError(QString("退币失败{%1}，用户已投入%2元请联系工作人员处理。").arg(ret).arg(m_income));

            QString info = QString("退币失败{%1}，用户已投入%2元").arg(ret).arg(m_income);
            logger()->error(info);
            logForCashbox(info);

            //TDOO:控制可以退出页面
//            return;
        }

        setNeedReturn(false);

        logForCashbox("退币成功。");
    }

    emit supplementaryOk(false);
    emit stopReading(true, 1);
    close();
}


void CompensationFareWidget::init()
{
    setNeedReturn(false);
    setStyle();

    // 设置非互斥
    QButtonGroup* group = new QButtonGroup(this);
    group->setExclusive(false);
    group->addButton(ui->bimRadioBtn);
    group->addButton(ui->brcRadioBtn);
//    group->addButton(ui->f53RadioBtn);

    ui->bimRadioBtn->setDisabled(true);
    ui->brcRadioBtn->setDisabled(true);
//    ui->f53RadioBtn->setDisabled(true);

    connect(ui->continueBtn, &QPushButton::clicked, this, &CompensationFareWidget::activePaying);
    connect(ui->endBtn, &QPushButton::clicked, this, &CompensationFareWidget::onStopPaying);
    connect(ui->returnCashBtn, &QPushButton::clicked, this, &CompensationFareWidget::onReturnMoney);

    m_payingState = 2;
}


void CompensationFareWidget::deviceStateShow()
{
    ui->bimRadioBtn->setChecked(m_isBanknotesOn);
    ui->brcRadioBtn->setChecked(m_isSpecieOn);
}

void CompensationFareWidget::showInfo(QString info)
{
    QString timeStr = QString::number(QDateTime::currentMSecsSinceEpoch());
    ui->listWidget->insertItem(0, timeStr + "  " +info);

    logger()->info(info);
}

void CompensationFareWidget::setNeedReturn(bool needReturn)
{
    m_isNeedReturn = needReturn;
    if (!m_isNeedReturn) {
        ui->returnCashBtn->setText("关闭");
    } else {
        ui->returnCashBtn->setText("退币");
    }

}

void CompensationFareWidget::logForCashbox(QString line)
{
    int amount2 = Request_Hopper_Balance();   // 检查找找零器中的硬币余额
    int amount3 = Request_Cashbox_Counter();   //查询钱箱中的硬币数量

    QDateTime curTime = QDateTime::currentDateTime();
    TicketBasicInfo* info = DataCenter::getThis()->getTicketBasicInfo();
    QString cardNo = info->number();
    QString prefixInfo = QString("%1  [卡号%2]%3, 硬币找零器余额%4元，硬币钱箱余额%5元")
            .arg(curTime.toString("yyyy-MM-dd HH:mm:ss"))
            .arg(cardNo)
            .arg(line)
            .arg(amount2).arg(amount3);

    QDate curDay = QDate::currentDate();
    QString fileName = QString("cashboxLog_%1.log").arg(curDay.toString("yyyyMMdd"));
    QString filePath = QDir::currentPath() + QDir::separator() + QString(CASHBOX_FILE_PATH) +
            QDir::separator() + fileName;

    logger()->info("[%1]%2", filePath, prefixInfo);
    QFile file(filePath);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text)) {

        logger()->info("%1日志文件打开失败", filePath);
//        logger()->info("[%1]%2", filePath, prefixInfo);
        return;
    }

    QTextStream in(&file);
    in << prefixInfo << "\n";
    file.close();
}


void CompensationFareWidget::showCheckState(int state, int bankNoteCount, int coinCount)
{
    // 计算已投入金额
    int banknotesAmount = bankNoteCount > 0 ? bankNoteCount : 0;
    int coinsAmount = coinCount > 0 ? coinCount : 0;

    m_inCoins = coinsAmount;
    m_inBanknotes = banknotesAmount;
    m_income = banknotesAmount + coinsAmount;

    QString info = QString("check state ret = %1, bankNoteCount = %2, coinCount = %3")
            .arg(state)
            .arg(bankNoteCount)
            .arg(coinCount);
//    showInfo(info);
}


void CompensationFareWidget::setStyle()
{
    // 设置窗体关闭时自动释放内存
//    this->setAttribute(Qt::WA_DeleteOnClose);
    // 透明背景设置
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
    QScreen *screen = QGuiApplication::primaryScreen();   //获取当前屏幕的大小
    QRect mm = screen->availableGeometry();
    int screenWidth = mm.width();
    int screenHeight = mm.height();
    this->setFixedSize(screenWidth, screenHeight);
}






