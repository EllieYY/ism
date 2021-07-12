#ifndef SETTINGCENTER_H
#define SETTINGCENTER_H

#include <QObject>
#include <QJsonArray>

class LineTimeTables;
class LineStations;
class LineInterchangeInfo;
class SettingCenter : public QObject
{
    Q_OBJECT
public:
    explicit SettingCenter(QObject *parent = nullptr);
    static SettingCenter* getThis();
    static SettingCenter* m_pInstance;

public:
    void saveLineStations(QList<LineStations*> lines);
    QList<LineStations*> getLineStations();
    QList<LineStations*> parseLineStation(QJsonObject obj);

    void saveLineTimeTables(QList<LineTimeTables*> lines);
    QList<LineTimeTables*> getLineTimeTables();
    QList<LineTimeTables*> parseLineTimeTables(QJsonObject obj);

    void saveLineInterchanes(QList<LineInterchangeInfo*> lines);
    QList<LineInterchangeInfo*> getLineInterchanes();
    QList<LineInterchangeInfo*> parseLineInterchanes(QJsonObject obj);

    QString getBasicInfo();
    QString getHciLogPath();

private:
    void clearJsonObject(QJsonObject &object);
    void saveJsonFile(QJsonObject rootObject, const QString &fileName);
    QJsonDocument readJsonFile(const QString &fileName);

private:
    QString m_stationName;      // 站点名称
    QString m_hciLogPath;       // 日志路径

signals:

};

#endif // SETTINGCENTER_H
