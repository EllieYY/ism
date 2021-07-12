#include "LineStations.h"

LineStations::LineStations(QString name, QString picPath,
                           QString color, QObject *parent) :
    LineInfo(name, picPath,color,parent)
{
    m_stationList.clear();
}

QList<QString> LineStations::stationList() const
{
    return m_stationList;
}

void LineStations::setStationList(const QList<QString> &stationList)
{
    m_stationList = stationList;
}

void LineStations::addStations(QList<QString> stations)
{
    m_stationList.append(stations);
}

void LineStations::addStation(QString station)
{
    m_stationList.append(station);
}
