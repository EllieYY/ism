#include "CardReadWidget.h"
#include "ui_CardReadWidget.h"
#include "ReaderLib.h"
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
    // TODO: 读取卡信息: 新的接口暂未提供
    BYTE  anti = 0x01;

    // 票卡信息获取
    int ret = readTicketInfo(anti);
    if (ret != 0x00) {
        QString errMsg = DataCenter::getThis()->getReaderErrorStr(ret);
        MyHelper::ShowMessageBoxInfo(QString("票卡信息获取失败[%1]:%2，如有疑问，请联系工作人员。")
                                     .arg(ret).arg(errMsg));
        initReadState();
        return;
    }

    int cardType = DataCenter::getThis()->getTicketBasicInfo()->typeNum();
    if (cardType == UL_CARD) {
        initReadState();
        ui->readerInfoLabel->setText("票卡读取完成……");

        delayMSec(500);
        emit readOk(m_ticketWidgetId);

        close();
        return;
    }


    // 票卡交易历史
    HISTORY_RESP cardHistory = {0};
    BYTE hisRet = readCardHistory(anti, &cardHistory);
    if (hisRet == 0x00) {
        // 交易条数
        QList<TransactionInfo*> transList;
        int count = cardHistory.HistoryCount;
        for (int i = 0; i < count; i++) {
            CARD_HISTORY item = cardHistory.HistoryInfo[i];
            // 交易序号 | 交易时间 | 交易金额（分） | 交易类型 | 终端SAM卡号
            bool ok;
//            long serialNum = QByteArray((char*)item.TradeSerialNum, 2).toHex().toLong(&ok, 16);
            QString time = QByteArray((char*)item.TradeDate, 7).toHex();
            long amount = QByteArray((char*)item.TradeAmount, 4).toHex().toLong(&ok, 16);
            int type = item.TradeType;
            QString devCode = QByteArray((char*)item.PsamID, 6).toHex().toUpper();

            transList.append(new TransactionInfo(time, type, amount, devCode));
        }

        DataCenter::getThis()->setTransInfoList(transList);

    } else {
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
    // TODO:
    // TODO:test.
    delayMSec(1000);
    ui->readBtn->setDisabled(false);
    ui->readerInfoLabel->setText("票卡读取完成……");

    delayMSec(500);

    emit readOk(m_ticketWidgetId);
    close();
}

BYTE CardReadWidget::readTicketInfo(BYTE anti)
{
    BYTE zone = DataCenter::getThis()->isPayZone() ? PAY : FREE;
    ANALYSECARD_RESP analyseInfo = {0};
    BYTE ret = cardAnalyse(anti,zone, &analyseInfo);
    if (ret != 0) {
        return ret;
    }

    BYTE type = analyseInfo.ICType;
    switch(type) {
    case UL_CARD:
        break;
    case METRO_CARD:
        break;
    case OCT_CARD:
        readOCTTicketInfo(type, analyseInfo.CardInfo);
        break;
    default:
        break;
    }

    return ret;
}

//void CardReadWidget::readULTicketInfo(BYTE* array)
//{
//    ULAnalyseOut *getByte = (ULAnalyseOut*)array;

//}

//void CardReadWidget::readMetroTicketInfo(BYTE* array)
//{

//}

void CardReadWidget::readOCTTicketInfo(BYTE cardType, BYTE* array)
{
    OCTAnalyseOut* info = (OCTAnalyseOut*)array;
    // 卡类型 | 逻辑卡号 | 发卡时间 | 有效期 | 卡状态 | 旅程状态 | 余额
    QString type = DataCenter::getThis()->getTicketTypeString(UL_CARD);
    QString number = QByteArray((char*)info->logicID, 8).toHex().toUpper();
    QString startDate = QByteArray((char*)info->appStartDate, 4).toHex();
    QString validDate = QByteArray((char*)info->appAvailDate, 4).toHex();

    int state = info->StatusFlag;
    int tripState = info->StatusFlag;   // TODO:旅程状态待定
    bool ok;
    long balance = QByteArray((char*)info->balance, 4).toHex().toLong(&ok, 16);

    TicketBasicInfo* ticket = new TicketBasicInfo(
                cardType, type, number, startDate, validDate, state, tripState, balance);

    DataCenter::getThis()->setTicketBasicInfo(ticket);
}


void CardReadWidget::delayMSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);

    while( QTime::currentTime() < _Timer )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


