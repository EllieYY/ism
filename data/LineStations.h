#ifndef LINESTATIONS_H
#define LINESTATIONS_H

#include "LineInfo.h"

class LineStations : public LineInfo
{
public:
    LineStations(QString name, QString picPath, QString color, QObject *parent = nullptr);

    QList<QString> stationList() const;
    void setStationList(const QList<QString> &stationList);
    void addStations(QList<QString> stations);
    void addStation(QString station);

private:
    QList<QString> m_stationList;       // 站点列表

};

#endif // LINESTATIONS_H
