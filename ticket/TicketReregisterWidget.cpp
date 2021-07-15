#include "TicketReregisterWidget.h"
#include "ui_TicketReregisterWidget.h"
#include "CommonHead.h"
#include "DataCenter.h"
#include "CardInfo.h"

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

bool TicketReregisterWidget::showData()
{
    CardInfo* info = DataCenter::getThis()->getCardInfo();

    // TODO:类型转换
    ui->lineEdit1->setText(QString::number(info->type()));
    ui->lineEdit2->setText(QString::number(info->state()));

//    ui->lineEdit3->setText(info->enterStation());
    ui->lineEdit4->setText(info->enterTime().toString("yyyy-MM-dd HH:mm:ss"));
//    ui->lineEdit5->setText(info->exitStation());
    ui->lineEdit6->setText(info->exitTime().toString("yyyy-MM-dd HH:mm:ss"));
    ui->lineEdit7->setText(QString("%1").arg(info->arrearage(), 0, 'f', 1));
    ui->lineEdit8->setText(info->reason());


    return true;
}



void TicketReregisterWidget::init()
{
    setStyle();

    m_curBtn = 0;
    connect(ui->selectBtn2, &QPushButton::clicked,
            this, &TicketReregisterWidget::onEnStationSelect);
    connect(ui->selectBtn3, &QPushButton::clicked,
            this, &TicketReregisterWidget::onExStationSelect);

}

void TicketReregisterWidget::onStationSelected(QString lineName, QString stationName, QString stationCode)
{
    if (m_curBtn) {
        ui->lineEdit5->setText(stationName);
    } else {
        ui->lineEdit3->setText(stationName);
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
    ui->lineEdit7->setDisabled(true);
    ui->lineEdit8->setDisabled(true);

}

void TicketReregisterWidget::setTestData()
{
    QDateTime enterTime = QDateTime::currentDateTime().addDays(-3);
    QDateTime exitTime = enterTime.addMSecs(32);

    CardInfo* info = new CardInfo(2,0,"地铁大厦",enterTime,"八一广场",exitTime,
                                  QDate(2022,10,01), 1.6, "出站刷卡失败。");
    DataCenter::getThis()->setCardInfo(info);
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
