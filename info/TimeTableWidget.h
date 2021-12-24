#ifndef TIMETABLEWIDGET_H
#define TIMETABLEWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

namespace Ui {
class TimeTableWidget;
}

typedef struct {
    int btnId;
    QPushButton* btn;
    QTableWidget* view;
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

    void setViewStyle(QTableView* view);
    void setStyle();

    void init();
    void setTestData();

private:
    bool m_initOk;
    QMap<int, LineStationTimetables*> m_lineTimes;
    QMap<int, LINE_VIEW> m_lineViewMap;
    QTableView* m_preTableView;
    QTableWidget* m_preTableWidget;
    LINE_VIEW m_preLineView;

private:
    Ui::TimeTableWidget *ui;
};

#endif // TIMETABLEWIDGET_H
