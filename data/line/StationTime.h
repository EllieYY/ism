#ifndef STATIONTIME_H
#define STATIONTIME_H

#include <QObject>
/*
 * 站点时间信息
*/
class StationTime : public QObject
{
    Q_OBJECT
public:
    explicit StationTime(QObject *parent = nullptr);

    QString stationName() const;
    void setStationName(const QString &stationName);

    QString aStartTime() const;
    void setAStartTime(const QString &aStartTime);

    QString aEndTime() const;
    void setAEndTime(const QString &aEndTime);

    QString bStartTime() const;
    void setBStartTime(const QString &bStartTime);

    QString bEndTime() const;
    void setBEndTime(const QString &bEndTime);

signals:

private:
    QString m_stationName;
    QString m_aStartTime;      // 方向A的首班车时间
    QString m_aEndTime;        // 方向A的末班车时间
    QString m_bStartTime;      // 方向B的首班车时间
    QString m_bEndTime;        // 方向B的末班车时间
};

#endif // STATIONTIME_H
