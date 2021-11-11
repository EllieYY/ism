#ifndef TICKETMAINWIDGET_H
#define TICKETMAINWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class TicketMainWidget;
}

class CardReadWidget;
class TicketMainWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit TicketMainWidget(QWidget *parent = nullptr);
    ~TicketMainWidget();

    void secEvent();

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
