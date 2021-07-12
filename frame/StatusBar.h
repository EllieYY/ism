#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class StatusBar;
}

class StatusBar : public WidgetBase
{
    Q_OBJECT

public:
    explicit StatusBar(QWidget *parent = nullptr);
    ~StatusBar();

public:
    static StatusBar* getThis();
    void showButtons(bool isShow);
    void secEvent();

private:
    bool showData();
    void setTestData();


private:
    Ui::StatusBar *ui;
};

#endif // STATUSBAR_H
