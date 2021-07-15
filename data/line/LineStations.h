#ifndef LINESTATIONS_H
#define LINESTATIONS_H

#include "LineInfo.h"

class Station;
class LineStations : public LineInfo
{
public:
    LineStations(QString name, QString code, QString picPath, QString color, QObject *parent = nullptr);

    QList<Station*> stationList() const;
    void setStationList(const QList<Station*> &stationList);
    void addStations(QList<Station*> stations);
    void addStation(Station* station);

private:
    QList<Station*> m_stationList;       // 站点列表

};

#endif // LINESTATIONS_H
