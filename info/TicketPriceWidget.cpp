#include "TicketPriceWidget.h"
#include "ui_TicketPriceWidget.h"
#include "HttpTool.h"
#include "StationSelectWidget.h"

TicketPriceWidget::TicketPriceWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::TicketPriceWidget)
{
    ui->setupUi(this);

    init();
}

TicketPriceWidget::~TicketPriceWidget()
{
    delete ui;
}



void TicketPriceWidget::init()
{
    setStyle();

    m_curBtn = 0;

    //# 选择车站
    StationSelectWidget* stationSelectedWidget = new StationSelectWidget();
    stationSelectedWidget->hide();
    connect(this, &TicketPriceWidget::selectStation, stationSelectedWidget, &StationSelectWidget::showData);
    connect(stationSelectedWidget, &StationSelectWidget::stationSelected, this, &TicketPriceWidget::onStationSelected);

    connect(ui->selectBtn0, &QPushButton::clicked, this, &TicketPriceWidget::onEnStationSelect);
    connect(ui->selectBtn1, &QPushButton::clicked, this, &TicketPriceWidget::onExStationSelect);
    connect(ui->priceCalBtn, &QPushButton::clicked, this, &TicketPriceWidget::onPriceCal);

    // 票价查询
    connect(HttpTool::getThis(), &HttpTool::priceReceived, this, &TicketPriceWidget::onPriceRecv);
}

void TicketPriceWidget::onStationSelected(QString lineName, QString stationName, QString stationCode)
{
    if (m_curBtn) {
        ui->lineEdit_2->setText(stationName);
        ui->lineEdit_2->setToolTip(stationCode);
    } else {
        ui->lineEdit->setText(stationName);
        ui->lineEdit->setToolTip(stationCode);
    }
}

void TicketPriceWidget::onPriceCal()
{
    QString enStationCode = ui->lineEdit->toolTip();
    QString exdStationCode = ui->lineEdit_2->toolTip();
    HttpTool::getThis()->requestPrice(enStationCode, exdStationCode);
}

void TicketPriceWidget::onPriceRecv(double price)
{
    ui->lineEdit_3->setText(QString("%1").arg(price));
}

bool TicketPriceWidget::showData()
{
    ui->lineEdit_3->setText("");
    return true;
}


void TicketPriceWidget::setStyle()
{
    ui->lineEdit->setDisabled(true);
    ui->lineEdit_2->setDisabled(true);
    ui->lineEdit_3->setDisabled(true);
}

void TicketPriceWidget::onEnStationSelect()
{
    m_curBtn = 0;
    emit selectStation();
}

void TicketPriceWidget::onExStationSelect()
{
    m_curBtn = 1;
    emit selectStation();
}
