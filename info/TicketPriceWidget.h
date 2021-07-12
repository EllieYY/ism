#ifndef TICKETPRICEWIDGET_H
#define TICKETPRICEWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class TicketPriceWidget;
}

class TicketPriceWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit TicketPriceWidget(QWidget *parent = nullptr);
    ~TicketPriceWidget();

private:
    void setStyle();
    void init();

    void onEnStationSelect();
    void onExStationSelect();
    void onStationSelected(QString lineName, QString stationName);

private:
    int m_curBtn;    // 0-en  1-ex

private:
    Ui::TicketPriceWidget *ui;
};

#endif // TICKETPRICEWIDGET_H
