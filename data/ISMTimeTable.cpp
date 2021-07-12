#include "ISMTimeTable.h"

ISMTimeTable::ISMTimeTable(QString startS, QString endS, QString startT, QString endT, QObject *parent)
    : QObject(parent)
{
    m_startStation = startS;
    m_endStation = endS;
    m_startTime = startT;
    m_endTime = endT;
}

QString ISMTimeTable::startStation() const
{
    return m_startStation;
}

void ISMTimeTable::setStartStation(const QString &startStation)
{
    m_startStation = startStation;
}

QString ISMTimeTable::endStation() const
{
    return m_endStation;
}

void ISMTimeTable::setEndStation(const QString &endStation)
{
    m_endStation = endStation;
}

QString ISMTimeTable::startTime() const
{
    return m_startTime;
}

void ISMTimeTable::setStartTime(const QString &startTime)
{
    m_startTime = startTime;
}

QString ISMTimeTable::endTime() const
{
    return m_endTime;
}

void ISMTimeTable::setEndTime(const QString &endTime)
{
    m_endTime = endTime;
}
