#include "ISMFrame.h"
#include "ui_ISMFrame.h"
#include "CommonHead.h"
#include "WidgetMng.h"
#include "DataCenter.h"

#include "TitleBar.h"
#include "StatusBar.h"
#include "MainWidget.h"
#include "TicketMainWidget.h"
#include "QrCodeMainWidget.h"
#include "InquiryMainWidget.h"
#include "InfoMainWidget.h"
#include "GuideMainWidget.h"
#include "TicketTransactionWidget.h"
#include "RefundWidget.h"
#include "TicketQueryWidget.h"
#include "TicketReregisterWidget.h"
#include "PaymentWidget.h"
#include "QrQueryWidget.h"
#include "QrReregisterWidget.h"
#include "MapWidget.h"
#include "LineWidget.h"
#include "TicketPriceWidget.h"
#include "MetroInterchangeWidget.h"
#include "MetroPeripheralWidget.h"
#include "TimeTableWidget.h"
#include "myhelper.h"
#include "StationSelectWidget.h"
#include <QGuiApplication>
#include "HttpTool.h"

ISMFrame::ISMFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ISMFrame)
{
    //设置窗体标题栏隐藏
//    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
//    this->showFullScreen();
//    myHelper::FormInCenter(this);

    ui->setupUi(this);
    init();
}

ISMFrame::~ISMFrame()
{
    delete ui;
}

void ISMFrame::init()
{
    initWgt();
    initTimer();
}

void ISMFrame::initTimer()
{
    m_oldtime = QDateTime::currentDateTime();
    m_time	  = new QTimer(this);
    connect(m_time, &QTimer::timeout, this, &ISMFrame::onTimer);
    m_time->start(50);
}

void ISMFrame::onTimer()
{
    QDateTime time = QDateTime::currentDateTime();
    if (m_oldtime.secsTo(time) > 0)
    {
        m_oldtime = time;
        secEvent();
    }
}


void ISMFrame::secEvent()
{
    DataCenter::getThis()->secEvent();
    StatusBar::getThis()->secEvent();
    TitleBar::getThis()->secEvent();
    WidgetMng::getThis()->secEvent();

}

void ISMFrame::initWgt()
{
    //# 页面管理员
    new WidgetMng();

    // 选择车站 - 公用
    m_stationSelectedWidget = new StationSelectWidget();
    m_stationSelectedWidget->hide();

    //# 公用部分
    TitleBar* titleBar = new TitleBar(this);
    StatusBar* statusBar = new StatusBar(this);

    //# 窗口注册
    QHBoxLayout *layoutWnd = new QHBoxLayout();
    layoutWnd->setSpacing(0);
    layoutWnd->setMargin(0);
    registerWidget(layoutWnd, new MainWidget(this), MAIN_DLG, true);
    registerWidget(layoutWnd, new TicketMainWidget(this), CARD_DLG, false);
    registerWidget(layoutWnd, new QrCodeMainWidget(this), QRCODE_DLG, false);

    InquiryMainWidget* inquiryWidget = new InquiryMainWidget(this);
    connect(HttpTool::getThis(), &HttpTool::hotIssuesReceived,
            inquiryWidget, &InquiryMainWidget::onHotIssues);
    connect(HttpTool::getThis(), &HttpTool::answerReceived,
            inquiryWidget, &InquiryMainWidget::onAnswerShow);
    registerWidget(layoutWnd, inquiryWidget, INQUIRY_DLG, false);
    registerWidget(layoutWnd, new InfoMainWidget(this), INFO_DLG, false);
    registerWidget(layoutWnd, new GuideMainWidget(this), GUID_DLG, false);
    // 票卡
    registerWidget(layoutWnd, new TicketTransactionWidget(this), PURCHASE_DLG, false);
    registerWidget(layoutWnd, new RefundWidget(this), REFUND_DLG, false);
    registerWidget(layoutWnd, new TicketQueryWidget(this), QUERY_DLG, false);
    registerWidget(layoutWnd, new TicketReregisterWidget(this), REREGISTER_DLG, false);
    registerWidget(layoutWnd, new PaymentWidget(this), PAYMENT_DLG, false);
    // 二维码
    registerWidget(layoutWnd, new QrQueryWidget(this), QR_QUERY_DLG, false);
    registerWidget(layoutWnd, new QrReregisterWidget(this), QR_REREGISTER_DLG, false);
    // 信息查询
    registerWidget(layoutWnd, new MapWidget(this), MAP_DLG, false);

    LineWidget* lineWidget = new LineWidget(this);
    registerWidget(layoutWnd, lineWidget, LINE_DLG, false);
    TicketPriceWidget* priceWidget = new TicketPriceWidget(this);
    registerWidget(layoutWnd, priceWidget, PRICE_DLG, false);

    registerWidget(layoutWnd, new MetroInterchangeWidget(this), TRANSFER_DLG, false);
    registerWidget(layoutWnd, new TimeTableWidget(this), TIME_DLG, false);
    registerWidget(layoutWnd, new MetroPeripheralWidget(this), PERIPHERY_DLG, false);

    connect(DataCenter::getThis(), &DataCenter::lineReceived,
            lineWidget, &LineWidget::onReadLines);
    connect(HttpTool::getThis(), &HttpTool::priceReceived,
            priceWidget, &TicketPriceWidget::onPriceRecv);

    //# 总体布局
    QVBoxLayout *layoutMain = new QVBoxLayout();
    layoutMain->addWidget(titleBar);         // 插入标题栏
    layoutMain->addLayout(layoutWnd);        // 主窗体
    layoutMain->addWidget(statusBar);		 // 插入状态栏
    layoutMain->setSpacing(0);
    layoutMain->setMargin(0);
    setLayout(layoutMain);

    //# 初始显示状态
    titleBar->show();
    WidgetMng::getThis()->showWidget(MAIN_DLG);
    statusBar->show();
}

/** 窗口管理 */
void ISMFrame::registerWidget(QHBoxLayout *layout, WidgetBase *widget, int widgetId, bool isMain)
{
    if (isMain)
    {
        WidgetMng::getThis()->setMainWidget(widget);
        widget->showWgt();
    }
    else
    {
        widget->hideWgt();
    }
    widget->setId(widgetId);
    WidgetMng::getThis()->insertWidget(widget);

    layout->addWidget(widget);

    // 选择车站
    connect(widget, &WidgetBase::selectStation,
            m_stationSelectedWidget, &StationSelectWidget::showData);
    connect(m_stationSelectedWidget, &StationSelectWidget::stationSelected,
            widget, &WidgetBase::onStationSelected);
}
