#ifndef STATIONSELECTWIDGET_H
#define STATIONSELECTWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

#include "CustomTabWidget.h"
namespace Ui {
class StationSelectWidget;
}

class StationSelectWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit StationSelectWidget(QWidget *parent = nullptr);
    ~StationSelectWidget();

    bool showData();

private:
    void setStyle();

    void init();
    void setTestData();

    void onClose();
    void onSelected(QString lineName, QString stationName, QString stationCode);

private:
   CustomTabWidget* m_tabWidget;

signals:
   void stationSelected(QString lineName, QString stationName, QString stationCode);

private:
    Ui::StationSelectWidget *ui;
};

#endif // STATIONSELECTWIDGET_H
