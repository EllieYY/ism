#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QTimer>
#include "WidgetBase.h"

namespace Ui {
class TitleBar;
}

class TitleBar : public WidgetBase
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = nullptr);
    ~TitleBar();

public:
    static TitleBar* getThis();
    void secEvent();

private:
    bool showData();
    void setTestData();
    void showLogoutBtn(bool isMain);
    void onLogout();

signals:
    void logout();

private:
    QTimer* m_timer;

private:
    Ui::TitleBar *ui;
};

#endif // TITLEBAR_H
