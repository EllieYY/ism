#ifndef STATIONLISTWIDGET_H
#define STATIONLISTWIDGET_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class StationListWidget;
}

class StationListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StationListWidget(QString m_lineName, QList<QString> stations, QWidget *parent = nullptr);
    ~StationListWidget();

private:
    void onSelected(QListWidgetItem *item);
    void setStyle();

private:
    QString m_lineName;     // 线路名称

signals:
    void selected(QString lineName, QString stationName);

private:
    Ui::StationListWidget *ui;
};

#endif // STATIONLISTWIDGET_H
