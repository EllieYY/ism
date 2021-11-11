#include "QrReregisterWidget.h"
#include "ui_QrReregisterWidget.h"
#include "CommonHead.h"
#include "DataCenter.h"
#include "QRCodeInfo.h"

QrReregisterWidget::QrReregisterWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::QrReregisterWidget)
{
    ui->setupUi(this);

    init();

#if IS_TEST_MODE
    setTestData();
#endif

}

QrReregisterWidget::~QrReregisterWidget()
{
    delete ui;
}

bool QrReregisterWidget::showData()
{
//    QRCodeInfo* info = DataCenter::getThis()->getQrCodeInfo();

//    ui->lineEdit1->setText(info->tips());

//    // TODO:类型转换
//    ui->lineEdit2->setText(QString::number(info->channel()));

//    ui->lineEdit3->setText(info->enterStation());
//    ui->lineEdit4->setText(info->enterTime().toString("yyyy-MM-dd HH:mm:ss"));
//    ui->lineEdit5->setText(info->exitStation());

    // TODO:
//    ui->lineEdit8->setText();



    return true;
}



void QrReregisterWidget::init()
{
    setStyle();
}


void QrReregisterWidget::setStyle()
{
    ui->lineEdit1->setDisabled(true);
    ui->lineEdit2->setDisabled(true);
    ui->lineEdit3->setDisabled(true);
    ui->lineEdit4->setDisabled(true);
    ui->lineEdit5->setDisabled(true);
}

void QrReregisterWidget::setTestData()
{
//    QString tips = "提示信息";
//    QDateTime enterTime = QDateTime::currentDateTime().addDays(-3);

//    QRCodeInfo* info = new QRCodeInfo(tips, 1, "地铁大厦",enterTime,"八一广场", true);
//    DataCenter::getThis()->setQrCodeInfo(info);
}
