#include "TicketReregisterWidget.h"
#include "ui_TicketReregisterWidget.h"
#include "CommonHead.h"
#include "DataCenter.h"
#include "ReregisterInfo.h"
#include "StationSelectWidget.h"
#include "CompensationFareWidget.h"

TicketReregisterWidget::TicketReregisterWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::TicketReregisterWidget)
{
    ui->setupUi(this);

    init();

#if IS_TEST_MODE
    setTestData();
#endif
}

TicketReregisterWidget::~TicketReregisterWidget()
{
    delete ui;
}

void TicketReregisterWidget::secEvent()
{
    if (m_fareWidget->isVisible()) {
        m_fareWidget->secEvent();
    }
}

bool TicketReregisterWidget::showData()
{
    if (!m_dataUpdateNum[TICKET_REREGISTER]) {
        return true;
    }

    m_dataUpdateNum[TICKET_REREGISTER] = false;

    ReregisterInfo* info = DataCenter::getThis()->getReregisterInfo();

    // 票卡基本信息显示
    QList<QTableWidgetItem*> itemList = DataCenter::getThis()->getRegisterItems(info);
    int index = 0;
    for (QTableWidgetItem* item:itemList) {
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont("Microsoft YaHei",18,500));
        item->setForeground(QColor("#09262A"));

        ui->tableWidget->setItem(0, index++, item);
    }

    ui->selectBtn2->setDisabled(true);
    ui->selectBtn3->setDisabled(true);
    int reregisterType = info->reregisterType();
    m_updateType = reregisterType;
    switch(reregisterType) {
    case NONE_TYPE:   // 不需要更新     
        ui->tUpdateBtn->setDisabled(true);
        ui->lineEdit5->setText("无");
        ui->lineEdit6->setText("0.0");
        ui->textTips->setText("票卡无需更新，如有疑问，请联系工作人员。");
        break;
    case EN_LACK:   // 进站更新
        ui->selectBtn2->setDisabled(false);
        ui->textTips->setText("请补进站信息。");

        break;
    case EX_LACK:   // 出站更新
        ui->selectBtn3->setDisabled(false);
        ui->textTips->setText("请补出站信息。");

        break;
    case OVERTIME:  // 超时更新
        ui->textTips->setText("站内停留时间超过规定时长，请补2元票价。");

        break;
    case OVER_TRIP:  // 超程更新
    {
        // TODO:用当前站点和进站点去计算票价，算出补票金额
        double ticketPrice = 3;
        double balance = 2;
        m_difference = (ticketPrice > balance) ? (ticketPrice - balance) : 0;
        ui->textTips->setText(QString("旅程票价：%1，票面值：%2，请补差价：%3。")
                              .arg(ticketPrice)
                              .arg(balance)
                              .arg(m_difference));
    }
        break;
    case OVER_TIME_TRIP:   // 超时超程更新
    {
        // TODO:用当前站点和进站点去计算票价，算出补票金额
        // TODO:用当前站点和进站点去计算票价，算出补票金额
        double ticketPrice = 3;
        double balance = 2;
        m_difference = (ticketPrice > balance) ? (ticketPrice - balance) : 0;
        ui->textTips->setText(QString("旅程票价：%1，站内滞留费：2元，票面值：%2，请补差价：%3。")
                              .arg(ticketPrice)
                              .arg(balance)
                              .arg(m_difference));
    }

        break;
    default:
        break;
    }

    // 进站时间 | 出站时间
    ui->lineEdit1->setText(info->enterTime().toString("yyyy-MM-dd HH:mm:ss"));
    ui->lineEdit3->setText(info->exitTime().toString("yyyy-MM-dd HH:mm:ss"));
    // 进站站点
    if (reregisterType != EN_LACK ) {
        ui->lineEdit2->setText(info->enterStation());
    }
    // 出站站点
    if (info->reregisterType() != EX_LACK ) {
        ui->lineEdit4->setText(info->enterStation());
    }
    ui->lineEdit5->setText(info->reason());


    return true;
}


void TicketReregisterWidget::init()
{
    setStyle();

    m_curBtn = 0;
    connect(ui->selectBtn2, &QPushButton::clicked, this, &TicketReregisterWidget::onEnStationSelect);
    connect(ui->selectBtn3, &QPushButton::clicked, this, &TicketReregisterWidget::onExStationSelect);

    //# 选择车站
    StationSelectWidget* stationSelectedWidget = new StationSelectWidget(this);
    stationSelectedWidget->hide();
    connect(this, &TicketReregisterWidget::selectStation, stationSelectedWidget, &StationSelectWidget::showData);
    connect(stationSelectedWidget, &StationSelectWidget::stationSelected, this, &TicketReregisterWidget::onStationSelected);

    m_updateType = NONE_TYPE;
    connect(ui->tUpdateBtn, &QPushButton::clicked, this, &TicketReregisterWidget::onUpdateTicket);
    connect(ui->cashPollBtn, &QPushButton::clicked, this, &TicketReregisterWidget::cashSupplementary);

    m_difference = 0;
    m_fareWidget = new CompensationFareWidget();
    m_fareWidget->hide();

    connect(m_fareWidget, &CompensationFareWidget::supplementaryOk, this, &TicketReregisterWidget::onSupplementaryOk);
}

void TicketReregisterWidget::onStationSelected(QString lineName, QString stationName, QString stationCode)
{
    if (m_curBtn) {
        ui->lineEdit4->setText(stationName);
    } else {
        ui->lineEdit2->setText(stationName);
    }
}

void TicketReregisterWidget::onUpdateTicket()
{

    switch(m_updateType) {
    case NONE_TYPE:   // 不需要更新
        break;
    case EN_LACK:   // 进站更新

        break;
    case EX_LACK:   // 出站更新

        break;
    case OVERTIME:          // 超时更新
    case OVER_TRIP:         // 超程更新
    case OVER_TIME_TRIP:    // 超时超程更新
    {

    }

        break;
    default:
        break;
    }

}

void TicketReregisterWidget::cashSupplementary()
{
    if (m_difference > 0) {
        m_fareWidget->initShow();

        BYTE state = DataCenter::getThis()->getCashboxState();
        m_fareWidget->getFare(m_difference, state);
    }
}

void TicketReregisterWidget::onSupplementaryOk(bool result)
{
    if (result) {
        //TODO: 票卡更新


    } else {
        //TODO: 更新失败提醒
    }

}


void TicketReregisterWidget::setStyle()
{
    ui->lineEdit1->setDisabled(true);
    ui->lineEdit2->setDisabled(true);
    ui->lineEdit3->setDisabled(true);
    ui->lineEdit4->setDisabled(true);
    ui->lineEdit5->setDisabled(true);
    ui->lineEdit6->setDisabled(true);

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(70);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setFrameShape(QFrame::NoFrame);

    ui->tableWidget->verticalHeader()->setVisible(false);                 // 列表头不可见
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 表格不可编辑

    ui->tableWidget->setAlternatingRowColors(true);

}

void TicketReregisterWidget::setTestData()
{
    QDateTime enterTime = QDateTime::currentDateTime().addDays(-3);
    QDateTime exitTime = enterTime.addMSecs(32);

    ReregisterInfo* info = new ReregisterInfo(this);
    info->setType(1);
    info->setNumber("100289996");
    info->setState(0);
    info->setCreateTime(QDate(2019, 10, 1));
    info->setValidDate(QDate(2022, 10, 1));
    info->setBalance(35.6);

    info->setEnterTime(enterTime);
    info->setExitTime(exitTime);
    info->setEnterStation("秋水广场站");
    info->setExitStation("市民中心站");
    int reregisterType = OVER_TIME_TRIP;
    info->setReregisterType(reregisterType);
    info->setReason(DataCenter::getThis()->getReregisterTypeString(reregisterType));

    DataCenter::getThis()->setReregisterInfo(info);
}

void TicketReregisterWidget::onEnStationSelect()
{
    m_curBtn = 0;
    emit selectStation();
}

void TicketReregisterWidget::onExStationSelect()
{
    m_curBtn = 1;
    emit selectStation();
}
