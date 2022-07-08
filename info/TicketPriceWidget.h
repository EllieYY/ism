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

    void onPriceRecv(double price);

    bool showData();

private:
    void setStyle();
    void init();

    void onEnStationSelect();
    void onExStationSelect();
    void onStationSelected(QString lineName, QString stationName, QString stationCode);
    void onPriceCal();


private:
    int m_curBtn;    // 0-en  1-ex

signals:
    void selectStation();    // 选择车站

private:
    Ui::TicketPriceWidget *ui;
};

#endif // TICKETPRICEWIDGET_H
