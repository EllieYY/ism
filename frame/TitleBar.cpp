#include "TitleBar.h"
#include "ui_TitleBar.h"
#include "DataCenter.h"


static TitleBar* afx_this = NULL;
TitleBar::TitleBar(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::TitleBar)
{
    ui->setupUi(this);
    afx_this = this;

#if IS_TEST_MODE
    setTestData();
#endif
}

TitleBar::~TitleBar()
{
    delete ui;
}

TitleBar *TitleBar::getThis()
{
    return afx_this;
}


bool TitleBar::showData()
{
    // 服务状态
    int serviceState = DataCenter::getThis()->getServiceState();
    QString serviceStateStr = "暂停服务";
    switch(serviceState) {
    case 0:
        serviceStateStr = "正常服务";
        break;
    case 1:
        serviceStateStr = "服务异常";
    default:
        break;
    }
    ui->stateLabel->setText(serviceStateStr);

    // 当前时间
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDateStr = currentDateTime.toString("yyyy.MM.dd hh:mm ddd");
    ui->dateLabel->setText(currentDateStr);

    return true;
}


void TitleBar::setTestData()
{
    DataCenter::getThis()->setServiceState(0);
}

void TitleBar::secEvent()
{
    showData();
}
