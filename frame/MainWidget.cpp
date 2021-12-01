#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "CommonHead.h"
#include "WidgetMng.h"
#include "DataCenter.h"

MainWidget::MainWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    init();

#if IS_TEST_MODE
    setTestData();
#endif
}

MainWidget::~MainWidget()
{
    delete ui;
}

const int  MAIN_WIDGET_NUM = 4;
void MainWidget::onBtn(int btnId)
{
    int widgetIdx[MAIN_WIDGET_NUM] =
    {
        CARD_DLG,                   //# 票卡处理界面
//        QRCODE_DLG,                 //# 二维码
        INQUIRY_DLG,                //# 智能问询
        INFO_DLG,                   //# 信息查询
//        GUID_DLG                    //# 服务指南
        TEST_DLG
    };

    if  (btnId >= 0 && btnId < MAIN_WIDGET_NUM)
    {
        WidgetMng::getThis()->showWidget(widgetIdx[btnId]);
    }
}

void MainWidget::init()
{
    ui->cardBtn->setText("票卡处理");
    ui->QRCodeBtn->setText("二维码");
    ui->inquiryBtn->setText("智能问询");
    ui->infoBtn->setText("信息查询");
    ui->guidBtn->setText("服务指南");

    ui->cardBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->QRCodeBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->inquiryBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->infoBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->guidBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // 2021-09-26 隐藏二维码功能入口
    ui->QRCodeBtn->hide();
    ui->horizontalSpacer_4->changeSize(0, 1, QSizePolicy::Fixed, QSizePolicy::Fixed);

    QList<QToolButton*> btnList;
    btnList.append(ui->cardBtn);
//    btnList.append(ui->QRCodeBtn);
    btnList.append(ui->inquiryBtn);
    btnList.append(ui->infoBtn);
    btnList.append(ui->guidBtn);

    m_buttonGroup = new QButtonGroup(this);
    int id = 0;
    for(QToolButton* btn : btnList) {
        m_buttonGroup->addButton(btn, id++);
    }

    connect(m_buttonGroup, &QButtonGroup::idClicked, this, &MainWidget::onBtn);
}

void MainWidget::setTestData()
{


}

void MainWidget::secEvent()
{
    // AFC状态控制
    if (m_dataUpdateNum[AFC_ONLINE_STATE_ID])
    {
        int afcState = DataCenter::getThis()->getServiceState();
        if (afcState == 0) {
            ui->cardBtn->setDisabled(false);
        } else {
            ui->cardBtn->setDisabled(true);
        }

        m_dataUpdateNum[AFC_ONLINE_STATE_ID] = false;
    }
}
