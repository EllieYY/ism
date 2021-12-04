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
    // 系统运行模式显示
    int netState = DataCenter::getThis()->getNetState();
    BYTE mode = DataCenter::getThis()->getStationMode();
    QString styleSheet = "color: white;";
    if (mode != 0) {
        styleSheet = "color: red;";
    }

    QString modeStr = getModeStr(mode, netState);
    ui->stateLabel->setText(modeStr);

    // 当前时间
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDateStr = currentDateTime.toString("yyyy.MM.dd hh:mm ddd");
    ui->dateLabel->setText(currentDateStr);

    return true;
}


void TitleBar::setTestData()
{
//    DataCenter::getThis()->setServiceState(0);
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

QString TitleBar::getModeStr(BYTE mode, int netState)
{
    // 第一册5.8：故障期间，显示列车运行模式
//    Bit 0	紧急模式             1：生效，0:无效
//    Bit 1	进站免检模式         1：生效，0:无效
//    Bit 2	日期免检模式         1：生效，0:无效
//    Bit 3	时间免检模式         1：生效，0:无效
//    Bit 4	列车故障模式         1：生效，0:无效
//    Bit 5	车费免检模式         1：生效，0:无效
//    Bit 6 ~ 15	[未定义]

    if (netState != 0) {
        return "离线服务";
    }

    QString info = "正常服务";
    if (mode == 0x00) {
        info = "正常服务";
    } else if (mode == 0x01) {
        info = "紧急模式";
    } else if (mode == 0x10) {
        info = "列车故障模式";
    } else {
        info = "降级运行模式";
    }

    return info;
}

void TitleBar::secEvent()
{
    showData();
}
