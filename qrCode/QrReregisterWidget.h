#ifndef QRREREGISTERWIDGET_H
#define QRREREGISTERWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class QrReregisterWidget;
}

class QrReregisterWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit QrReregisterWidget(QWidget *parent = nullptr);
    ~QrReregisterWidget();

private:
    bool showData();
    void setStyle();

    void init();
    void setTestData();

private:
    Ui::QrReregisterWidget *ui;
};

#endif // QRREREGISTERWIDGET_H
