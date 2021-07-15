#ifndef INTERCHANGESTATION_H
#define INTERCHANGESTATION_H

#include <QObject>

/*
 * 换乘信息 -- 对应请求返回结果集 | 中间结果
 */
class InterchangeStation : public QObject
{
    Q_OBJECT
public:
    explicit InterchangeStation(QObject *parent = nullptr);

private:
    QString m_station;              // 换乘站点
    QList<QString> m_lineCodeList;  // 换乘线路编号列表
    QList<QString> m_lineNameList;  // 换乘线路名称列表
    QList<QString> m_lineColorList; // 换乘线路颜色列表

signals:

};

#endif // INTERCHANGESTATION_H
