#ifndef TICKETTRANSACTIONWIDGET_H
#define TICKETTRANSACTIONWIDGET_H

#include <QWidget>
#include "WidgetBase.h"


namespace Ui {
class TicketTransactionWidget;
}

class TicketTransactionWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit TicketTransactionWidget(QWidget *parent = nullptr);
    ~TicketTransactionWidget();

private:
    void init();
    void onPurchase();
    void onStationSelected(QString lineName, QString stationName);

private:
    Ui::TicketTransactionWidget *ui;
};

#endif // TICKETTRANSACTIONWIDGET_H
