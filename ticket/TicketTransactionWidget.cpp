#include "TicketTransactionWidget.h"
#include "ui_TicketTransactionWidget.h"
#include "WidgetMng.h"
#include "CommonHead.h"
#include "StationSelectWidget.h"

TicketTransactionWidget::TicketTransactionWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::TicketTransactionWidget)
{
    ui->setupUi(this);

    init();
}

TicketTransactionWidget::~TicketTransactionWidget()
{
    delete ui;
}

void TicketTransactionWidget::init()
{
    connect(ui->payBtn, &QPushButton::clicked,
            this, &TicketTransactionWidget::onPurchase);
    connect(ui->selectBtn1, &QPushButton::clicked,
            this, &WidgetBase::selectStation);

}

void TicketTransactionWidget::onPurchase()
{
    WidgetMng::getThis()->showWidget(PAYMENT_DLG);
}

void TicketTransactionWidget::onStationSelected(QString lineName, QString stationName, QString stationCode)
{
    // TODO:

    ui->stationNameLineEdit->setText(stationName);

}

