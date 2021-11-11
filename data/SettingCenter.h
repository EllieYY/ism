#ifndef SETTINGCENTER_H
#define SETTINGCENTER_H

#include <QObject>
#include <QJsonArray>

class LineInfo;
class LineTimeTables;
class LineStations;
class LineInterchangeInfo;
class InterchangeInfo;
class BasicInfo;
class SettingCenter : public QObject
{
    Q_OBJECT
public:
    explicit SettingCenter(QObject *parent = nullptr);
    static SettingCenter* getThis();
    static SettingCenter* m_pInstance;

public:
    // 线路基础信息
    void saveLineBasicInfo(QList<LineInfo*> lines);
    QList<LineInfo*> getLineBasicInfo();
    QList<LineInfo*> parseLineBasicInfo(QJsonObject obj);

    // 线路站点信息
    void saveLineStations(QList<LineStations*> lines);
    QList<LineStations*> getLineStations();
    QList<LineStations*> parseLineStation(QJsonObject obj);

    // 线路时刻表
    void saveLineTimeTables(QList<LineTimeTables*> lines);
    QList<LineTimeTables*> getLineTimeTables();
    QList<LineTimeTables*> parseLineTimeTables(QJsonObject obj);

    // 线路换乘信息
    void saveLineInterchanes(QList<LineInterchangeInfo*> lines);
    QList<LineInterchangeInfo*> getLineInterchanes();
    QList<LineInterchangeInfo*> parseLineInterchanes(QJsonObject obj);

    BasicInfo* getBasicInfo();

private:
    QList<InterchangeInfo*> matchInterchangeStations(QList<InterchangeInfo*> stations, QString lineCode);
    void clearJsonObject(QJsonObject &object);
    void saveJsonFile(QJsonObject rootObject, const QString &fileName);
    QJsonDocument readJsonFile(const QString &fileName);

};

#endif // SETTINGCENTER_H
