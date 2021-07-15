#ifndef TICKETQUERYWIDGET_H
#define TICKETQUERYWIDGET_H

#include <QWidget>
#include "WidgetBase.h"


namespace Ui {
class TicketQueryWidget;
}

class TicketQueryWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit TicketQueryWidget(QWidget *parent = nullptr);
    ~TicketQueryWidget();

private:
    bool showData();
    void setStyle();

    void init();
    void setTestData();

private:
    Ui::TicketQueryWidget *ui;
};

#endif // TICKETQUERYWIDGET_H
