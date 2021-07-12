#ifndef METROINTERCHANGEWIDGET_H
#define METROINTERCHANGEWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class MetroInterchangeWidget;
}

class MetroInterchangeWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit MetroInterchangeWidget(QWidget *parent = nullptr);
    ~MetroInterchangeWidget();

private:
    bool showData();
    void setStyle();

    void init();
    void setTestData();

private:
    Ui::MetroInterchangeWidget *ui;
};

#endif // METROINTERCHANGEWIDGET_H
