#include "SettingCenter.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>

#include "LineInfo.h"
#include "LineStations.h"
#include "LineTimeTables.h"
#include "ISMTimeTable.h"
#include "LineInterchangeInfo.h"
#include "InterchangeInfo.h"
#include "InterchangeStation.h"
#include "HttpTool.h"
#include "DataCenter.h"
#include "Station.h"

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

void SettingCenter::saveLineBasicInfo(QList<LineInfo *> lines)
{
    QJsonArray jsonArray;
    QJsonObject rootObject;
    QJsonObject branchObject;
    for (LineInfo* line : lines) {
        branchObject.insert("lineName", line->getName());
        branchObject.insert("lineColor", line->getColor());
        branchObject.insert("lineCode", line->getCode());
        branchObject.insert("lineMap", line->getPicPath());
        jsonArray.append(branchObject);
        clearJsonObject(branchObject);
    }

    rootObject.insert("lineBasic", jsonArray);

    saveJsonFile(rootObject, "lineBasic.json");

}

QList<LineInfo *> SettingCenter::getLineBasicInfo()
{
    QList<LineInfo *> lines;
    QJsonDocument jsonDocument = readJsonFile("lineBasic.json");
    if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
        return lines;
    }
    QJsonObject rootObject = jsonDocument.object();
    return parseLineBasicInfo(rootObject);
}

// 线路基础信息
QList<LineInfo *> SettingCenter::parseLineBasicInfo(QJsonObject rootObject)
{
    QList<LineInfo *> lines;
    if(!rootObject.contains("lineBasic") || !rootObject.value("lineBasic").isArray())
    {
        qDebug() << "No target value";
        qDebug() << rootObject.keys();
        return lines;
    }

    QJsonArray jsonArray = rootObject.value("lineBasic").toArray();
    for(auto iter = jsonArray.constBegin(); iter != jsonArray.constEnd(); ++iter)
    {
        QJsonObject jsonObject = (*iter).toObject();

        // 线路名称
        if (!jsonObject.contains("lineName") || !jsonObject.value("lineName").isString()) {
            continue;
        }
        QString name = jsonObject.value("lineName").toString();

        // 线路编号
        if (!jsonObject.contains("lineCode") || !jsonObject.value("lineCode").isString()) {
            continue;
        }
        QString code = jsonObject.value("lineCode").toString();

        // 线路颜色
        if (!jsonObject.contains("lineColor") || !jsonObject.value("lineColor").isString()) {
            continue;
        }
        QString color = jsonObject.value("lineColor").toString();

        // 线路图片路径
        QString map = jsonObject.value("lineMap").toString();

        LineInfo* line = new LineInfo(name, code, map, color);
        lines.append(line);
    }
    return lines;
}



// 线路信息存储
void SettingCenter::saveLineStations(QList<LineStations *> lines)
{
    QJsonArray jsonArray;
    QJsonObject rootObject;
    QJsonObject branchObject;
    for (LineStations* line:lines) {
        QJsonArray subJsonArray;
        QList<Station*> stations = line->stationList();

        QJsonObject leafObject;
        for (Station* station:stations) {
            leafObject.insert("stationName", station->name());
            leafObject.insert("stationCode", station->code());
            subJsonArray.append(leafObject);
        }

        branchObject.insert("lineName", line->getName());
        branchObject.insert("lineColor", line->getColor());
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

        // 线路颜色
        if (!jsonObject.contains("lineColor") || !jsonObject.value("lineColor").isString()) {
            continue;
        }
        QString lineColor = jsonObject.value("lineColor").toString();
        LineStations* line = new LineStations(lineName, "", "", lineColor);

        // 线路站点
        if (!jsonObject.contains("stations") || !jsonObject.value("stations").isArray()) {
            lines.append(line);
            continue;
        }

        QJsonArray subJsonArray = jsonObject.value( "stations" ).toArray();
        for(auto subIter = subJsonArray.constBegin(); subIter != subJsonArray.constEnd(); ++subIter)
        {
            QJsonObject subJsonObject = (*subIter).toObject();
            if (subJsonObject.contains("stationName") && subJsonObject.value("stationName").isString() &&
                subJsonObject.contains("stationCode") && subJsonObject.value("stationCode").isString()) {
                Station* station = new Station(subJsonObject.value("stationName").toString(),
                                               subJsonObject.value("stationCode").toString());
                line->addStation(station);
            }
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
    if(!rootObject.contains("lineTimeTables") || !rootObject.value("lineTimeTables").isArray())
    {
        qDebug() << "No target value";
        qDebug() << rootObject.keys();
        return lines;
    }

    QJsonArray jsonArray = rootObject.value("lineTimeTables").toArray();
    for(auto iter = jsonArray.constBegin(); iter != jsonArray.constEnd(); ++iter)
    {
        QJsonObject jsonObject = (*iter).toObject();

        // 线路名称
        if (!jsonObject.contains("lineName") || !jsonObject.value("lineName").isString()) {
            continue;
        }
        QString lineName = jsonObject.value("lineName").toString();
        LineTimeTables* line = new LineTimeTables(lineName, "", "", "");

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
            QJsonArray lineJsonArray;
            QJsonObject lineItemLeafObject;
            QList<LineInfo*> interLines = info->lineList();
            for (LineInfo* item:interLines) {
                lineItemLeafObject.insert("lineName", item->getName());
                lineItemLeafObject.insert("lineCode", item->getCode());
                lineItemLeafObject.insert("lineColor", item->getColor());
                lineJsonArray.append(lineItemLeafObject);
            }

            leafObject.insert("stationName", info->stationName());
            leafObject.insert("stationCode", info->stationCode());
            leafObject.insert("lines", lineJsonArray);
            subJsonArray.append(leafObject);
        }

        branchObject.insert("lineName", line->getName());
        branchObject.insert("lineCode", line->getCode());
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
        // 线路编码
        if (!jsonObject.contains("lineCode") || !jsonObject.value("lineCode").isString()) {
            continue;
        }
        QString lineCode = jsonObject.value("lineCode").toString();
        // 线路名称
        if (!jsonObject.contains("lineColor") || !jsonObject.value("lineColor").isString()) {
            continue;
        }
        QString lineColor = jsonObject.value("lineColor").toString();
        LineInterchangeInfo* line = new LineInterchangeInfo(lineName, lineCode, "", lineColor);

        // 线路换乘信息
        if (!jsonObject.contains("interchanges") || !jsonObject.value("interchanges").isArray()) {
            lines.append(line);
            continue;
        }

        QJsonArray subJsonArray = jsonObject.value( "interchanges" ).toArray();
        for(auto subIter = subJsonArray.constBegin(); subIter != subJsonArray.constEnd(); ++subIter)
        {
            QJsonObject subJsonObject = (*subIter).toObject();
            if (!subJsonObject.contains("stationName") || !subJsonObject.value("stationName").isString() ||
                !subJsonObject.contains("stationCode") || !subJsonObject.value("stationCode").isString() ||
                !subJsonObject.contains("lines") || !subJsonObject.value("lines").isArray()) {
                continue;
            }
            QString stationName = subJsonObject.value("stationName").toString();
            QString stationCode = subJsonObject.value("stationCode").toString();

            QList<LineInfo*> transLines;
            QJsonArray lineJsonArray = jsonObject.value( "lines" ).toArray();
            for(auto lineIter = lineJsonArray.constBegin(); lineIter != lineJsonArray.constEnd(); ++lineIter)
            {
                QJsonObject subJsonObject = (*subIter).toObject();
                if (subJsonObject.contains("lineName") && subJsonObject.value("lineName").isString() &&
                    subJsonObject.contains("lineColor") && subJsonObject.value("lineColor").isString() &&
                    subJsonObject.contains("lineCode") && subJsonObject.value("lineCode").isString()) {

                    LineInfo* info = new LineInfo(subJsonObject.value("lineName").toString(),
                                                  subJsonObject.value("lineCode").toString(),
                                                  "",
                                                  subJsonObject.value("lineColor").toString());
                    transLines.append(info);
                }
            }

            line->addInterchangeInfo(new InterchangeInfo(stationName, stationCode, transLines));
        }
        lines.append(line);
    }

    return lines;
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

    QList<InterchangeInfo* > stations;
    QJsonArray jsonArray = rootObject.value("lineInterchanges").toArray();
    for(auto iter = jsonArray.constBegin(); iter != jsonArray.constEnd(); ++iter)
    {
        QJsonObject jsonObject = (*iter).toObject();

        // 换乘站点名称
        if (!jsonObject.contains("stationName") || !jsonObject.value("stationName").isString()) {
            continue;
        }
        QString stationName = jsonObject.value("stationName").toString();
        // 换乘站点编号
        if (!jsonObject.contains("stationCode") || !jsonObject.value("stationCode").isString()) {
            continue;
        }
        QString stationCode = jsonObject.value("stationCode").toString();

        // 换乘线路信息容错
        if (!jsonObject.contains("lineCode") || !jsonObject.value("lineCode").isArray() ||
            !jsonObject.contains("lineName") || !jsonObject.value("lineName").isArray() ||
            !jsonObject.contains("lineColor") || !jsonObject.value("lineColor").isArray()) {
            continue;
        }

        // 换乘线路信息
        QList<LineInfo*> interLineList;
        QJsonArray codeJsonArray = jsonObject.value( "lineCode" ).toArray();
        QJsonArray nameJsonArray = jsonObject.value( "lineName" ).toArray();
        QJsonArray colorJsonArray = jsonObject.value( "lineColor" ).toArray();
        int size = codeJsonArray.size();
        if (size != nameJsonArray.size() || size != colorJsonArray.size()) {
            continue;
        }
        for(int i = 0; i < size; ++i)
        {
            if (codeJsonArray.at(i).isString() && nameJsonArray.at(i).isString() && colorJsonArray.at(i).isString()) {
                LineInfo* info = new LineInfo(nameJsonArray.at(i).toString(),
                                              codeJsonArray.at(i).toString(),
                                              "",
                                              colorJsonArray.at(i).toString());

                interLineList.append(info);
            }
        }

        InterchangeInfo* station = new InterchangeInfo(stationName, stationCode, interLineList);
        stations.append(station);
    }


    // 中间结果 -- 转换
    QList<LineInfo*> lineBasic = DataCenter::getThis()->getLineList();
    QListIterator<LineInfo*> it(lineBasic);
    while(it.hasNext())
    {
        LineInfo* item = it.next();
        LineInterchangeInfo* line = new LineInterchangeInfo(item->getName(), item->getCode(), "", item->getColor());

        // 匹配换乘列表
        QList<InterchangeInfo*> sList = matchInterchangeStations(stations, item->getCode());
        line->setInterchangeList(sList);

        lines.append(line);
    }

    return lines;
}

// 匹配换乘列表
QList<InterchangeInfo*> SettingCenter::matchInterchangeStations(QList<InterchangeInfo *> stations, QString lineCode)
{
    QList<InterchangeInfo*> result;

    QList<InterchangeInfo*>::const_iterator it;
    for (it = stations.constBegin(); it != stations.constEnd(); ++it) {
        InterchangeInfo* station = new InterchangeInfo((*it)->stationName(), (*it)->stationCode(), (*it)->lineList());
        if (station->isPartOfLine(lineCode)) {
            result.append(station);
        }
    }

    return result;
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


