#ifndef TICKETQUERYWIDGET_H
#define TICKETQUERYWIDGET_H

#include <QWidget>
#include "WidgetBase.h"


namespace Ui {
class TicketQueryWidget;
}

class CardReadWidget;
class TicketQueryWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit TicketQueryWidget(QWidget *parent = nullptr);
    ~TicketQueryWidget();

    void secEvent();

private:
    bool showData();
    void showTicketBasicInfo();          // 基础信息
    void showTicketTransInfo();          // 交易信息

    void setStyle();
    void init();
    void setTestData();

private:
    Ui::TicketQueryWidget *ui;
};

#endif // TICKETQUERYWIDGET_H
