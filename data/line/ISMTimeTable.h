#ifndef ISMTIMETABLE_H
#define ISMTIMETABLE_H

#include <QObject>

class ISMTimeTable : public QObject
{
    Q_OBJECT
public:
    explicit ISMTimeTable(QString startS, QString endS, QString startT, QString endT, QObject *parent = nullptr);

public:
    QString startStation() const;
    void setStartStation(const QString &startStation);

    QString endStation() const;
    void setEndStation(const QString &endStation);

    QString startTime() const;
    void setStartTime(const QString &startTime);

    QString endTime() const;
    void setEndTime(const QString &endTime);

private:
    QString m_startStation;     // 起始站
    QString m_endStation;       // 终点站
    QString m_startTime;        // 首班车时间
    QString m_endTime;          // 末班车时间

signals:

};

#endif // ISMTIMETABLE_H
