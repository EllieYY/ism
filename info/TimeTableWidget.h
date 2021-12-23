#ifndef TIMETABLEWIDGET_H
#define TIMETABLEWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class TimeTableWidget;
}

class LineStationTimetables;
class TimeTableWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit TimeTableWidget(QWidget *parent = nullptr);
    ~TimeTableWidget();

private:
    bool showData();
    void showLineTimetables(int line);

    void setStyle();

    void init();
    void setTestData();

private:
    bool m_initOk;
    QMap<int, LineStationTimetables*> m_lineTimes;

private:
    Ui::TimeTableWidget *ui;
};

#endif // TIMETABLEWIDGET_H
