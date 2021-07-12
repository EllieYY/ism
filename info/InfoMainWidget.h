#ifndef INFOMAINWIDGET_H
#define INFOMAINWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class InfoMainWidget;
}

class InfoMainWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit InfoMainWidget(QWidget *parent = nullptr);
    ~InfoMainWidget();

protected:
    void registerBtn(QAbstractButton *btn, int id);

private slots:
    void onBtn(int btnId);

private:
    void init();
    void setTestData();

private:
    QButtonGroup *m_buttonGroup;

private:
    Ui::InfoMainWidget *ui;
};

#endif // INFOMAINWIDGET_H
