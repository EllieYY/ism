/* 配置中心
 * 各种信息与json文件的转换
 *
*/
#include "SettingCenter.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QUrl>

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
#include "TradeFileInfo.h"
#include "CommonHead.h"
#include "ReaderSoftFileInfo.h"
#include "UpdateParamInfo.h"
#include "LineStationTimetables.h"
#include "StationTime.h"
#include "X7000FileInfo.h"

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
void SettingCenter::saveTradeFileInfo(TradeFileInfo* info)
{
    if (info == nullptr) {
        return;
    }
    QJsonObject rootObject;

    rootObject.insert("datetime", info->datetime().toString("yyyy-MM-dd HH:mm:ss"));
    rootObject.insert("fileSerialNo", QString::number(info->fileTradeSerial()));
    rootObject.insert("deviceSerialNo", QString::number(info->deviceTradeSerial()));
    rootObject.insert("fileCount", info->fileCount());
    QSet<QString> fileList = info->fileNameSet();

    QJsonArray jsonArray;
    for (QString name:fileList) {
        jsonArray.append(name);
    }
    rootObject.insert("fileList", jsonArray);

    saveJsonFile(rootObject, "sc/transcation/tradeFileInfo.json");
}

TradeFileInfo* SettingCenter::getTradeFileInfo()
{
    TradeFileInfo* info = new TradeFileInfo(this);

    QString fileName = QString("sc/transcation/tradeFileInfo.json");

    QJsonDocument jsonDocument = readJsonFile(fileName);
    if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
        return info;
    }
    QJsonObject rootObject = jsonDocument.object();
    QList<LineInfo *> lines;
    if(!rootObject.contains("datetime") || !rootObject.value("datetime").isString() ||
            !rootObject.contains("fileSerialNo") || !rootObject.value("fileSerialNo").isString() ||
            !rootObject.contains("deviceSerialNo") || !rootObject.value("deviceSerialNo").isString() ||
            !rootObject.contains("fileCount"))
    {
        qDebug() << "No target value";
        qDebug() << rootObject.keys();
        return info;
    }


    int fileCount = rootObject.value("fileCount").toInt();
    ulong fileSerial = rootObject.value("fileSerialNo").toString().toULong();
    ulong devTradeSerial = rootObject.value("deviceSerialNo").toString().toULong();
    QString timeStr = rootObject.value("datetime").toString();
    QDateTime dDate = QDateTime::fromString(timeStr, "yyyy-MM-dd HH:mm:ss");
    QDateTime curDate = QDateTime::currentDateTime();

    QJsonArray jsonArray = rootObject.value("fileList").toArray();
    QSet<QString> fileList;
    for(auto iter = jsonArray.constBegin(); iter != jsonArray.constEnd(); ++iter)
    {
        QString name = iter->toString();
        fileList.insert(name);
    }

    // 新的一天，序号从1开始
    if (dDate.daysTo(curDate) > 0) {
        info->setDatetime(curDate);
        info->setFileTradeSerial(1);
        info->setDeviceTradeSerial(1);
        info->setFileCount(0);
    } else {    // 当天读取，序号延续之前的，考虑程序崩溃重启
        info->setDatetime(dDate);
        info->setFileTradeSerial(fileSerial);
        info->setDeviceTradeSerial(devTradeSerial);
        info->setFileNameSet(fileList);
        info->setFileCount(fileCount);
    }

    return info;
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
    info->setTradeFileDays(rootObject.value("tradeFileDays").toInt());

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

    // FTP信息
    if(rootObject.contains("ftpInfo"))
    {
        QJsonObject item = rootObject.value("ftpInfo").toObject();

        QUrl url;
        url.setScheme("ftp");
        url.setHost(item.value("ip").toString());
        url.setPort(item.value("port").toInt());
        url.setUserName(item.value("user").toString());
        url.setPassword(item.value("pwd").toString());
        url.setPath(item.value("path").toString());

        info->setFtpUrl(url);
    }

    return info;
}

void SettingCenter::saveParamVersionInfo(QList<BomParamVersionInfo *> list, QString fileName)
{
    if (list.size() <= 0) {
        return;
    }

    QJsonArray jsonArray;
    QJsonObject rootObject;
    QJsonObject branchObject;
    for (BomParamVersionInfo* item : list) {
        branchObject.insert("type", (int)item->type());
        branchObject.insert("version", (int)item->version());
        branchObject.insert("preVersion", (int)item->preVersion());
        branchObject.insert("fileName", item->fileName());
        branchObject.insert("preFileName", item->preFileName());
        branchObject.insert("isReaderUse", item->readerUsed());
        branchObject.insert("isReaderUpdated", item->readerUpdated());
        jsonArray.append(branchObject);
        clearJsonObject(branchObject);
    }

    rootObject.insert("versions", jsonArray);

    QString filePath = QString("%1/%2").arg(PARAM_FILE_PATH).arg(fileName);
    saveJsonFile(rootObject, filePath);
}

QList<BomParamVersionInfo *> SettingCenter::getParamVersionInfo(QString filePath)
{
    QList<BomParamVersionInfo *> list;
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

            BomParamVersionInfo* item = new BomParamVersionInfo();
            item->setType(jsonObject.value("type").toInt());
            item->setVersion(jsonObject.value("version").toInt());
            item->setFileName(jsonObject.value("fileName").toString());

            if (jsonObject.contains("preVersion") &&
                    jsonObject.contains("isReaderUse") && jsonObject.contains("isReaderUpdated") &&
                    jsonObject.contains("preFileName") && jsonObject.value("preFileName").isString()) {
                item->setPreVersion(jsonObject.value("preVersion").toInt());
                item->setPreFileName(jsonObject.value("preFileName").toString());
                item->setReaderUsed(jsonObject.value("isReaderUse").toBool());
                item->setReaderUpdated(jsonObject.value("isReaderUpdated").toBool());
            }

            list.append(item);
        }
    }

    return list;
}

void SettingCenter::saveUpdateParamInfo(QList<UpdateParamInfo *> list, QString fileName)
{
    if (list.size() <= 0) {
        return;
    }

    QJsonArray jsonArray;
    QJsonObject rootObject;
    QJsonObject branchObject;
    for (UpdateParamInfo* item : list) {
        branchObject.insert("type", (int)item->type());
        branchObject.insert("version", (int)item->version());
        branchObject.insert("fileName", item->fileName());
        branchObject.insert("isFileOk", item->fileOk());
        branchObject.insert("hasUpdated", item->updated());
        jsonArray.append(branchObject);
        clearJsonObject(branchObject);
    }

    rootObject.insert("versions", jsonArray);

    QString filePath = QString("%1/%2").arg(PARAM_FILE_PATH).arg(fileName);
    saveJsonFile(rootObject, filePath);
}

QList<UpdateParamInfo *> SettingCenter::getUpdateParamInfo(QString filePath)
{
    QList<UpdateParamInfo *> list;
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
                jsonObject.contains("isFileOk") && jsonObject.contains("hasUpdated") &&
                jsonObject.contains("fileName") && jsonObject.value("fileName").isString()) {

            UpdateParamInfo* item = new UpdateParamInfo();
            item->setType(jsonObject.value("type").toInt());
            item->setVersion(jsonObject.value("version").toInt());
            item->setFileName(jsonObject.value("fileName").toString());

            item->setFileOk(jsonObject.value("isFileOk").toBool());
            item->setUpdated(jsonObject.value("hasUpdated").toBool());

            list.append(item);
        }
    }

    return list;
}


// 下载失败的文件
void SettingCenter::saveDownloadFailedFiles(QList<QString> list)
{
    if (list.size() <= 0) {
        return;
    }
    QJsonObject rootObject;
    QJsonArray jsonArray;
    for (QString item : list) {
        jsonArray.append(item);
    }

    rootObject.insert("files", jsonArray);

    QString filePath = QString("%1/failedParams.json").arg(PARAM_FILE_PATH);
    saveJsonFile(rootObject, filePath);
}

QList<QString> SettingCenter::getDownloadFailedFiles()
{
    QString filePath = QDir::currentPath() + QDir::separator() + QString(PARAM_FILE_PATH) + "/failedParams.json";
    QList<QString> list;
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
    if(!rootObject.contains("files") || !rootObject.value("files").isArray())
    {
        qDebug() << "No target value";
        qDebug() << rootObject.keys();
        return list;
    }

    QJsonArray jsonArray = rootObject.value("files").toArray();
    for(auto iter = jsonArray.constBegin(); iter != jsonArray.constEnd(); ++iter)
    {
        QString name = (*iter).toString();
        list.append(name);
    }

    return list;
}


// 读写器程序更新文件
void SettingCenter::saveReaderSoftInfo(ReaderSoftFileInfo *info)
{
    if (info == nullptr) {
        return;
    }

    QJsonObject rootObject;
    rootObject.insert("fileName", info->getFileName());
    rootObject.insert("date", info->getDateString());
    rootObject.insert("hasUpdated", info->getHasUpdated());
    rootObject.insert("fileReady", info->getFileReady());

    QString filePath = QString("%1/updateVersion.json").arg(SOFT_FILE_PATH);
    saveJsonFile(rootObject, filePath);

}

ReaderSoftFileInfo *SettingCenter::getReaderSoftInfo()
{
    ReaderSoftFileInfo* info = NULL;
    QString filePath = QDir::currentPath() + QDir::separator() + QString(SOFT_FILE_PATH) + "/updateVersion.json";

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << QString("fail to open the file: %1, %2, %3")
                    .arg(__FILE__).arg(__LINE__).arg(__FUNCTION__);
        return info;
    }
    QByteArray array = file.readAll();
    file.close();

    QJsonParseError jsonParseError;
    QJsonDocument jsonDocument(QJsonDocument::fromJson(array, &jsonParseError));
    if(QJsonParseError::NoError != jsonParseError.error)
    {
        qDebug() << QString("JsonParseError: %1").arg(jsonParseError.errorString());
        return info;
    }

    QJsonObject rootObject = jsonDocument.object();
    if(!rootObject.contains("fileName") || !rootObject.value("fileName").isString())
    {
        qDebug() << "No target value";
        qDebug() << rootObject.keys();
        return info;
    }

    info = new ReaderSoftFileInfo(this);
    info->setFileName(rootObject.value("fileName").toString());
    info->setHasUpdated(rootObject.value("hasUpdated").toBool());
    info->setFileReady(rootObject.value("fileReady").toBool());

    return info;
}

QMap<int, LineStationTimetables *> SettingCenter::getLineStationTimetables()
{
    QMap<int, LineStationTimetables *> lines;
    QJsonDocument jsonDocument = readJsonFile("lineStationTimes.json");
    if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
        return lines;
    }
    QJsonObject rootObject = jsonDocument.object();
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

        // 线路编号
        if (!jsonObject.contains("lineCode")) {
            continue;
        }
        int code = jsonObject.value("lineCode").toInt();
        LineStationTimetables* line = new LineStationTimetables(this);
        line->setLineCode(code);
        line->setDirA(jsonObject.value("dirA").toString());
        line->setDirB(jsonObject.value("dirB").toString());

        // 线路运营时间表
        if (!jsonObject.contains("stations") || !jsonObject.value("stations").isArray()) {
            lines.insert(code, line);
            continue;
        }
        QJsonArray subJsonArray = jsonObject.value( "stations" ).toArray();
        for(auto subIter = subJsonArray.constBegin(); subIter != subJsonArray.constEnd(); ++subIter)
        {
            QJsonObject subJsonObject = (*subIter).toObject();
            if (subJsonObject.contains("name") && subJsonObject.value("name").isString() &&
                subJsonObject.contains("endTimeA") && subJsonObject.value("endTimeA").isString() &&
                subJsonObject.contains("endTimeB") && subJsonObject.value("endTimeB").isString() &&
                subJsonObject.contains("startTimeA") && subJsonObject.value("startTimeA").isString() &&
                subJsonObject.contains("startTimeB") && subJsonObject.value("startTimeB").isString()) {

                StationTime* station = new StationTime(this);
                station->setStationName(subJsonObject.value("name").toString());
                station->setAEndTime(subJsonObject.value("endTimeA").toString());
                station->setBEndTime(subJsonObject.value("endTimeB").toString());
                station->setAStartTime(subJsonObject.value("startTimeA").toString());
                station->setBStartTime(subJsonObject.value("startTimeB").toString());

                line->addStationTime(station);
            }
        }
        lines.insert(code, line);
    }
    return lines;
}

void SettingCenter::saveLineStationTimetables(QList<LineStationTimetables *> lines)
{
    QJsonArray jsonArray;
    QJsonObject rootObject;
    QJsonObject branchObject;
    QJsonObject leafObject;
    for (LineStationTimetables* line:lines) {
        QJsonArray subJsonArray;
        QList<StationTime*> timeTables = line->stationTimes();
        for (StationTime* timeTable:timeTables) {
            leafObject.insert("name", timeTable->stationName());
            leafObject.insert("endTimeA", timeTable->aEndTime());
            leafObject.insert("endTimeB", timeTable->bEndTime());
            leafObject.insert("startTimeA", timeTable->aStartTime());
            leafObject.insert("startTimeB", timeTable->bStartTime());
            subJsonArray.append(leafObject);
        }

        branchObject.insert("lineCode", line->lineCode());
        branchObject.insert("dirA", line->dirA());
        branchObject.insert("dirB", line->dirB());
        branchObject.insert("stations", subJsonArray);
        jsonArray.append(branchObject);

        clearJsonObject(leafObject);
        clearJsonObject(branchObject);
    }

    rootObject.insert("lineTimeTables", jsonArray);
    saveJsonFile(rootObject, "lineStationTimes.json");
}


// 测试用--读取文件设置运营结束时间
long SettingCenter::getTestServiceOffTime()
{
    QTime time = QTime::fromString("01:30", "HH:mm");
    int serviceOffTime = 0.001 * time.msecsSinceStartOfDay();

    QMap<int, LineStationTimetables *> lines;
    QJsonDocument jsonDocument = readJsonFile("serviceOffTime.json");
    if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
        return serviceOffTime;
    }

    QJsonObject rootObject = jsonDocument.object();
    if(!rootObject.contains("time") || !rootObject.value("time").isString())
    {
        return serviceOffTime;
    }

    QString timeStr = rootObject.value("time").toString();
    QTime newTime = QTime::fromString(timeStr, "HH:mm");
    serviceOffTime = 0.001 * newTime.msecsSinceStartOfDay();

    qDebug() << "运营结束时间：" << serviceOffTime;
    return serviceOffTime;
}

// 交易信息维护
void SettingCenter::addTradeFileInfo(X7000FileInfo *info)
{
    QDateTime curTime = QDateTime::currentDateTime();
    QString curTimeStr = curTime.toString("yyyyMMdd");
    QString fileName = QString("%1/TradeFileInfo_%2.json").arg(TRADE_FILE_PATH).arg(curTimeStr);

    QJsonDocument jsonDocument = readJsonFile(fileName);
    QJsonObject rootObject;
    QJsonArray jsonArray;
    if (!jsonDocument.isNull() && !jsonDocument.isEmpty()) {
        rootObject = jsonDocument.object();
        if(rootObject.contains("tradeFiles") && rootObject.value("tradeFiles").isArray()) {
            jsonArray = rootObject.value("versions").toArray();
        }
    }

    QJsonObject leafObject;
    leafObject.insert("fileName", info->fileName());
    QString md5Str = info->md5Arr().toHex();
    leafObject.insert("md5Str", md5Str);
    leafObject.insert("type", info->type());

    jsonArray.append(leafObject);
    rootObject.insert("tradeFiles", jsonArray);

    saveJsonFile(rootObject, fileName);
}

void SettingCenter::deleteTradeFileInfo(QString md5Str)
{

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

    QString filePath = QDir::currentPath() + QDir::separator() + fileName;
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

    QString filePath = QDir::currentPath() + QDir::separator() + fileName;
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


