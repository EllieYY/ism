#include "CoinsManageWidget.h"
#include "ui_CoinsManageWidget.h"
#include "CommonHead.h"
#include "MyHelper.h"
#include "DataCenter.h"
#include <QDateTime>
#include "DeviceManager.h"
#include "BIM2020.h"


CoinsManageWidget::CoinsManageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoinsManageWidget)
{
    ui->setupUi(this);
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

    setNeedReturn(false);
    m_type = ADD_COIN;

    connect(ui->continueBtn, &QPushButton::clicked, this, &CoinsManageWidget::activeWork);
    connect(ui->endBtn, &QPushButton::clicked, this, &CoinsManageWidget::onStopPaying);
    connect(ui->returnCashBtn, &QPushButton::clicked, this, &CoinsManageWidget::onReturnMoney);
}

CoinsManageWidget::~CoinsManageWidget()
{
    delete ui;
}

void CoinsManageWidget::initShow(int type, long amount)
{
    m_type = type;
    // 操作信息显示
    ui->listWidget->clear();
    QString info1 = QString("加币金额%1元").arg(amount);
    if (type == RETURN_COIN) {
        info1 = QString("退币金额%1元").arg(amount);
    }
    ui->infoLabel->setText(info1);
    showInfo(info1);

    // 设备状态检查，限制功能使用
    BYTE devState = DataCenter::getThis()->getCashboxState();
    bool isSpecieOn = devState & 0x01;
    if (!isSpecieOn) {
        MyHelper::ShowMessageBoxError("硬件设备故障，请联系工作人员。");
        logger()->error("硬件设备故障，cashboxState=%1", devState);
        emit sigWorkOk(false);
        close();
        return;
    }

    // 操作金额校验
    if (amount <= 0) {
        MyHelper::ShowMessageBoxInfo("无需操作");
        logger()->info("操作金额为%1，退出操作。", amount);
        emit sigWorkOk(true);
        close();
        return;
    }

    // 数值初始化
    setNeedReturn(false);
    m_difference = amount;
    m_amount = 0;
    m_payingState = -1;    // 初始投币状态
    QString logInfo = QString("需投币%1元").arg(m_difference);
    logForCashbox(logInfo);

    // 页面显示控制
    ui->endBtn->setText("结束");
    ui->continueBtn->setText("开始");
    ui->returnCashBtn->setText("关闭");

    ui->continueBtn->setDisabled(false);
    ui->endBtn->setDisabled(true);
    ui->returnCashBtn->setDisabled(false);

    show();
}

void CoinsManageWidget::onAutoStopPaying(int amount)
{
    m_payingState = 2;
    ui->continueBtn->setDisabled(true);
    ui->endBtn->setDisabled(true);

    QString info3 = QString("已投入硬币%1元").arg(amount);
    if (m_type == RETURN_COIN) {
        info3 = QString("已退出硬币%1元").arg(amount);
    }
    logForCashbox(info3);
    showInfo(info3);

    m_amount = amount;
    onAmountConfirm(m_amount);
}

void CoinsManageWidget::onStopPaying()
{
    emit sigWorStart(m_type, true);
    m_payingState = 2;

    if (m_type == ADD_COIN) {
        stopAddCoin();
    }
}

void CoinsManageWidget::showCheckState(int state, int amount)
{

}


void CoinsManageWidget::activeWork()
{
    // 0-开始   1-继续   2-停止
    if (m_payingState == -1) {
        showInfo("开始...");
        startWork();
    } else {
        showInfo("继续...");
        continueWork();
    }

    ui->continueBtn->setText("继续");
    ui->continueBtn->setDisabled(true);
    ui->endBtn->setDisabled(false);
}

void CoinsManageWidget::startWork()
{
    if (m_type == ADD_COIN) {
        startAddCoin();
    } else {

    }

}

void CoinsManageWidget::startAddCoin()
{
    int ret = StartPutCoin(m_difference);
    QString info3 = QString("打开阀门StartPutCoin(%1)=%2").arg(m_difference).arg(ret);
    showInfo(info3);

    if (ret != 0 && ret != 1) {
        logger()->error("[StartPutCoin]开始投币={%1}", ret);
        MyHelper::ShowMessageBoxError("投币模块故障，暂时无法使用，请联系工作人员解决。");
        DataCenter::getThis()->setCashboxState(-1, -1, 0, 0x03);
        logForCashbox(QString("阀门打开故障[%1]，投币结束。").arg(ret));
        emit sigWorkOk(false);
        close();
        return;
    }

    emit sigWorStart(ADD_COIN, true);
    m_payingState = 0;
}

void CoinsManageWidget::stopAddCoin()
{
    // 结束硬件投币
    int banknotes = 0;
    int coins = 0;
    int ret = StopPutCoin(&banknotes, &coins);

    QString info3 = QString("StopPutCoin()=%1，投入纸币%2，投入硬币%3").arg(ret).arg(banknotes).arg(coins);
    logForCashbox(info3);
    showInfo(info3);

    if (m_amount > 0 && (banknotes + coins) == 0) {
        showInfo("StopPutCoin接口返回的投币金额为0时，使用投币状态检查接口返回的投币金额。");
        onAmountConfirm(m_amount);
    } else {
        onAmountConfirm(coins);
    }
}


void CoinsManageWidget::continueWork()
{
    m_payingState = 1;
    emit sigWorStart(m_type, true);
}

void CoinsManageWidget::amountCheck()
{

}

void CoinsManageWidget::onReturnMoney()
{
    if (m_type == RETURN_COIN) {
        close();
        return;
    }

    if (m_isNeedReturn) {
        int ret = ResultOperate(0);

        if (ret != 0) {
            DataCenter::getThis()->setCashboxState(-1, -1, 0, 0x03);
            QString info = QString("退币失败{%1}，用户已投入%2元").arg(ret).arg(m_amount);

            MyHelper::ShowMessageBoxError(info);

            logger()->error(info);
            logForCashbox(info);
        }

        setNeedReturn(false);
        QString info1 = QString("投入%1元退币成功。").arg(m_amount);
        logForCashbox(info1);
    }

    emit sigWorkOk(false);
    close();
}

void CoinsManageWidget::onAmountConfirm(int amount)
{
    if (m_type == ADD_COIN) {
        addAmountConfirm(amount);
    } else {
        returnAmountConfirm(amount);
    }
}

void CoinsManageWidget::addAmountConfirm(int amount)
{
    int amount2 = Request_Hopper_Balance();   // 检查找找零器中的硬币余额
    int amount3 = Request_Cashbox_Counter();   //查询钱箱中的硬币数量
    QString amountInfo = QString("找零器中硬币余额%1元,钱箱中硬币金额%2元").arg(amount2).arg(amount3);

    showInfo(amountInfo);
    showInfo("结束投币，请确认投币金额。");

    QString info1 = QString("投入硬币金额：%1元，%2").arg(amount).arg(amountInfo);
    showInfo(info1);
    logForCashbox(info1);

    if (m_amount <= 0) {
        setNeedReturn(false);
    } else {
        setNeedReturn(true);
    }
    ui->returnCashBtn->setDisabled(false);

//    // 无需继续投币
//    ui->continueBtn->setText("开始投币");
//    ui->continueBtn->setDisabled(true);
//    ui->endBtn->setDisabled(true);

    // 投币金额及找零金额，用户确认
//    int changeAmount = m_income > m_difference ? (m_income - m_difference) : 0;
//    QString info = QString("已投入纸币%1元，投入硬币%2元，无需找零，点击确认按钮完成投币！").arg(banknotes).arg(coins);
//    if (changeAmount > 0) {
//        info = QString("已投入纸币%1元，投入硬币%2元，应找零%3元，点击确认按钮开始找零！\n对金额有疑问，请手动退币或联系工作人员。")
//                .arg(banknotes).arg(coins).arg(changeAmount);
//    }
//    showInfo(info);
//    int ret = MyHelper::ShowMessageBoxQuesion(info);
//    if (ret != 1) {
//        qDebug() << "用户对金额有疑问，";
//        return;
//    }


//    emit sigCashboxIn();
//    showInfo("钱进钱箱");
//    logForCashbox(QString("钱进钱箱"));


//    setNeedReturn(false);

//    emit supplementaryOk(true);
//    emit stopReading(true, 1);
    close();

}

void CoinsManageWidget::returnAmountConfirm(int amount)
{

}

void CoinsManageWidget::showInfo(QString info)
{
    QString timeStr = QString::number(QDateTime::currentMSecsSinceEpoch());
    ui->listWidget->insertItem(0, timeStr + "  " +info);

    logger()->info(info);
}

void CoinsManageWidget::setNeedReturn(bool needReturn)
{
    m_isNeedReturn = needReturn;
    if (!m_isNeedReturn) {
        ui->returnCashBtn->setText("关闭");
    } else {
        ui->returnCashBtn->setText("退币");
    }
}

void CoinsManageWidget::logForCashbox(QString line)
{
    QDateTime curTime = QDateTime::currentDateTime();
    QString operatorId = DataCenter::getThis()->getOperatorId();
    QString prefixInfo = QString("%1  [操作员%2]%3")
            .arg(curTime.toString("yyyy-MM-dd HH:mm:ss"))
            .arg(operatorId)
            .arg(line);

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

