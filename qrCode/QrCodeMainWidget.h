#ifndef QRCODEMAINWIDGET_H
#define QRCODEMAINWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class QrCodeMainWidget;
}

class QrCodeMainWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit QrCodeMainWidget(QWidget *parent = nullptr);
    ~QrCodeMainWidget();

protected:
    void registerBtn(QToolButton *btn, int id);

private slots:
    void onBtn(int btnId);

private:
    void init();
    void setTestData();

private:
    QButtonGroup *m_buttonGroup;

private:
    Ui::QrCodeMainWidget *ui;
};

#endif // QRCODEMAINWIDGET_H
