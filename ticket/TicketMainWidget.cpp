#include "TicketMainWidget.h"
#include "ui_TicketMainWidget.h"
#include "WidgetMng.h"
#include "CommonHead.h"
#include "CardReadWidget.h"
#include "DeviceManager.h"
#include "ReaderWorker.h"

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

void TicketMainWidget::secEvent()
{
    if (m_cardReadWidget->isVisible()) {
        m_cardReadWidget->secEvent();
    }
}

//void TicketMainWidget::setDeviceManager(DeviceManager *manager)
//{
//    connect(manager, &DeviceManager::ticketRead, m_cardReadWidget, &CardReadWidget::updateReadingState);
//    connect(m_cardReadWidget, &CardReadWidget::doReading, manager, &DeviceManager::setOnReading);
//}

void TicketMainWidget::setDeviceManager(ReaderWorker *manager)
{
    connect(manager, &ReaderWorker::ticketRead, m_cardReadWidget, &CardReadWidget::updateReadingState);
    connect(m_cardReadWidget, &CardReadWidget::doReading, manager, &ReaderWorker::onReading);
}

const int  TICKET_WIDGET_NUM = 2;
void TicketMainWidget::onBtn(int btnId)
{

    int widgetIdx[TICKET_WIDGET_NUM] =
    {
//        PURCHASE_DLG,                   //# 购票界面
//        REFUND_DLG,                //# 退款
        QUERY_DLG,                 //# 票卡查询
        REREGISTER_DLG                   //# 票卡补登
    };

    if  (btnId >= 0 && btnId < TICKET_WIDGET_NUM)
    {
        // TODO:test code 2022-04-20
//        WidgetMng::getThis()->showWidget(widgetIdx[btnId]);

        // TODO:打开注释 2022-04-20
        m_cardReadWidget->show();
        m_cardReadWidget->readCard(widgetIdx[btnId]);
    }
}

void TicketMainWidget::onDataOk(int widgetId)
{
    if (widgetId < 0) {
        return;
    }
    WidgetMng::getThis()->showWidget(widgetId);
}

//void TicketMainWidget::setCardReadWidget(CardReadWidget *cardReadWidget)
//{
//    m_cardReadWidget = cardReadWidget;
//    connect(m_cardReadWidget, &CardReadWidget::readOk,
//            this, &TicketMainWidget::onDataOk);

//}


void TicketMainWidget::init()
{
    ui->purchaseBtn->setText("单程票购买");
    ui->refundBtn->setText("单程票退款");
    ui->queryBtn->setText("票卡查询");
    ui->reregisterBtn->setText("票卡更新");

    ui->purchaseBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->refundBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->queryBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->reregisterBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // 2021-09-26 隐藏单程票购买功能和退款功能入口
    ui->purchaseBtn->hide();
    ui->refundBtn->hide();
    ui->horizontalSpacer_3->changeSize(0, 1, QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->horizontalSpacer_4->changeSize(0, 1, QSizePolicy::Fixed, QSizePolicy::Fixed);

    QList<QToolButton*> btnList;
//    btnList.append(ui->purchaseBtn);
//    btnList.append(ui->refundBtn);
    btnList.append(ui->queryBtn);
    btnList.append(ui->reregisterBtn);

    m_buttonGroup = new QButtonGroup(this);
    int id = 0;
    for(QToolButton* btn : btnList) {
        m_buttonGroup->addButton(btn, id++);
    }

    connect(m_buttonGroup, &QButtonGroup::idClicked, this, &TicketMainWidget::onBtn);


    // 读卡提示页面
    m_cardReadWidget = new CardReadWidget();
    m_cardReadWidget->hide();
    connect(m_cardReadWidget, &CardReadWidget::readOk, this, &TicketMainWidget::onDataOk);
}
