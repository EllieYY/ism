#ifndef METROPERIPHERALWIDGET_H
#define METROPERIPHERALWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class MetroPeripheralWidget;
}

class MetroPeripheralWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit MetroPeripheralWidget(QWidget *parent = nullptr);
    ~MetroPeripheralWidget();

private:
    Ui::MetroPeripheralWidget *ui;
};

#endif // METROPERIPHERALWIDGET_H
