#include "CompensationFareWidget.h"
#include "ui_CompensationFareWidget.h"
#include "BIM2020.h"
//#include "F53Board.h"
//#include "BRCBoard.h"

#include "CommonHead.h"
#include "MyHelper.h"

static int RETRY_TIMES = 3;
CompensationFareWidget::CompensationFareWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompensationFareWidget)
{
    ui->setupUi(this);
    init();
}

CompensationFareWidget::~CompensationFareWidget()
{
    // 设备关闭
//    BRC_Close();
//    BIM_CLOSE();
//    F53Bill_DisConnect();

    delete ui;
}

void CompensationFareWidget::secEvent()
{
    // 投币
    if (m_startPaying && (m_amount < m_difference)) {
        deviceCheck();
    }

    if (m_reSpecieAmount > 0 && m_isSpecieReturn) {
        specieReturnCheck();
    }
}


void CompensationFareWidget::initShow()
{
    ui->infoLabel->setText("");
    ui->listWidget->clear();
    ui->endBtn->setText("结束投币");
//    ui->endBtn->setDisabled(true);
    ui->continueBtn->setText("开始投币");
    ui->continueBtn->setDisabled(true);
    ui->returnCashBtn->setDisabled(true);

    m_banknotesAmount = 0;
    m_specieAmount = 0;
    m_startPaying = false;

    m_initSpecieAmount = 0;
    m_reSpecieAmount = 0;
    m_isSpecieReturn = false;
    m_sReturnRetryCnt = 0;

    m_successFlag = true;

    deviceStateShow();
    show();
}


void CompensationFareWidget::getFare(double difference, uchar devState)
{
    m_difference = difference;
    ui->infoLabel->setText(QString("请投币%1元。").arg(m_difference));
    ui->listWidget->clear();

    if ((m_isBanknotesOn && m_isBanknotesReturnOn) || m_isSpecieOn) {
        // 打开投币口阀门
        if (!deviceOpen()) {
            // 弹窗提示：阀门无法打开，不可用
            MyHelper::ShowMessageBoxInfo("投币口阀门无法打开，请联系工作人员。");
            close();
            return;
        }

        // 开始纸币、硬币检测
        m_startPaying = true;

        ui->endBtn->setText("结束投币");
        ui->endBtn->setDisabled(false);
        ui->listWidget->insertItem(0, "阀门已打开，请开始投币。");
    } else {
        ui->continueBtn->setDisabled(true);
        ui->endBtn->setDisabled(true);
        ui->endBtn->setText("结束投币");

        // 弹窗提示当前不可用
        MyHelper::ShowMessageBoxInfo("钱币接收设备无法使用，请联系工作人员。");
        close();
        return;
    }

}

void CompensationFareWidget::init()
{
    setStyle();
    initDevice();

    // 设置非互斥
    QButtonGroup* group = new QButtonGroup(this);
    group->setExclusive(false);
    group->addButton(ui->bimRadioBtn);
    group->addButton(ui->brcRadioBtn);
    group->addButton(ui->f53RadioBtn);

    ui->bimRadioBtn->setDisabled(true);
    ui->brcRadioBtn->setDisabled(true);
    ui->f53RadioBtn->setDisabled(true);
    connect(ui->endBtn, &QPushButton::clicked, this, &CompensationFareWidget::onAmountConfirm);
    connect(ui->endBtn, &QPushButton::clicked, this, &CompensationFareWidget::onReturnMoney);

    m_difference = 0.0;
    m_amount = 0.0;
    m_startPaying = false;

    connect(this, &CompensationFareWidget::stopPaying, this, &CompensationFareWidget::onStopPaying);
    connect(this, &CompensationFareWidget:: change, this, &CompensationFareWidget::onChange);
}

void CompensationFareWidget::initDevice()
{
    // 打开设备
//    //# 硬币
//    int retBRC = BRC_Connect(3);
//    logger()->info("硬币模块串口：{%1}，硬币模块自检：{%2}", retBRC, Perform_Self_Check());

//    //# 纸币
//    int retBIM = BIM_Connect(3);
//    logger()->info("纸币模块串口：{%1}，纸币模块自检：{%2}", retBIM, BIM_Initial());

//    //# 纸币找零
//    int retF53 = F53Bill_Connect(3);
//    logger()->info("纸币找零模块串口：{%1}", retF53);

//    m_isSpecieOn = (retBRC == 0);
//    m_isBanknotesOn = (retBIM == 0);
//    m_isBanknotesReturnOn = (retF53 == 0);

//    // TODO:Test
//    m_isSpecieOn = true;
//    m_isBanknotesOn = true;
//    m_isBanknotesReturnOn = true;
}


bool CompensationFareWidget::deviceOpen()
{
    int ret = ConnectMachine(2, 3, 4);
//    // 打开投币口阀门
//    //# 硬币
//    int sTryTime = 0;
//    while (sTryTime++ < RETRY_TIMES) {
//        int retClear = Clear_Money_Counters();
//        if (retClear == 0) {
//            int ret = Modify_inhibit_Open();
//            if (ret == 0) {
//                ui->listWidget->insertItem(0, "硬币阀门已打开。");
//                logger()->info("硬币阀门打开。");
//                m_isSpecieOn = true;
//                break;
//            } else {
//                m_isSpecieOn = false;
//                ui->listWidget->insertItem(0, "硬币阀门打开失败。");
//                logger()->error("[%1] - 硬币阀门打开失败，返回值{%2}", sTryTime, ret);
//            }
//        } else {
//            m_isSpecieOn = false;
//            ui->listWidget->insertItem(0, "硬币器故障，无法清零。");
//            logger()->error("[%1] - 硬币清零失败，Clear_Money_Counters() = {%2}", sTryTime, retClear);
//        }
//    }


//    //# 纸币
//    int bTryTime = 0;
//    while(bTryTime++ < RETRY_TIMES) {
//        int retRest = BIM_Reset();
//        if (retRest == 0) {
//            int ret = BIM_OPEN();
//            if (ret == 0) {
//                m_isBanknotesOn = true;
//                ui->listWidget->insertItem(0, "纸币阀门已打开。");
//                logger()->info("纸币阀门打开。");
//                break;
//            } else {
//                m_isBanknotesOn = false;
//                ui->listWidget->insertItem(0, "纸币阀门打开失败。");
//                logger()->error("[%1] - 纸币阀门打开失败，返回值{%2}", bTryTime, ret);
//            }
//        } else {
//            m_isBanknotesOn = false;
//            ui->listWidget->insertItem(0, "纸币器故障，复位失败。");
//            logger()->error("[%1] - 纸币器复位失败，BIM_Reset() = {%2}", bTryTime, retRest);
//        }
//    }

    // TODO:Test
    m_isSpecieOn = true;
    m_isBanknotesOn = true;

    deviceStateShow();
    return ((m_isBanknotesOn && m_isBanknotesReturnOn) || m_isSpecieOn);
}

bool CompensationFareWidget::deviceClose()
{
//    //# 硬币
//    int sTryTime = 0;
//    while (sTryTime++ < RETRY_TIMES) {
//        int ret = Modify_inhibit_Closes();
//        if (ret == 0) {
//            ui->listWidget->insertItem(0, "硬币阀门关闭成功。");
//            logger()->info("硬币阀门关闭。");
//            m_isSpecieOn = true;
//            break;
//        } else {
//            m_isSpecieOn = false;
//            ui->listWidget->insertItem(0, "硬币阀门关闭失败。");
//            logger()->error("[%1] - 硬币阀门关闭失败，返回值{%2}", sTryTime, ret);
//        }
//    }

//    //# 纸币
//    int bTryTime = 0;
//    while(bTryTime++ < RETRY_TIMES) {
//        int ret = BIM_CLOSE();
//        if (ret == 0) {
//            m_isBanknotesOn = true;
//            ui->listWidget->insertItem(0, "纸币阀门已关闭。");
//            logger()->info("纸币阀门关闭。");
//            break;
//        } else {
//            m_isBanknotesOn = false;
//            ui->listWidget->insertItem(0, "纸币阀门关闭失败。");
//            logger()->error("[%1] - 纸币阀门关闭失败，返回值{%2}", bTryTime, ret);
//        }
//    }

    deviceStateShow();
    return (m_isBanknotesOn && m_isSpecieOn);
}

void CompensationFareWidget::deviceStateShow()
{
    ui->bimRadioBtn->setChecked(m_isBanknotesOn);
    ui->f53RadioBtn->setChecked(m_isBanknotesReturnOn);
    ui->brcRadioBtn->setChecked(m_isSpecieOn);
}

int CompensationFareWidget::requestBanknotesIn()
{
    return 0;
}

void CompensationFareWidget::onAmountConfirm()
{
    m_amount = m_banknotesAmount + m_specieAmount;
    if (m_amount == m_difference) {    // 结束投币
        emit stopPaying();
    } else if (m_amount > m_difference) {    // 退币并结束投币
        int difference = m_amount - m_difference;
        emit change(difference);
    }
}


// 退币
void CompensationFareWidget::onReturnMoney()
{
    m_successFlag = false;

//    // 将最后检测到的纸币及暂存器中的纸币退出
//    int bTryTime = 0;
//    while (bTryTime++ < RETRY_TIMES) {
//        if(BIM_ReturnBankNote() == 0) {
//            ui->listWidget->insertItem(0, "收到的纸币已退出。");
//            break;
//        }
//    }

//    // 硬币
//    if (m_specieAmount <= 0) {
//        return;
//    }

//    int amount = Request_Hopper_Balance();
//    if (amount >= 0) {
//        m_initSpecieAmount = amount;
//    }
//    int ret = Dispense_Hopper_Pattern(m_specieAmount);
//    if(ret == 0) {
//        m_isSpecieReturn = true;
//        ui->listWidget->insertItem(0, QString("[硬币]开始退币"));
//    } else {
//        ui->listWidget->insertItem(0, QString("硬币退币失败，请联系工作人员"));
//        MyHelper::ShowMessageBoxInfo(QString("硬币退币失败[%1]，请联系工作人员处理。").arg(ret));
//    }

//    emit supplementaryOk(false);
//    close();
}


void CompensationFareWidget::deviceCheck()
{
//    int retBRC = SimplePoll();
//    int retBIM = BIM_Poll();
//    int retF53 = F53Poll();

    // TODO:test
    int retBRC = 0;
    int retBIM = 0;
    int retF53 = 0;

    // 硬币
//    if (retBRC == 0) {
////        int inSpecie = Request_money_in();
////        inSpecie = inSpecie > 0 ? inSpecie : 0;

//        // TODO:test
//        int inSpecie = inSpecie > 0 ? inSpecie : 1;
//        m_specieAmount += inSpecie;
//        ui->listWidget->insertItem(0, QString("收到硬币%1元").arg(inSpecie));
//    }

//    // 纸币
//    if (retBIM == 0) {
//        int inBanknotes = requestBanknotesIn();
//        inBanknotes = inBanknotes > 0 ? inBanknotes : 1;

//        m_banknotesAmount += inBanknotes;
//        ui->listWidget->insertItem(0, QString("收到纸币%1元").arg(inBanknotes));
//    }

//    m_isSpecieOn = (retBRC == 0);
//    m_isBanknotesOn = (retBIM == 0);
//    m_isBanknotesReturnOn = (retF53 == 0);
//    deviceStateShow();


    // 金额确认
    onAmountConfirm();
}

void CompensationFareWidget::specieReturnCheck()
{
//    int amount = Request_Hopper_Balance();
//    if (amount < 0) {
//        if (m_sReturnRetryCnt++ > 5) {
//            m_isSpecieReturn = false;

//            // TODO:多次无法读取找零器余额时，如何处理

//        }
//    }

//    // 退币完成
//    if (m_initSpecieAmount - amount >= m_reSpecieAmount) {
//        ui->listWidget->insertItem(0, QString("退币完成，金额%1元").arg(m_reSpecieAmount));
//        m_isSpecieReturn = false;
//        m_initSpecieAmount = 0;
//        m_reSpecieAmount = 0;
//        m_sReturnRetryCnt = 0;

//        // 信号
//        emit supplementaryOk(m_successFlag);
//        close();
//    }
}

void CompensationFareWidget::onStopPaying()
{
    m_startPaying = false;
    ui->listWidget->insertItem(0, QString("投币结束，收到%1元。").arg(m_amount));

    // 设备初始化
    deviceClose();

    // 参数初始化
    m_amount = 0;
    m_difference = 0;

    m_successFlag = true;

    emit supplementaryOk(m_successFlag);
}

bool CompensationFareWidget::onChange(int amount)
{
//    m_startPaying = false;
//    ui->listWidget->insertItem(0, QString("收到钱币总额%1元，需找零%2元").arg(m_amount).arg(amount));

//    int reBanknotesNum = 5 * (amount / 5);
//    int reSpecieNum = (amount % 5);
//    if (m_isBanknotesOn && m_isBanknotesReturnOn && reBanknotesNum > 0) {
//        if (F53Bill_BillCount(reBanknotesNum, 0) == 0) {
//            ui->listWidget->insertItem(0, QString("纸币找零%1元。").arg(reBanknotesNum));
//            logger()->info("纸币找零%1元", reBanknotesNum);
//            if (reSpecieNum <= 0) {
//                emit supplementaryOk(true);
//                close();
//            }

//        } else {
//            ui->listWidget->insertItem(0, QString("纸币找零失败。"));
//            reSpecieNum += reBanknotesNum;
//            logger()->info("纸币找零(%1元)失败", reBanknotesNum);
//        }
//    }

//    if (m_isSpecieOn && reSpecieNum > 0 && Request_Cashbox_Counter() > reSpecieNum) {
//        if (Dispense_Hopper_Pattern(reSpecieNum) == 0) {
//            ui->listWidget->insertItem(0, QString("硬币找零%1元。").arg(reSpecieNum));
//            logger()->info("硬币找零%1元", reSpecieNum);
//            emit supplementaryOk(true);
//            close();
//        } else {
//            ui->listWidget->insertItem(0, QString("硬币找零失败。"));
//            logger()->info("硬币找零(%1元)失败", reSpecieNum);

//            MyHelper::ShowMessageBoxError("找零失败，请手动退币。");
//            ui->returnBtn->setDisabled(false);
//            ui->continueBtn->setDisabled(true);
//            ui->endBtn->setDisabled(true);

//        }
//    }

    // 参数初始化
    m_amount = 0;
    m_difference = 0;
}


void CompensationFareWidget::setAmout(double amout)
{
    m_amount = amout;
}



void CompensationFareWidget::setStyle()
{
    // 透明背景设置
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QScreen *screen = QGuiApplication::primaryScreen();   //获取当前屏幕的大小
    QRect mm = screen->availableGeometry();
    int screenWidth = mm.width();
    int screenHeight = mm.height();
    this->setFixedSize(screenWidth, screenHeight);
}


//    unsigned char Number;
//    char Model,ErrStatus[16],Ainfo[16],BankNote[16];
//    int status;
//    memset(ErrStatus, 0, sizeof(ErrStatus));
//    memset(Ainfo, 0, sizeof(Ainfo));
//    memset(BankNote, 0, sizeof(BankNote));
//    status = BIM_RequestStatus(&Model, &Number, ErrStatus, Ainfo, BankNote);
//    if(status) {
//        //MessageBox("操作失败",NULL,MB_OK);
//    }
//    else {
//        if(Model==0x30)
////                AddListItem("待命模式");
//        if(Model==0x31)
////                AddListItem("检验模式");

//        if(memcmp(Ainfo, "A02", 3) == 0)	//
//        {
////                AddListItem("检测到钞票");
//            if(memcmp(BankNote,"D02",3)==0)
////                        AddListItem("检测到钞票面额5元");

//            else if(memcmp(BankNote,"D06",3)==0)
////                        AddListItem("检测到钞票面额10元");
//            else
////                    AddListItem(BankNote);
//        }

//        cs.Format("暂存钞票数 %d张",Number);
//        AddListItem(cs.GetBuffer(0));
//    }
