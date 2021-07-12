#include "StatusBar.h"
#include "ui_StatusBar.h"
#include "CommonHead.h"
#include "WidgetMng.h"
#include "DataCenter.h"

static StatusBar* afx_this = NULL;
StatusBar::StatusBar(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::StatusBar)
{
    ui->setupUi(this);
    afx_this = this;

    connect(ui->returnBtn, &QPushButton::clicked,
            WidgetMng::getThis(), &WidgetMng::onReturn);
    connect(ui->homeBtn, &QPushButton::clicked,
            WidgetMng::getThis(), &WidgetMng::onHome);
    connect(WidgetMng::getThis(), &WidgetMng::returnCtl,
            this, &StatusBar::showButtons);



#if IS_TEST_MODE
    setTestData();
#endif
}

StatusBar::~StatusBar()
{
    delete ui;
}

StatusBar *StatusBar::getThis()
{
    return afx_this;
}

void StatusBar::showButtons(bool isMainWnd)
{
    ui->homeBtn->setVisible(!isMainWnd);
    ui->returnBtn->setVisible(!isMainWnd);
}

bool StatusBar::showData()
{
    int netState = DataCenter::getThis()->getNetState();
    QString netStateStr = netState == 0 ? "网络状态：在线":"网络状态：离线";
    ui->netStateLabel->setText(netStateStr);
    ui->siteNameLabel->setText(DataCenter::getThis()->getStationName());

    return true;
}

void StatusBar::setTestData()
{
//    DataCenter::getThis()->setNetState(0);
}

void StatusBar::secEvent()
{
    showData();
}
