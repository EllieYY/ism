#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "CommonHead.h"
#include "WidgetMng.h"

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

void MainWidget::registerBtn(QToolButton *btn, int id)
{
    this->m_buttonGroup->addButton(btn, id);
}

void MainWidget::onBtn(int btnId)
{
    int widgetIdx[5] =
    {
        CARD_DLG,                   //# 票卡处理界面
        QRCODE_DLG,                 //# 二维码
        INQUIRY_DLG,                //# 智能问询
        INFO_DLG,                   //# 信息查询
        GUID_DLG                    //# 服务指南
    };

    if  (btnId >= 0 && btnId < 5)
    {
        WidgetMng::getThis()->showWidget(widgetIdx[btnId]);
    }
}

void MainWidget::init()
{
    m_buttonGroup = new QButtonGroup(this);
    registerBtn(ui->cardBtn, 0);
    registerBtn(ui->QRCodeBtn, 1);
    registerBtn(ui->inquiryBtn, 2);
    registerBtn(ui->infoBtn, 3);
    registerBtn(ui->guidBtn, 4);

    connect(m_buttonGroup, &QButtonGroup::idClicked, this, &MainWidget::onBtn);

}

void MainWidget::setTestData()
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

}
