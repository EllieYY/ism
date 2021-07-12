#ifndef QRQUERYWIDGET_H
#define QRQUERYWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class QrQueryWidget;
}

class QrQueryWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit QrQueryWidget(QWidget *parent = nullptr);
    ~QrQueryWidget();

private:
    Ui::QrQueryWidget *ui;
};

#endif // QRQUERYWIDGET_H
