#ifndef LINESTATIONTIMETABLES_H
#define LINESTATIONTIMETABLES_H

#include <QObject>

/*
 * 线路各站点时间表信息
*/
class StationTime;
class LineStationTimetables : public QObject
{
    Q_OBJECT
public:
    explicit LineStationTimetables(QObject *parent = nullptr);

    int lineCode() const;
    void setLineCode(int lineCode);

    QString dirA() const;
    void setDirA(const QString &dirA);

    QString dirB() const;
    void setDirB(const QString &dirB);

    QList<StationTime *> stationTimes() const;
    void setStationTimes(const QList<StationTime *> &stationTimes);
    void addStationTime(StationTime* station);

signals:

private:
    int m_lineCode;     // 线路编码
    QString m_dirA;     // 方向A名称
    QString m_dirB;     // 方向B名称
    QList<StationTime*> m_stationTimes;     // 站点时刻表

};

#endif // LINESTATIONTIMETABLES_H
