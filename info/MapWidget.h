#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include "WidgetBase.h"


namespace Ui {
class MapWidget;
}

class MapWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = nullptr);
    ~MapWidget();

private:
    Ui::MapWidget *ui;
};

#endif // MAPWIDGET_H
