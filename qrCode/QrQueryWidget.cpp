#include "QrQueryWidget.h"
#include "ui_QrQueryWidget.h"

QrQueryWidget::QrQueryWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::QrQueryWidget)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setFrameShape(QFrame::NoFrame);

    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_2->verticalHeader()->setVisible(false);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->setFrameShape(QFrame::NoFrame);
}

QrQueryWidget::~QrQueryWidget()
{
    delete ui;
}
