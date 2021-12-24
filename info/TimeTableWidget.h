#ifndef TIMETABLEWIDGET_H
#define TIMETABLEWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class TimeTableWidget;
}

typedef struct {
    int btnId;
    QTableView* view;
}LINE_VIEW;

class LineStationTimetables;
class TimeTableWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit TimeTableWidget(QWidget *parent = nullptr);
    ~TimeTableWidget();

private:
    bool showData();
    void showLineTimetables(QTableView* view, LineStationTimetables* lineInfo);
    void onBtn(int id);

    void setStyle();

    void init();
    void setTestData();

private:
    bool m_initOk;
    QMap<int, LineStationTimetables*> m_lineTimes;
    QMap<int, QTableView*> m_lineViewMap;
    QTableView* m_tableView;

private:
    Ui::TimeTableWidget *ui;
};

#endif // TIMETABLEWIDGET_H
