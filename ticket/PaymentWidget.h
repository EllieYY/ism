#ifndef PAYMENTWIDGET_H
#define PAYMENTWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class PaymentWidget;
}

class PaymentWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit PaymentWidget(QWidget *parent = nullptr);
    ~PaymentWidget();


private:
    void init();
    void onBtn(int btnId);

private:
    QButtonGroup* m_buttonGroup;

private:
    Ui::PaymentWidget *ui;
};

#endif // PAYMENTWIDGET_H
