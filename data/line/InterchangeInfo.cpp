#include "InterchangeInfo.h"
#include "LineInfo.h"

InterchangeInfo::InterchangeInfo(QString stationName,
                                 QString stationCode,
                                 QList<LineInfo*> lineList,
//                                 QString lineName,
//                                 QString lineColor,
                                 QObject *parent) : QObject(parent)
{
    m_stationName = stationName;
    m_stationCode = stationCode;
    m_lineList = lineList;

    m_lineCode = m_stationCode.mid(0,2);
//    m_lineName = lineName;
//    m_lineColor = lineColor;
}

QString InterchangeInfo::stationName() const
{
    return m_stationName;
}

void InterchangeInfo::setStationName(const QString &stationName)
{
    m_stationName = stationName;
}

QList<LineInfo *> InterchangeInfo::lineList() const
{
    return m_lineList;
}

void InterchangeInfo::setLineList(const QList<LineInfo *> &lineList)
{
    m_lineList = lineList;
}

// 判断换乘站点属于某线路
bool InterchangeInfo::isPartOfLine(QString lineCode)
{
    if (m_lineCode.compare(lineCode) == 0) {
        return true;
    }

    return false;
}

QString InterchangeInfo::stationCode() const
{
    return m_stationCode;
}

void InterchangeInfo::setStationCode(const QString &stationCode)
{
    m_stationCode = stationCode;
}

//QString InterchangeInfo::lineName() const
//{
//    return m_lineName;
//}

//void InterchangeInfo::setLineName(const QString &lineName)
//{
//    m_lineName = lineName;
//}

//QString InterchangeInfo::lineColor() const
//{
//    return m_lineColor;
//}

//void InterchangeInfo::setLineColor(const QString &lineColor)
//{
//    m_lineColor = lineColor;
//}
