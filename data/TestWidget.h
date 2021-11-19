#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include "WidgetBase.h"
#include "logger.h"
#include "propertyconfigurator.h"

namespace Ui {
class TestWidget;
}

class TestWidget : public WidgetBase
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit TestWidget(QWidget *parent = nullptr);
    ~TestWidget();

private slots:
    void on_Btn3001_clicked();

    void on_Btn4002_clicked();

    void on_Btn7000_clicked();

    void on_Btn7001_clicked();

    void on_Btn9003_login_clicked();

    void on_Btn9003_logout_clicked();

    void on_Btn9002_clicked();

    void on_Btn9002_2_clicked();

private:
    void init();

    void secEvent();


private:
    QString m_userName;

private:
    Ui::TestWidget *ui;
};

#endif // TESTWIDGET_H
