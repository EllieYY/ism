#include "LineStationTimetables.h"
#include "StationTime.h"

LineStationTimetables::LineStationTimetables(QObject *parent) : QObject(parent)
{

}

int LineStationTimetables::lineCode() const
{
    return m_lineCode;
}

void LineStationTimetables::setLineCode(int lineCode)
{
    m_lineCode = lineCode;
}

QString LineStationTimetables::dirA() const
{
    return m_dirA;
}

void LineStationTimetables::setDirA(const QString &dirA)
{
    m_dirA = dirA;
}

QString LineStationTimetables::dirB() const
{
    return m_dirB;
}

void LineStationTimetables::setDirB(const QString &dirB)
{
    m_dirB = dirB;
}

QList<StationTime *> LineStationTimetables::stationTimes() const
{
    return m_stationTimes;
}

void LineStationTimetables::setStationTimes(const QList<StationTime *> &stationTimes)
{
    m_stationTimes = stationTimes;
}
