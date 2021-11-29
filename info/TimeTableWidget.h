#ifndef TIMETABLEWIDGET_H
#define TIMETABLEWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class TimeTableWidget;
}

class TimeTableWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit TimeTableWidget(QWidget *parent = nullptr);
    ~TimeTableWidget();

private:
    bool showData();
    void setStyle();

    void init();
    void setTestData();

private:
    bool m_initOk;

private:
    Ui::TimeTableWidget *ui;
};

#endif // TIMETABLEWIDGET_H
