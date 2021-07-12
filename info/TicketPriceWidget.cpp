#include "TicketPriceWidget.h"
#include "ui_TicketPriceWidget.h"

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

}

void TicketPriceWidget::onStationSelected(QString lineName, QString stationName)
{
    if (m_curBtn) {
        ui->lineEdit_2->setText(stationName);
    } else {
        ui->lineEdit->setText(stationName);
    }

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
