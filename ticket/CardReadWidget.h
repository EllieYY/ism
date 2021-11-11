#ifndef CARDREADWIDGET_H
#define CARDREADWIDGET_H

#include <QWidget>
#include "WidgetBase.h"
#include "logger.h"
#include "propertyconfigurator.h"

namespace Ui {
class CardReadWidget;
}

class CardReadWidget : public WidgetBase
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit CardReadWidget(QWidget *parent = nullptr);
    ~CardReadWidget();

    void secEvent();
    void readCard(int id);

signals:
    void readOk(int widgetId);

private:
    void init();
    void initReadState();    // 恢复至重新读卡准备状态
    void setStyle();

    void onClose();          // 取消读卡
    void onRefundTicket();   // 重新读卡

    void updateStatus(QString info);

    void delayMSec(unsigned int msec);

    void readTransactionInfo();     // 读取交易信息
    void readReregisterInfo();      // 读取票卡更新信息

    // 票卡详情
    BYTE readTicketInfo(BYTE anti);    // 票卡信息读取
//    void readULTicketInfo(BYTE* array);
//    void readMetroTicketInfo(BYTE* array);
    void readOCTTicketInfo(BYTE type, BYTE* array);

private:
    int  m_ticketWidgetId;
    bool m_isReading;           // 是否正在读取票卡信息

private:
    Ui::CardReadWidget *ui;
};

#endif // CARDREADWIDGET_H
