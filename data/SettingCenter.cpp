/* 配置中心
 * 各种信息与json文件的转换
 *
*/
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
#include "ASRHttpTool.h"
#include "BasicInfo.h"
#include "BomParamVersionInfo.h"

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


// 交易序列号
void SettingCenter::saveTradeSerial(int serial)
{
    QJsonObject rootObject;

    rootObject.insert("datetime", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    rootObject.insert("serialNo", serial);

    saveJsonFile(rootObject, "tradeSerial.json");
}

int SettingCenter::getTradeSerial()
{
    int tradeSerial = 1;
    QJsonDocument jsonDocument = readJsonFile("tradeSerial.json");
    if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
        return tradeSerial;
    }
    QJsonObject rootObject = jsonDocument.object();
    QList<LineInfo *> lines;
    if(!rootObject.contains("datetime") || !rootObject.value("datetime").isString() ||
            !rootObject.contains("serialNo"))
    {
        qDebug() << "No target value";
        qDebug() << rootObject.keys();
        return tradeSerial;
    }

    QString timeStr = rootObject.value("datetime").toString();
    QDate dDate = QDateTime::fromString(timeStr, "yyyy-MM-dd HH:mm:ss").date();
    QDate curDate = QDate::currentDate();

    // 非当天记录
    if (dDate.daysTo(curDate) > 0) {
        return tradeSerial;
    }

    tradeSerial = rootObject.value("serialNo").toInt();
    return tradeSerial;
}


// 线路基本信息
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

    rootObject.insert("lineTimeTables", jsonArray);
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
            QJsonArray lineJsonArray = subJsonObject.value("lines").toArray();
            for(auto lineIter = lineJsonArray.constBegin(); lineIter != lineJsonArray.constEnd(); ++lineIter)
            {
                QJsonObject lineJsonObject = (*lineIter).toObject();
                if (lineJsonObject.contains("lineName") && lineJsonObject.value("lineName").isString() &&
                    lineJsonObject.contains("lineColor") && lineJsonObject.value("lineColor").isString() &&
                    lineJsonObject.contains("lineCode") && lineJsonObject.value("lineCode").isString()) {

                    LineInfo* info = new LineInfo(lineJsonObject.value("lineName").toString(),
                                                  lineJsonObject.value("lineCode").toString(),
                                                  "",
                                                  lineJsonObject.value("lineColor").toString());
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
BasicInfo* SettingCenter::getBasicInfo()
{
    QString filePath = QDir::currentPath() + QDir::separator() + QDir::separator() + "setting.json";
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << QString("fail to open the file: %1, %2, %3")
                    .arg(__FILE__).arg(__LINE__).arg(__FUNCTION__);
        return NULL;
    }
    QByteArray array = file.readAll();
    file.close();

    QJsonParseError jsonParseError;
    QJsonDocument jsonDocument(QJsonDocument::fromJson(array, &jsonParseError));
    if(QJsonParseError::NoError != jsonParseError.error)
    {
        qDebug() << QString("JsonParseError: %1").arg(jsonParseError.errorString());
        return NULL;
    }

    QJsonObject rootObject = jsonDocument.object();
    if(!rootObject.keys().contains("stationName"))
    {
        qDebug() << "No target value";
        return NULL;
    }

    BasicInfo* info = new BasicInfo(this);
    info->setStationName(rootObject.value("stationName").toString());
    info->setStationCode(rootObject.value("stationCode").toString());
    info->setDeviceId(rootObject.value("deviceId").toString());
    info->setIsPayZone(rootObject.value("isPayZone").toBool());

    // ism后台连接
    if(rootObject.contains("ismService"))
    {
        QJsonObject item = rootObject.value("ismService").toObject();
        info->setIsmServiceIp(item.value("ip").toString());
        info->setIsmServicePort(item.value("port").toInt());
    }

    // asr后台连接信息
    if(rootObject.contains("asrInfo"))
    {
        QJsonObject item = rootObject.value("asrInfo").toObject();
        info->setAsrServiceIp(item.value("asrIp").toString());
        info->setAsrServicePort(item.value("asrPort").toInt());
        info->setAppkey(item.value("appkey").toString());
        info->setSecret(item.value("secret").toString());
    }

    // 硬件设备信息
    if(rootObject.contains("deviceInfo"))
    {
        QJsonObject item = rootObject.value("deviceInfo").toObject();
        info->setReaderPort(item.value("readerPort").toInt());
        info->setAntiNo(item.value("anti").toInt());
        info->setBrcPort(item.value("brcPort").toInt());
        info->setF53Port(item.value("f53Port").toInt());
        info->setBimPort(item.value("bimPort").toInt());
    }

    // AFC通讯信息
    if(rootObject.contains("afcInfo"))
    {
        QJsonObject item = rootObject.value("afcInfo").toObject();
        info->setScId(item.value("scId").toString());
        info->setScIP(item.value("scIP").toString());
        info->setScPort(item.value("scPort").toInt());
        info->setLocalIP(item.value("localIp").toString());
        info->setLocalPort(item.value("localPort").toInt());
    }

    return info;
}

void SettingCenter::saveParamVersionInfo(QList<BomParamVersionInfo *> list)
{
    QJsonArray jsonArray;
    QJsonObject rootObject;
    QJsonObject branchObject;
    for (BomParamVersionInfo* item : list) {
        branchObject.insert("type", (int)item->type());
        branchObject.insert("version", (int)item->version());
        branchObject.insert("fileName", item->fileName());
        jsonArray.append(branchObject);
        clearJsonObject(branchObject);
    }

    rootObject.insert("versions", jsonArray);

    QString filePath = QString("bom-param%1version.json").arg(QDir::separator());
    saveJsonFile(rootObject, filePath);
}

QList<BomParamVersionInfo *> SettingCenter::getParamVersionInfo()
{
    QList<BomParamVersionInfo *> list;
    QString filePath = QDir::currentPath() + QDir::separator() + "bom-param" + QDir::separator() + "version.json";
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << QString("fail to open the file: %1, %2, %3")
                    .arg(__FILE__).arg(__LINE__).arg(__FUNCTION__);
        return list;
    }
    QByteArray array = file.readAll();
    file.close();

    QJsonParseError jsonParseError;
    QJsonDocument jsonDocument(QJsonDocument::fromJson(array, &jsonParseError));
    if(QJsonParseError::NoError != jsonParseError.error)
    {
        qDebug() << QString("JsonParseError: %1").arg(jsonParseError.errorString());
        return list;
    }

    QJsonObject rootObject = jsonDocument.object();
    if(!rootObject.contains("versions") || !rootObject.value("versions").isArray())
    {
        qDebug() << "No target value";
        qDebug() << rootObject.keys();
        return list;
    }

    QJsonArray jsonArray = rootObject.value("versions").toArray();
    for(auto iter = jsonArray.constBegin(); iter != jsonArray.constEnd(); ++iter)
    {
        QJsonObject jsonObject = (*iter).toObject();

        // 类型 | 版本号 | 文件名
        if (jsonObject.contains("type") && jsonObject.contains("version") &&
            jsonObject.contains("fileName") && jsonObject.value("fileName").isString()) {

            BomParamVersionInfo* item = new BomParamVersionInfo(
                        jsonObject.value("type").toInt(),
                        jsonObject.value("version").toInt(),
                        jsonObject.value("fileName").toString());

            list.append(item);
        }
    }

    return list;
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


