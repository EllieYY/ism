#include "TicketPriceWidget.h"
#include "ui_TicketPriceWidget.h"
#include "HttpTool.h"

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
    connect(ui->selectBtn2, &QPushButton::clicked,
            this, &TicketPriceWidget::onEnStationSelect);
    connect(ui->selectBtn3, &QPushButton::clicked,
            this, &TicketPriceWidget::onExStationSelect);
    connect(ui->priceCalBtn, &QPushButton::clicked,
            this, &TicketPriceWidget::onPriceCal);

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
