#include "QrCodeMainWidget.h"
#include "ui_QrCodeMainWidget.h"
#include "CommonHead.h"
#include "WidgetMng.h"

QrCodeMainWidget::QrCodeMainWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::QrCodeMainWidget)
{
    ui->setupUi(this);
    init();


#if IS_TEST_MODE
    setTestData();
#endif
}

QrCodeMainWidget::~QrCodeMainWidget()
{
    delete ui;
}


void QrCodeMainWidget::registerBtn(QToolButton *btn, int id)
{
    this->m_buttonGroup->addButton(btn, id);
}

void QrCodeMainWidget::onBtn(int btnId)
{
    int widgetIdx[2] =
    {
        QR_QUERY_DLG,                   //# 票卡处理界面
        QR_REREGISTER_DLG                 //# 二维码
    };

    if  (btnId >= 0 && btnId < 2)
    {
        WidgetMng::getThis()->showWidget(widgetIdx[btnId]);
    }
}

void QrCodeMainWidget::init()
{
    m_buttonGroup = new QButtonGroup(this);
    registerBtn(ui->codeQueryBtn, 0);
    registerBtn(ui->codeReregisterBtn, 1);

    connect(m_buttonGroup, &QButtonGroup::idClicked, this, &QrCodeMainWidget::onBtn);

}

void QrCodeMainWidget::setTestData()
{

    ui->codeQueryBtn->setText("二维码查询");
    ui->codeReregisterBtn->setText("二维码补登");

    ui->codeQueryBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->codeReregisterBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}
