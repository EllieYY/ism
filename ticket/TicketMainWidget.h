#ifndef TICKETMAINWIDGET_H
#define TICKETMAINWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class TicketMainWidget;
}

class TicketMainWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit TicketMainWidget(QWidget *parent = nullptr);
    ~TicketMainWidget();

protected:
    void registerBtn(QToolButton *btn, int id);

private slots:
    void onBtn(int btnId);

private:
    void init();

private:
    QButtonGroup *m_buttonGroup;

private:
    Ui::TicketMainWidget *ui;
};

#endif // TICKETMAINWIDGET_H
