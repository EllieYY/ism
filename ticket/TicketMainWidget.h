#ifndef TICKETMAINWIDGET_H
#define TICKETMAINWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class TicketMainWidget;
}

class CardReadWidget;
class DeviceManager;
class TicketMainWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit TicketMainWidget(QWidget *parent = nullptr);
    ~TicketMainWidget();

    void secEvent();

    void setDeviceManager(DeviceManager* manager);

//    void setCardReadWidget(CardReadWidget *cardReadWidget);

private:
    void init();
    void onBtn(int btnId);
    void onDataOk(int widgetId);

private:
    QButtonGroup*   m_buttonGroup;
    CardReadWidget* m_cardReadWidget;

private:
    Ui::TicketMainWidget *ui;
};

#endif // TICKETMAINWIDGET_H
