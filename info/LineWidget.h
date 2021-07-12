#ifndef LINEWIDGET_H
#define LINEWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

class LineInfo;
namespace Ui {
class LineWidget;
}

class LineWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit LineWidget(QWidget *parent = nullptr);
    ~LineWidget();

private:
    void init();
    void onLineChange(int id);

private:
    QList<LineInfo*>        m_lineInfoLst;
    QButtonGroup*           m_btnGroup;

private:
    Ui::LineWidget *ui;
};

#endif // LINEWIDGET_H
