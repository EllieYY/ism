#ifndef INTERCHANGEINFO_H
#define INTERCHANGEINFO_H

#include <QObject>

class LineInfo;
class InterchangeInfo : public QObject
{
    Q_OBJECT
public:
    explicit InterchangeInfo(QString stationName,
                             QString stationCode,
                             QList<LineInfo*> lineList,
//                             QString lineName,
//                             QString lineColor,
                             QObject *parent = nullptr);


    QString stationName() const;
    void setStationName(const QString &stationName);

//    QString lineName() const;
//    void setLineName(const QString &lineName);

//    QString lineColor() const;
//    void setLineColor(const QString &lineColor);

    QList<LineInfo *> lineList() const;
    void setLineList(const QList<LineInfo *> &lineList);

    bool isPartOfLine(QString lineCode);

    QString stationCode() const;
    void setStationCode(const QString &stationCode);

private:
    QString m_stationName;        // 换乘站点名称
    QString m_stationCode;        // 换乘站点编号 -- 用来对应线路
    QString m_lineCode;           // 换乘站点对应的线路
    QList<LineInfo*> m_lineList;  // 换乘线路信息
//    QString m_lineName;         // 换乘线路名称
//    QString m_lineColor;        // 换乘线路颜色

signals:

};

#endif // INTERCHANGEINFO_H
