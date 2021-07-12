#ifndef GUIDEMAINWIDGET_H
#define GUIDEMAINWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class GuideMainWidget;
}

class GuideMainWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit GuideMainWidget(QWidget *parent = nullptr);
    ~GuideMainWidget();

private:
    Ui::GuideMainWidget *ui;
};

#endif // GUIDEMAINWIDGET_H
