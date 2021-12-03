#include "TitleBar.h"
#include "ui_TitleBar.h"
#include "DataCenter.h"
#include "WidgetMng.h"


static TitleBar* afx_this = NULL;
TitleBar::TitleBar(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::TitleBar)
{
    ui->setupUi(this);
    afx_this = this;

    connect(ui->tLogoutBtn, &QPushButton::clicked, this, &TitleBar::onLogout);
    connect(WidgetMng::getThis(), &WidgetMng::returnCtl, this, &TitleBar::showLogoutBtn);

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
//    // 系统运行模式设置
//    if (m_dataUpdateNum[STATION_MODE])
//    {
//        int afcState = DataCenter::getThis()->getServiceState();
//        if (afcState == 0) {
//            ui->cardBtn->setDisabled(false);
//        } else {
//            ui->cardBtn->setDisabled(true);
//        }

//        m_dataUpdateNum[STATION_MODE] = false;
//    }

    if (!m_dataUpdateNum[AFC_ONLINE_STATE_ID])
    {
        return true;
    }
    m_dataUpdateNum[AFC_ONLINE_STATE_ID] = false;

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

void TitleBar::showLogoutBtn(bool isMainWnd)
{
    ui->tLogoutBtn->setVisible(isMainWnd);
}

void TitleBar::onLogout()
{
    // TODO:用户名+密码验证
    emit logout();
}

void TitleBar::secEvent()
{
//    qDebug() << "TitleBar::secEvent()";
    // 当前时间
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDateStr = currentDateTime.toString("yyyy.MM.dd hh:mm:ss ddd");

//    qDebug() << "current time:" << currentDateStr;

    ui->dateLabel->setText(currentDateStr);

    showData();
}
