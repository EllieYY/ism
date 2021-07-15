#ifndef REFUND_H
#define REFUND_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class RefundWidget;
}

class RefundWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit RefundWidget(QWidget *parent = nullptr);
    ~RefundWidget();

private:
    bool showData();
    void setStyle();

    void init();
    void setTestData();

private:
    Ui::RefundWidget *ui;
};

#endif // REFUND_H
