#include "StationListWidget.h"
#include "ui_StationListWidget.h"
#include "Station.h"

StationListWidget::StationListWidget(QString lineName, QList<Station*> stations, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StationListWidget)
{
    ui->setupUi(this);

    setStyle();


    m_lineName = lineName;

    // 显示列表
    ui->listWidget->clear();
    for (Station* s:stations ) {
        QListWidgetItem* item = new QListWidgetItem(s->name(), ui->listWidget);
        item->setToolTip(s->code());
        ui->listWidget->addItem(item);
    }
    connect(ui->listWidget, &QListWidget::itemClicked, this, &StationListWidget::onSelected);
}

StationListWidget::~StationListWidget()
{
    delete ui;
}

void StationListWidget::onSelected(QListWidgetItem *item)
{
    QString station = item->text();
    QString code = item->toolTip();
    emit selected(m_lineName, station, code);
}

void StationListWidget::setStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("border: none;");

    setStyleSheet(
                "QListWidget"
                "{"
                "    border: none;   "
                "    background: #FFFFFF;    "
                "    color: #09262A;           "
                "    font-size: 32px;"
                "    font-weight: 500;"
                "    line-height: 80px;"
                "    outline: none;"
                "} "
                "QListWidget::item"
                "{"
                "    border-bottom: 1px solid #EEECEB;"
                "    min-height: 80px;"
                "    padding-left: 20px;"
                "}"
                "QListWidget::item:selected"
                "{"
                "    color: #09262A;"
                "    background: #E0F4EB;"
                "}"
                "QListWidget::item:selected:!active"
                "{"
                "    background-color: transparent ;"
                "}"
    );

}
