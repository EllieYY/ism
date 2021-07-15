#include "TicketMainWidget.h"
#include "ui_TicketMainWidget.h"
#include "WidgetMng.h"
#include "CommonHead.h"

TicketMainWidget::TicketMainWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::TicketMainWidget)
{
    ui->setupUi(this);

    init();
}

TicketMainWidget::~TicketMainWidget()
{
    delete ui;
}


void TicketMainWidget::onBtn(int btnId)
{
    int widgetIdx[4] =
    {
        PURCHASE_DLG,                   //# 购票界面
        REFUND_DLG,                //# 退款
        QUERY_DLG,                 //# 票卡查询
        REREGISTER_DLG                   //# 票卡补登
    };

    if  (btnId >= 0 && btnId < 4)
    {
        WidgetMng::getThis()->showWidget(widgetIdx[btnId]);
    }
}

void TicketMainWidget::init()
{
    m_buttonGroup = new QButtonGroup(this);
    registerBtn(ui->purchaseBtn, 0);
    registerBtn(ui->refundBtn, 1);
    registerBtn(ui->queryBtn, 2);
    registerBtn(ui->reregisterBtn, 3);

    ui->purchaseBtn->setText("单程票购买");
    ui->refundBtn->setText("单程票退款");
    ui->queryBtn->setText("票卡查询");
    ui->reregisterBtn->setText("票卡补登");

    ui->purchaseBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->refundBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->queryBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->reregisterBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);


    connect(m_buttonGroup, &QButtonGroup::idClicked, this, &TicketMainWidget::onBtn);

}

void TicketMainWidget::registerBtn(QToolButton *btn, int id)
{
    this->m_buttonGroup->addButton(btn, id);
}
