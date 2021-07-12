#ifndef CUATOMTABWIDGET_H
#define CUATOMTABWIDGET_H

#include <QTabWidget>

class CustomTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit CustomTabWidget(QWidget *parent = nullptr);

    void updateselect();                          //用来设置一开始选中的状态
    int addTab(QWidget *widget, const QString &); //重载(不是虚函数)

private:
    void slotSetLabelColor(int color);            //自定义槽(响应tabBar点击事件)


signals:

};

#endif // CUATOMTABWIDGET_H
