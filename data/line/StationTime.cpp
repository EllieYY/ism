#include "StationTime.h"

StationTime::StationTime(QObject *parent) : QObject(parent)
{

}

QString StationTime::stationName() const
{
    return m_stationName;
}

void StationTime::setStationName(const QString &stationName)
{
    m_stationName = stationName;
}

QString StationTime::aStartTime() const
{
    return m_aStartTime;
}

void StationTime::setAStartTime(const QString &aStartTime)
{
    m_aStartTime = aStartTime;
}

QString StationTime::aEndTime() const
{
    return m_aEndTime;
}

void StationTime::setAEndTime(const QString &aEndTime)
{
    m_aEndTime = aEndTime;
}

QString StationTime::bStartTime() const
{
    return m_bStartTime;
}

void StationTime::setBStartTime(const QString &bStartTime)
{
    m_bStartTime = bStartTime;
}

QString StationTime::bEndTime() const
{
    return m_bEndTime;
}

void StationTime::setBEndTime(const QString &bEndTime)
{
    m_bEndTime = bEndTime;
}
