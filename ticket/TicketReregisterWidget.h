#ifndef TICKETREREGISTERWIDGET_H
#define TICKETREREGISTERWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class TicketReregisterWidget;
}

class TicketReregisterWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit TicketReregisterWidget(QWidget *parent = nullptr);
    ~TicketReregisterWidget();

private:
    bool showData();
    void setStyle();

    void init();
    void setTestData();

    void onEnStationSelect();
    void onExStationSelect();
    void onStationSelected(QString lineName, QString stationName);

private:
    int m_curBtn;    // 0-en  1-ex

private:
    Ui::TicketReregisterWidget *ui;
};

#endif // TICKETREREGISTERWIDGET_H
