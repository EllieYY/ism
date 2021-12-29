/* ISMFrame:
 * 1、提供秒驱动
 * 2、窗口注册及管理
 *
 *
*/


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
#include "MyHelper.h"
#include "StationSelectWidget.h"
#include <QGuiApplication>
#include "HttpTool.h"
#include "ASRHttpTool.h"
#include "SettingCenter.h"
#include "LoginDlg.h"
#include "LogoutDlg.h"
#include "TestWidget.h"
#include <QDebug>

#include "DeviceManager.h"
#include "CompensationFareWidget.h"
#include "CardReadWidget.h"

#include "ReaderWorker.h"
#include "CashboxWorker.h"

ISMFrame::ISMFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ISMFrame)
{
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    this->showFullScreen();
    this->showMaximized();

    ui->setupUi(this);
    init();
}

ISMFrame::~ISMFrame()
{
    if (m_deviceThread->isRunning()) {
        m_deviceThread->quit();
        m_deviceThread->wait();
        m_deviceThread->deleteLater();
    }

    if (m_readerThread->isRunning()) {
        m_readerThread->quit();
        m_readerThread->wait();
        m_readerThread->deleteLater();
    }

    if (m_cashboxThread->isRunning()) {
        m_cashboxThread->quit();
        m_cashboxThread->wait();
        m_cashboxThread->deleteLater();
    }

    if(DataCenter::getThis() != NULL) {
        delete DataCenter::getThis();
    }

    delete ui;
}

void ISMFrame::login()
{
    this->hide();
    m_loginDlg->initShow();
}

void ISMFrame::logout()
{
    // 签退
    LogoutDlg* dlg = new LogoutDlg(this);
    connect(dlg, &LogoutDlg::logoutOk, this, &ISMFrame::login);
    MyHelper::FormInCenter(dlg);
    dlg->show();
}

void ISMFrame::init()
{
    qDebug() << "main thread: " <<  QThread::currentThreadId();

    // 数据中心初始化
    DataCenter* data = new DataCenter(this);
//    DataCenter* data = DataCenter::getThis();
    data->init();
    connect(data, &DataCenter::sigSerivceOff, this, &ISMFrame::onServiceOff);

    // 设备初始化
    initDevice();

    // 窗口初始化
    initWgt();

    // 页面更新时钟初始化
    initTimer();
}

void ISMFrame::initShow()
{
    this->showMaximized();
}

void ISMFrame::initDevice()
{
    // 设备管理器初始化
//    m_deviceThread = new QThread();
//    m_deviceManager = new DeviceManager();
//    connect(this, &ISMFrame::initDeviceInThread, m_deviceManager, &DeviceManager::initDevice);
//    connect(this, &ISMFrame::deviceUpdate, m_deviceManager, &DeviceManager::onDeviceUpdate);

//    DataCenter* data = DataCenter::getThis();
//    connect(data, &DataCenter::sigReaderReset, m_deviceManager, &DeviceManager::onReaderReset);
//    connect(data, &DataCenter::sigCashboxReset, m_deviceManager, &DeviceManager::onCashboxReset);
//    m_deviceManager->startDeviceTimer();
//    m_deviceManager->moveToThread(m_deviceThread);
//    m_deviceThread->start();

    //=============================================
    DataCenter* data = DataCenter::getThis();

    // 读写器和钱箱分开两个线程处理
    m_readerThread = new QThread();
    m_readerMng = new ReaderWorker();
    connect(this, &ISMFrame::initDeviceInThread, m_readerMng, &ReaderWorker::onResetDevice);
    connect(this, &ISMFrame::deviceUpdate, m_readerMng, &ReaderWorker::onDeviceUpdate);
    connect(data, &DataCenter::sigReaderReset, m_readerMng, &ReaderWorker::onResetDevice);
    m_readerMng->moveToThread(m_readerThread);
    m_readerThread->start();

    m_cashboxThread = new QThread();
    m_cashboxMng = new CashboxWorker();
    connect(this, &ISMFrame::initDeviceInThread, m_cashboxMng, &CashboxWorker::onInitDevice);
    connect(data, &DataCenter::sigCashboxReset, m_cashboxMng, &CashboxWorker::onCashboxReset);
    m_cashboxMng->moveToThread(m_cashboxThread);
    m_cashboxThread->start();

//    // TODO:
//    m_deviceCount = 1;

    emit initDeviceInThread();
}

void ISMFrame::initTimer()
{
//    qDebug() << "ISMFrame::initTimer";
    m_oldtime = QDateTime::currentDateTime();
    m_time	  = new QTimer(this);
    connect(m_time, &QTimer::timeout, this, &ISMFrame::onTimer);
    m_time->start(200);
}


// 时间取绝对值，考虑时间同步时，会设置早于当前时间的时间值
void ISMFrame::onTimer()
{
    QDateTime time = QDateTime::currentDateTime();
    long interval = abs(m_oldtime.secsTo(time));

    if (interval > 0)
    {
        m_oldtime = time;
        secEvent();
    }
}

//# 秒驱动
void ISMFrame::secEvent()
{
    StatusBar::getThis()->secEvent();
    TitleBar::getThis()->secEvent();

    WidgetMng::getThis()->secEvent();    // 子窗口的秒驱动

    DataCenter::getThis()->secEvent();

//    if (m_deviceCount > 0) {
//        if (m_deviceCount++ > 5) {
//            m_deviceThread->requestInterruption();
//            m_deviceCount = 0;
//        }
//        qDebug() << "device count = " << m_deviceCount;
//    }
}

void ISMFrame::initWgt()
{
    //# 页面管理员
    WidgetMng* mng = new WidgetMng();

    // 控制读写器停止工作
//    connect(mng, &WidgetMng::stopReadingTicket, m_deviceManager, &DeviceManager::setOnReading);
    connect(mng, &WidgetMng::stopReadingTicket, m_readerMng, &ReaderWorker::onReading);

    // 现金缴费窗口：窗口生成线程要跟设备管理线程属于同一个，否则信号槽连接不通
    m_fareWidget = new CompensationFareWidget();
//    connect(m_fareWidget, &CompensationFareWidget::startChecking, m_deviceManager, &DeviceManager::onCheckingCashbox);
//    connect(m_fareWidget, &CompensationFareWidget::stopReading, m_deviceManager, &DeviceManager::setOnReading);
//    connect(m_fareWidget, &CompensationFareWidget::sigCashboxIn, m_deviceManager, &DeviceManager::onCashboxIn);
//    connect(m_deviceManager, &DeviceManager::receiveOk, m_fareWidget, &CompensationFareWidget::onAutoStopPaying);
//    connect(m_deviceManager, &DeviceManager::timeoutChecking, m_fareWidget, &CompensationFareWidget::onStopPaying);
//    connect(m_deviceManager, &DeviceManager::checkState, m_fareWidget, &CompensationFareWidget::showCheckState);

    connect(m_fareWidget, &CompensationFareWidget::stopReading, m_readerMng, &ReaderWorker::onReading);

    connect(m_fareWidget, &CompensationFareWidget::startChecking, m_cashboxMng, &CashboxWorker::onCheckingCashbox);
    connect(m_fareWidget, &CompensationFareWidget::sigCashboxIn, m_cashboxMng, &CashboxWorker::onCashboxIn);
    connect(m_cashboxMng, &CashboxWorker::receiveOk, m_fareWidget, &CompensationFareWidget::onAutoStopPaying);
    connect(m_cashboxMng, &CashboxWorker::timeoutChecking, m_fareWidget, &CompensationFareWidget::onStopPaying);
    connect(m_cashboxMng, &CashboxWorker::checkState, m_fareWidget, &CompensationFareWidget::showCheckState);
    m_fareWidget->hide();


    //# 固定页面
    TitleBar* titleBar = new TitleBar(this);
    StatusBar* statusBar = new StatusBar(this);
    connect(titleBar, &TitleBar::logout, this, &ISMFrame::logout);

    //# 窗口注册
    QHBoxLayout *layoutWnd = new QHBoxLayout();
    layoutWnd->setSpacing(0);
    layoutWnd->setMargin(0);

    //TODO: 测试页面：
//    TestWidget* testWidget = new TestWidget(this);
//    registerWidget(layoutWnd, testWidget, TEST_DLG, false);
//    connect(testWidget, &TestWidget::onCashboxChecking, m_deviceManager, &DeviceManager::onCheckingCashbox);
//    connect(testWidget, &TestWidget::onReader, m_readerMng, &ReaderManager::onStartDoingSomething);

    registerWidget(layoutWnd, new MainWidget(this), MAIN_DLG, true);

    TicketMainWidget* ticketMainWgt = new TicketMainWidget(this);
//    ticketMainWgt->setDeviceManager(m_deviceManager);
    ticketMainWgt->setDeviceManager(m_readerMng);
    registerWidget(layoutWnd, ticketMainWgt, CARD_DLG, false);

    registerWidget(layoutWnd, new InquiryMainWidget(this), INQUIRY_DLG, false);      // 2021-10-20
    registerWidget(layoutWnd, new InfoMainWidget(this), INFO_DLG, false);
    registerWidget(layoutWnd, new GuideMainWidget(this), GUID_DLG, false);

    // 票卡 -- 暂时移除购票和退款功能（PaymentWidget是二级页面），2021-09-26
//    registerWidget(layoutWnd, new TicketTransactionWidget(this), PURCHASE_DLG, false);
//    registerWidget(layoutWnd, new RefundWidget(this), REFUND_DLG, false);
    registerWidget(layoutWnd, new TicketQueryWidget(this), QUERY_DLG, false);

    TicketReregisterWidget* registWidget = new TicketReregisterWidget(this);
    registWidget->setFareWidget(m_fareWidget);
    registerWidget(layoutWnd, registWidget, REREGISTER_DLG, false);
//    registerWidget(layoutWnd, new PaymentWidget(this), PAYMENT_DLG, false);

//    // 二维码 -- 暂时移除该功能 {Ellie 2021-09-26}  -- 会议定稿：互联网支付相关业务去掉 20211012
//    registerWidget(layoutWnd, new QrCodeMainWidget(this), QRCODE_DLG, false);
//    registerWidget(layoutWnd, new QrQueryWidget(this), QR_QUERY_DLG, false);
//    registerWidget(layoutWnd, new QrReregisterWidget(this), QR_REREGISTER_DLG, false);

    // 信息查询
    registerWidget(layoutWnd, new MapWidget(this), MAP_DLG, false);
    registerWidget(layoutWnd, new LineWidget(this), LINE_DLG, false);
    registerWidget(layoutWnd, new TicketPriceWidget(this), PRICE_DLG, false);
    registerWidget(layoutWnd, new MetroInterchangeWidget(this), TRANSFER_DLG, false);
    registerWidget(layoutWnd, new TimeTableWidget(this), TIME_DLG, false);
    registerWidget(layoutWnd, new MetroPeripheralWidget(this), PERIPHERY_DLG, false);

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
//    WidgetMng::getThis()->showWidget(MAIN_DLG);

    statusBar->show();

    m_loginDlg = new LoginDlg();
    connect(m_loginDlg, &LoginDlg::loginOk, this, &ISMFrame::initShow);
    m_loginDlg->hide();
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
}


// 运营结束相关处理
void ISMFrame::onServiceOff()
{
    qDebug() << "onServiceOff: " << QThread::currentThreadId();
//    // 运营日结束时，自动签退 -- 只执行一次
//    bool isLogin = DataCenter::getThis()->getIsLogin();
//    bool isServiceOff = DataCenter::getThis()->getServiceOff();
//    if (isLogin && isServiceOff) {
//        // 自动签退
//        if (DataCenter::getThis()->autoLogout()) {
//            // 显示签到页面
//            login();
//        }
//    }

    // 设备参数和程序升级并重新初始化
//    if (m_deviceManager != NULL) {
//        emit deviceUpdate();
//    }
    if (m_readerMng != NULL) {
        emit deviceUpdate();
    }
}
