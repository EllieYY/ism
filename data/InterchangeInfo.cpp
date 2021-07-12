#include "InterchangeInfo.h"

InterchangeInfo::InterchangeInfo(QString stationName,
                                 QString lineName,
                                 QString lineColor,
                                 QObject *parent) : QObject(parent)
{
    m_stationName = stationName;
    m_lineName = lineName;
    m_lineColor = lineColor;
}

QString InterchangeInfo::stationName() const
{
    return m_stationName;
}

void InterchangeInfo::setStationName(const QString &stationName)
{
    m_stationName = stationName;
}

QString InterchangeInfo::lineName() const
{
    return m_lineName;
}

void InterchangeInfo::setLineName(const QString &lineName)
{
    m_lineName = lineName;
}

QString InterchangeInfo::lineColor() const
{
    return m_lineColor;
}

void InterchangeInfo::setLineColor(const QString &lineColor)
{
    m_lineColor = lineColor;
}
