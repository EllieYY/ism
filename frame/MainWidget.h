#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void onBtn(int btnId);

private:
    void init();
    void setTestData();

    void secEvent();

private:
    QButtonGroup *m_buttonGroup;

private:
    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H
