#ifndef TICKETREREGISTERWIDGET_H
#define TICKETREREGISTERWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class TicketReregisterWidget;
}

class CompensationFareWidget;
class TicketReregisterWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit TicketReregisterWidget(QWidget *parent = nullptr);
    ~TicketReregisterWidget();

    void secEvent();

private:
    bool showData();
    void setStyle();

    void init();
    void setTestData();

    void onEnStationSelect();
    void onExStationSelect();
    void onStationSelected(QString lineName, QString stationName, QString stationCode);

    void onUpdateTicket();          // 票卡更新
    void cashSupplementary();       // 现金补票

    void onSupplementaryOk(bool result);    //  补票结果返回

private:
    int m_curBtn;               // 0-en  1-ex
    int m_updateType;           // 更新类型
    CompensationFareWidget* m_fareWidget;       // 补差额窗口
    double  m_difference;       // 代补差价

signals:
    void selectStation();    // 选择车站

private:
    Ui::TicketReregisterWidget *ui;
};

#endif // TICKETREREGISTERWIDGET_H
