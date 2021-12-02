#include "CardReadWidget.h"
#include "ui_CardReadWidget.h"
#include "NC_ReaderLib.h"
#include "DataCenter.h"
#include "MyHelper.h"
#include "TransactionInfo.h"
#include "TicketBasicInfo.h"

CardReadWidget::CardReadWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::CardReadWidget)
{
    ui->setupUi(this);
    init();
}

CardReadWidget::~CardReadWidget()
{
    delete ui;
}

void CardReadWidget::readCard(int id)
{
    m_ticketWidgetId = id;
    onRefundTicket();
}

void CardReadWidget::secEvent()
{
    if (!m_isReading) {
        return;
    }

    if (QUERY_DLG == m_ticketWidgetId) {
        ui->readerInfoLabel->setText("正在读取票卡基本信息……");
        this->readTransactionInfo();
    } else if (REREGISTER_DLG == m_ticketWidgetId) {
        ui->readerInfoLabel->setText("正在读取票卡补登信息……");
        this->readReregisterInfo();
    }
}


void CardReadWidget::init()
{
    m_isReading = false;
    m_ticketWidgetId = QUERY_DLG;

    setStyle();

    connect(ui->readCancleBtn, &QPushButton::clicked, this, &CardReadWidget::onClose);
    connect(ui->readBtn, &QPushButton::clicked, this, &CardReadWidget::onRefundTicket);
}

void CardReadWidget::initReadState()
{
    m_isReading = false;
    ui->readBtn->setDisabled(false);
}

void CardReadWidget::setStyle()
{
    // 透明背景设置
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QScreen *screen = QGuiApplication::primaryScreen();   //获取当前屏幕的大小
    QRect mm = screen->availableGeometry();
    int screenWidth = mm.width();
    int screenHeight = mm.height();
    this->setFixedSize(screenWidth, screenHeight);
}

void CardReadWidget::onClose()
{
    m_isReading = false;
    ui->readBtn->setDisabled(false);
    ui->readerInfoLabel->setText("取消读卡。");
    this->close();
}

void CardReadWidget::onRefundTicket()
{
    m_isReading = true;
    ui->readBtn->setDisabled(true);
    ui->readerInfoLabel->setText("正在读卡，请稍后……");
}


void CardReadWidget::readTransactionInfo()
{
    BYTE anti = DataCenter::getThis()->getAntiNo();

    // 票卡信息获取
    int ret = readTicketInfo(anti);
    if (ret != 0x00) {
        ui->readerInfoLabel->setText("票卡信息读取失败");
        QString errMsg = DataCenter::getThis()->getReaderErrorStr(ret);
        MyHelper::ShowMessageBoxInfo(QString("票卡信息获取失败[%1]:%2，请尝试重新读卡，或联系工作人员。")
                                     .arg(ret).arg(errMsg));
        initReadState();
        return;
    }

    // 卡历史信息读取限制：排除token票
    int cardType = DataCenter::getThis()->getTicketBasicInfo()->typeNum();
    if (cardType == UL_CARD) {
        initReadState();
        ui->readerInfoLabel->setText("票卡读取完成……");

//        delayMSec(500);
        emit readOk(m_ticketWidgetId);

        close();
        return;
    }


    // 票卡交易历史
    int hisRet = readHistoryTrade(anti);
    if (hisRet != 0x00) {
        ui->readerInfoLabel->setText("票卡历史交易信息读取失败");
        QString errMsg = DataCenter::getThis()->getReaderErrorStr(hisRet);
        MyHelper::ShowMessageBoxInfo(QString("票卡历史交易获取失败[%1]:%2，如有疑问，请联系工作人员。")
                                     .arg(hisRet).arg(errMsg));
        initReadState();
        return;
    }

    initReadState();
    ui->readerInfoLabel->setText("票卡读取完成……");

    delayMSec(500);
    emit readOk(m_ticketWidgetId);
    close();
}

void CardReadWidget::readReregisterInfo()
{
    BYTE anti = DataCenter::getThis()->getAntiNo();

    // 票卡信息获取
    int ret = readTicketInfo(anti);

////    // TODO: 使用测试数据
//    ret = 0;
//    setTestData();


    if (ret != 0x00) {
        ui->readerInfoLabel->setText("票卡信息读取失败");
        QString errMsg = DataCenter::getThis()->getReaderErrorStr(ret);
        MyHelper::ShowMessageBoxInfo(QString("票卡信息获取失败[%1]:%2，请尝试重新读卡，或联系工作人员。")
                                     .arg(ret).arg(errMsg));
        initReadState();
        return;
    }

    initReadState();
    ui->readerInfoLabel->setText("票卡读取完成……");

    delayMSec(500);

    emit readOk(m_ticketWidgetId);
    close();
}


BYTE CardReadWidget::readTicketInfo(BYTE anti)
{
    BYTE zone = DataCenter::getThis()->isPayZone() ? PAY : FREE;
    ANALYSECARD_RESP analyseInfo = {0};
    BYTE ret = cardAnalyse(anti, zone, &analyseInfo);
    if (ret != 0) {
        return ret;
    }

    // 结果打印
    QByteArray resArr = QByteArray((char*)&analyseInfo, sizeof(ANALYSECARD_RESP));
    QString resStr = resArr.toHex();
    logger()->info("[cardAnalyse] %1", resStr);

    /* 字段解析 ------------*/
    // 卡类型 | 逻辑卡号 | 发卡时间 | 有效期 | 卡状态 | 旅程状态 | 余额
    int typeNum = analyseInfo.ticketType;
    QString type = DataCenter::getThis()->getTicketTypeString(typeNum);
    QString number = QByteArray((char*)analyseInfo.logicID, 10).toHex().toUpper();
    QString startDate = QByteArray((char*)analyseInfo.issueStartDate, 4).toHex();
    QString validDate = QByteArray((char*)analyseInfo.issueOutDate, 4).toHex();

    int state = analyseInfo.ticketStatus;
    int tripState = analyseInfo.ticketStatus;   // TODO:旅程状态待定
    bool ok;
    long balance = QByteArray((char*)analyseInfo.balance, 4).toHex().toLong(&ok, 16);

    // 允许更新 | 卡扣更新 | 更新类型 | 应收费用
    bool isAllowUpdate = analyseInfo.isAllowUpdate;
    bool isAllowOctPay = analyseInfo.isAllowOctPay;
    int updateType = analyseInfo.UpdateType;
    uint amount = analyseInfo.UpdateAmount;

    // 进站车站 | 进站时间 | 出站车站 | 出站时间
    QString enStation = QByteArray((char*)analyseInfo.lastEnrtyStation, 2).toHex().toUpper();
    QString exStation = QByteArray((char*)analyseInfo.lastExitStation, 2).toHex().toUpper();
    QString enTime = QByteArray((char*)analyseInfo.lastEntryTime, 7).toHex();
    QString exTime = QByteArray((char*)analyseInfo.lastExitTime, 7).toHex();

    TicketBasicInfo* ticket = new TicketBasicInfo(
                typeNum, type, number, startDate, validDate, state, tripState, balance);
    ticket->setIcType(analyseInfo.ICType);
    ticket->setErrorCode(analyseInfo.errCode);
    ticket->setIsAllowOctPay(isAllowOctPay);
    ticket->setIsAllowUpdate(isAllowUpdate);
    ticket->setUpdateType(updateType);
    ticket->setEnStationCode(enStation);
    ticket->setEnTime(enTime);
    ticket->setExStationCode(exStation);
    ticket->setExTime(exTime);
    ticket->setUpdateAmount(amount);

    DataCenter::getThis()->setTicketBasicInfo(ticket);

    return ret;
}

BYTE CardReadWidget::readHistoryTrade(BYTE anti)
{
    HISTORY_RESP cardHistory = {0};
    BYTE hisRet = readCardHistory(anti, &cardHistory);
    if (hisRet != 0x00) {
        return hisRet;
    }

    // 结果打印
    QByteArray resArr = QByteArray((char*)&cardHistory, sizeof(HISTORY_RESP));
    QString resStr = resArr.toHex();
    logger()->info("[readCardHistory] %1", resStr);

    // 交易条数
    QList<TransactionInfo*> transList;
    int count = cardHistory.HistoryCount;
    for (int i = 0; i < count; i++) {
        CARD_HISTORY item = cardHistory.HistoryInfo[i];
        // 交易时间 | 交易金额（分） | 交易类型 | 终端SAM卡号
        bool ok;
        QString time = QByteArray((char*)item.TradeDate, 7).toHex();
        long amount = QByteArray((char*)item.TradeAmount, 4).toHex().toLong(&ok, 16);
        int type = item.TradeType;
        QString devCode = QByteArray((char*)item.PsamID, 6).toHex().toUpper();

        transList.append(new TransactionInfo(time, type, amount, devCode));
    }

    DataCenter::getThis()->setTransInfoList(transList);
    return hisRet;
}

void CardReadWidget::setTestData()
{
    int typeNum = 0x80;
    QString type = DataCenter::getThis()->getTicketTypeString(typeNum);
    TicketBasicInfo* ticket = new TicketBasicInfo(
                0x85, type, "30010088562007", "20200901", "20231001", 1, 1, 500);
    ticket->setIsAllowOctPay(false);
    ticket->setIsAllowUpdate(true);
    ticket->setUpdateType(FARE_EX);
    ticket->setEnStationCode("0203");
    ticket->setEnTime("20211115212305");
    ticket->setExStationCode("0306");
    ticket->setExTime("20211115212606");
    ticket->setUpdateAmount(3);

    DataCenter::getThis()->setTicketBasicInfo(ticket);


//    QList<TransactionInfo*> transList;
//    int count = cardHistory.HistoryCount;
//    for (int i = 0; i < count; i++) {
//        CARD_HISTORY item = cardHistory.HistoryInfo[i];
//        // 交易时间 | 交易金额（分） | 交易类型 | 终端SAM卡号
//        bool ok;
//        QString time = QByteArray((char*)item.TradeDate, 7).toHex();
//        long amount = QByteArray((char*)item.TradeAmount, 4).toHex().toLong(&ok, 16);
//        int type = item.TradeType;
//        QString devCode = QByteArray((char*)item.PsamID, 6).toHex().toUpper();

//        transList.append(new TransactionInfo(time, type, amount, devCode));
//    }

//    DataCenter::getThis()->setTransInfoList(transList);
}


void CardReadWidget::delayMSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);

    while( QTime::currentTime() < _Timer )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


