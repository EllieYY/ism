#include "SettingCenter.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>

#include "LineStations.h"
#include "LineTimeTables.h"
#include "ISMTimeTable.h"
#include "LineInterchangeInfo.h"
#include "InterchangeInfo.h"
#include "HttpTool.h"

SettingCenter* SettingCenter::m_pInstance = NULL;
SettingCenter::SettingCenter(QObject *parent) : QObject(parent)
{

}

SettingCenter *SettingCenter::getThis()
{
    if (m_pInstance == NULL)
        m_pInstance = new SettingCenter();
    return m_pInstance;
}

// 线路信息存储
void SettingCenter::saveLineStations(QList<LineStations *> lines)
{
    QJsonArray jsonArray;
    QJsonObject rootObject;
    QJsonObject branchObject;
    for (LineStations* line:lines) {
        QJsonArray subJsonArray;
        QList<QString> stations = line->stationList();
        for (QString station:stations) {
            subJsonArray.append(station);
        }

        branchObject.insert("lineName", line->getName());
        branchObject.insert("stations", subJsonArray);
        jsonArray.append(branchObject);
        clearJsonObject(branchObject);
    }

    rootObject.insert("lineStations", jsonArray);

    saveJsonFile(rootObject, "lineStations.json");
}

QList<LineStations *> SettingCenter::getLineStations()
{
    QList<LineStations *> lines;
    QJsonDocument jsonDocument = readJsonFile("lineStations.json");
    if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
        return lines;
    }
    QJsonObject rootObject = jsonDocument.object();
    return parseLineStation(rootObject);
}

QList<LineStations *> SettingCenter::parseLineStation(QJsonObject rootObject)
{
    QList<LineStations *> lines;
    if(!rootObject.contains("lineStations") || !rootObject.value("lineStations").isArray())
    {
        qDebug() << "No target value";
        qDebug() << rootObject.keys();
        return lines;
    }

    QJsonArray jsonArray = rootObject.value("lineStations").toArray();
    for(auto iter = jsonArray.constBegin(); iter != jsonArray.constEnd(); ++iter)
    {
        QJsonObject jsonObject = (*iter).toObject();

        // 线路名称
        if (!jsonObject.contains("lineName") || !jsonObject.value("lineName").isString()) {
            continue;
        }
        QString lineName = jsonObject.value("lineName").toString();
        LineStations* line = new LineStations(lineName, "", "");

        // 线路站点
        if (!jsonObject.contains("stations") || !jsonObject.value("stations").isArray()) {
            lines.append(line);
            continue;
        }
        QJsonArray subJsonArray = jsonObject.value( "stations" ).toArray();
        for(auto subIter = subJsonArray.constBegin(); subIter != subJsonArray.constEnd(); ++subIter)
        {
            if (!(*subIter).isString()) {
                continue;
            }
            line->addStation((*subIter).toString());
        }
        lines.append(line);
    }
    return lines;
}

// 线路运营时间存储
void SettingCenter::saveLineTimeTables(QList<LineTimeTables *> lines)
{
    QJsonArray jsonArray;
    QJsonObject rootObject;
    QJsonObject branchObject;
    QJsonObject leafObject;
    for (LineTimeTables* line:lines) {
        QJsonArray subJsonArray;
        QList<ISMTimeTable*> timeTables = line->timeTable();
        for (ISMTimeTable* timeTable:timeTables) {
            leafObject.insert("startStation", timeTable->startStation());
            leafObject.insert("endStation", timeTable->endStation());
            leafObject.insert("startTime", timeTable->startTime());
            leafObject.insert("endTime", timeTable->endTime());
            subJsonArray.append(leafObject);
        }

        branchObject.insert("lineName", line->getName());
        branchObject.insert("timeTable", subJsonArray);
        jsonArray.append(branchObject);

        clearJsonObject(leafObject);
        clearJsonObject(branchObject);
    }

    rootObject.insert("lineTime", jsonArray);
    saveJsonFile(rootObject, "lineTime.json");
}

QList<LineTimeTables *> SettingCenter::getLineTimeTables()
{
    QList<LineTimeTables *> lines;

    QJsonDocument jsonDocument = readJsonFile("lineTime.json");
    if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
        return lines;
    }
    QJsonObject rootObject = jsonDocument.object();
    return parseLineTimeTables(rootObject);
}

QList<LineTimeTables *> SettingCenter::parseLineTimeTables(QJsonObject rootObject)
{
    QList<LineTimeTables *> lines;
    if(!rootObject.contains("lineTime") || !rootObject.value("lineTime").isArray())
    {
        qDebug() << "No target value";
        qDebug() << rootObject.keys();
        return lines;
    }

    QJsonArray jsonArray = rootObject.value("lineTime").toArray();
    for(auto iter = jsonArray.constBegin(); iter != jsonArray.constEnd(); ++iter)
    {
        QJsonObject jsonObject = (*iter).toObject();

        // 线路名称
        if (!jsonObject.contains("lineName") || !jsonObject.value("lineName").isString()) {
            continue;
        }
        QString lineName = jsonObject.value("lineName").toString();
        LineTimeTables* line = new LineTimeTables(lineName, "", "");

        // 线路运营时间表
        if (!jsonObject.contains("timeTable") || !jsonObject.value("timeTable").isArray()) {
            lines.append(line);
            continue;
        }
        QJsonArray subJsonArray = jsonObject.value( "timeTable" ).toArray();
        for(auto subIter = subJsonArray.constBegin(); subIter != subJsonArray.constEnd(); ++subIter)
        {
            QJsonObject subJsonObject = (*subIter).toObject();
            if (subJsonObject.contains("startStation") && subJsonObject.value("startStation").isString() &&
                subJsonObject.contains("endStation") && subJsonObject.value("endStation").isString() &&
                subJsonObject.contains("startTime") && subJsonObject.value("startTime").isString() &&
                subJsonObject.contains("endTime") && subJsonObject.value("endTime").isString()) {
                ISMTimeTable* timeTable = new ISMTimeTable(subJsonObject.value("startStation").toString(),
                                                           subJsonObject.value("endStation").toString(),
                                                           subJsonObject.value("startTime").toString(),
                                                           subJsonObject.value("endTime").toString());
                line->addTimeTable(timeTable);
            }
        }
        lines.append(line);
    }
    return lines;
}

// 线路换乘信息存储
void SettingCenter::saveLineInterchanes(QList<LineInterchangeInfo *> lines)
{
    QJsonArray jsonArray;
    QJsonObject rootObject;
    QJsonObject branchObject;
    QJsonObject leafObject;
    for (LineInterchangeInfo* line:lines) {
        QJsonArray subJsonArray;
        QList<InterchangeInfo*> interchangeList = line->interchangeList();
        for (InterchangeInfo* info:interchangeList) {
            leafObject.insert("changeLine", info->lineName());
            leafObject.insert("lineColor", info->lineColor());
            leafObject.insert("stationName", info->stationName());
            subJsonArray.append(leafObject);
        }

        branchObject.insert("lineName", line->getName());
        branchObject.insert("lineColor", line->getColor());
        branchObject.insert("interchanges", subJsonArray);
        jsonArray.append(branchObject);

        clearJsonObject(leafObject);
        clearJsonObject(branchObject);
    }

    rootObject.insert("lineInterchanges", jsonArray);
    saveJsonFile(rootObject, "lineInterchanges.json");
}

QList<LineInterchangeInfo *> SettingCenter::getLineInterchanes()
{
    QList<LineInterchangeInfo *> lines;

    QJsonDocument jsonDocument = readJsonFile("lineInterchanges.json");
    if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
        return lines;
    }
    QJsonObject rootObject = jsonDocument.object();
    return parseLineInterchanes(rootObject);
}

/* 换乘信息解析 */
QList<LineInterchangeInfo *> SettingCenter::parseLineInterchanes(QJsonObject rootObject)
{
    QList<LineInterchangeInfo *> lines;
    if(!rootObject.contains("lineInterchanges") || !rootObject.value("lineInterchanges").isArray())
    {
        qDebug() << "No target value";
        qDebug() << rootObject.keys();
        return lines;
    }

    QJsonArray jsonArray = rootObject.value("lineInterchanges").toArray();
    for(auto iter = jsonArray.constBegin(); iter != jsonArray.constEnd(); ++iter)
    {
        QJsonObject jsonObject = (*iter).toObject();

        // 线路名称
        if (!jsonObject.contains("lineName") || !jsonObject.value("lineName").isString()) {
            continue;
        }
        QString lineName = jsonObject.value("lineName").toString();
        // 线路名称
        if (!jsonObject.contains("lineColor") || !jsonObject.value("lineColor").isString()) {
            continue;
        }
        QString lineColor = jsonObject.value("lineColor").toString();
        LineInterchangeInfo* line = new LineInterchangeInfo(lineName, "", lineColor);

        // 线路换乘信息
        if (!jsonObject.contains("interchanges") || !jsonObject.value("interchanges").isArray()) {
            lines.append(line);
            continue;
        }
        QJsonArray subJsonArray = jsonObject.value( "interchanges" ).toArray();
        for(auto subIter = subJsonArray.constBegin(); subIter != subJsonArray.constEnd(); ++subIter)
        {
            QJsonObject subJsonObject = (*subIter).toObject();
            if (subJsonObject.contains("changeLine") && subJsonObject.value("changeLine").isString() &&
                subJsonObject.contains("lineColor") && subJsonObject.value("lineColor").isString() &&
                subJsonObject.contains("stationName") && subJsonObject.value("stationName").isString()) {
                InterchangeInfo* info = new InterchangeInfo(subJsonObject.value("stationName").toString(),
                                                           subJsonObject.value("changeLine").toString(),
                                                           subJsonObject.value("lineColor").toString());
                line->addInterchangeInfo(info);
            }
        }
        lines.append(line);
    }


    // 中间结果 -- 转换


    return lines;
}



// 基础信息获取
QString SettingCenter::getBasicInfo()
{
    m_stationName = "";

    QString filePath = QDir::currentPath() + QDir::separator() + QDir::separator() + "setting.json";
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << QString("fail to open the file: %1, %2, %3")
                    .arg(__FILE__).arg(__LINE__).arg(__FUNCTION__);
        return m_stationName;
    }
    QByteArray array = file.readAll();
    file.close();

    QJsonParseError jsonParseError;
    QJsonDocument jsonDocument(QJsonDocument::fromJson(array, &jsonParseError));
    if(QJsonParseError::NoError != jsonParseError.error)
    {
        qDebug() << QString("JsonParseError: %1").arg(jsonParseError.errorString());
        return m_stationName;
    }

    QJsonObject rootObject = jsonDocument.object();
    if(!rootObject.keys().contains("stationName"))
    {
        qDebug() << "No target value";
        return m_stationName;
    }


    m_stationName = rootObject.value("stationName").toString();
    QString ip = rootObject.value("ip").toString();
    long port = rootObject.value("port").toInt();
    QString deviceId = rootObject.value("deviceId").toString();
    HttpTool::getThis()->setId(deviceId, m_stationName);
    HttpTool::getThis()->setServUrl(ip, port);

    m_hciLogPath = rootObject.value("hciLogPath").toString();

    return m_stationName;
}

QString SettingCenter::getHciLogPath()
{
    return m_hciLogPath;
}

void SettingCenter::clearJsonObject(QJsonObject &object)
{
    QStringList strList = object.keys();

    for(int i = 0; i < strList.size(); ++i)
    {
        object.remove(strList.at(i));
    }

}

void SettingCenter::saveJsonFile(QJsonObject rootObject, const QString &fileName)
{
    QJsonDocument jsonDocument;
    jsonDocument.setObject(rootObject);
    QByteArray byteArray = jsonDocument.toJson(QJsonDocument::Indented);

    QString filePath = QDir::currentPath() + QDir::separator() + QDir::separator() + fileName;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << QString("fail to open the file: %1, %2, %3").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__);
        return;
    }
    QTextStream out(&file);
    out.setCodec("utf-8");
    out << byteArray;

    file.close();
}

QJsonDocument SettingCenter::readJsonFile(const QString &fileName)
{
    QJsonDocument doc;

    QString filePath = QDir::currentPath() + QDir::separator() + QDir::separator() + fileName;
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << QString("fail to open the file: %1, %2, %3")
                    .arg(__FILE__).arg(__LINE__).arg(__FUNCTION__);
        return doc;
    }
    QByteArray array = file.readAll();
    file.close();

    QJsonParseError jsonParseError;
    doc = QJsonDocument::fromJson(array, &jsonParseError);
    if(QJsonParseError::NoError != jsonParseError.error)
    {
        qDebug() << QString("JsonParseError: %1").arg(jsonParseError.errorString());
        return doc;
    }

    return doc;
}


