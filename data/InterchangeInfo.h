#ifndef INTERCHANGEINFO_H
#define INTERCHANGEINFO_H

#include <QObject>

class InterchangeInfo : public QObject
{
    Q_OBJECT
public:
    explicit InterchangeInfo(QString stationName,
                             QString lineName,
                             QString lineColor,
                             QObject *parent = nullptr);


    QString stationName() const;
    void setStationName(const QString &stationName);

    QString lineName() const;
    void setLineName(const QString &lineName);

    QString lineColor() const;
    void setLineColor(const QString &lineColor);

private:
    QString m_stationName;      // 换乘站点名称
    QString m_lineName;         // 换乘线路名称
    QString m_lineColor;        // 换乘线路颜色

signals:

};

#endif // INTERCHANGEINFO_H
