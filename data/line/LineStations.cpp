#include "LineStations.h"
#include "Station.h"

LineStations::LineStations(QString name, QString code, QString picPath,
                           QString color, QObject *parent) :
    LineInfo(name, code, picPath,color,parent)
{
    m_stationList.clear();
}

QList<Station*> LineStations::stationList() const
{
    return m_stationList;
}

void LineStations::setStationList(const QList<Station*> &stationList)
{
    m_stationList = stationList;
}

void LineStations::addStations(QList<Station*> stations)
{
    m_stationList.append(stations);
}

void LineStations::addStation(Station* station)
{
    m_stationList.append(station);
}
