#include "HttpTool.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "SettingCenter.h"
#include "DataCenter.h"

HttpTool* HttpTool::m_pInstance = NULL;
HttpTool::HttpTool(QObject *parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);  //新建QNetworkAccessManager对象
    connect(m_manager, &QNetworkAccessManager::finished,
            this, &HttpTool::replyFinished);
}

HttpTool *HttpTool::getThis()
{
    if (m_pInstance == NULL)
        m_pInstance = new HttpTool();
    return m_pInstance;
}

// 响应处理
void HttpTool::replyFinished(QNetworkReply *reply)
{
    // 获取http状态码
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
        logger()->info(QString("status code = %1").arg(statusCode.toInt()));

    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if(reason.isValid())
        logger()->info("reason = " + reason.toString());

    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError) {
        logger()->error("Failed: " + reply->errorString());
        return;
    }


    // 获取返回内容
    QJsonParseError jsonParseError;
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(), &jsonParseError);
    if(QJsonParseError::NoError != jsonParseError.error) {
        logger()->error("JsonParseError:" + jsonParseError.errorString());
        return;
    }
    if (doc.isNull() || doc.isEmpty()) {
        logger()->error("No content.");
        return;
    }

    // 根节点 -- json格式和错误码判断
    QJsonObject rootObject = doc.object();
    if(rootObject.contains("code") && rootObject.contains("msg")) {
        int rtCode = rootObject.value("code").toInt();
        if (rtCode != 200) {
            logger()->error(QString("Request error: code = %1, msg = %2")
                            .arg(rtCode).arg(rootObject.value("msg").toString()));
            return;
        }
    } else {
        logger()->error("Invalid json format.");
        return;
    }

    // 根节点 -- 返回数据
    if(!rootObject.contains("data")) {
//        logger()->error("Invalid json format.");
        return;
    }

    parse(rootObject.value("data").toObject());

}

// post -- 线路基础信息
void HttpTool::requestLineBaseInfo()
{
    QByteArray param = defaultParam();
    post(param, "lineBaseInfo");
}

// post -- 线路站点信息
void HttpTool::requestLineStations()
{
    QByteArray param = defaultParam();
    post(param, "stationInfo");
}

// post -- 站点导览
void HttpTool::requestStationMap()
{
    QByteArray param = stationParam();
    post(param, "stationMapInfo");
}

// post -- 线路图
void HttpTool::requestLineMap()
{
    QByteArray param = defaultParam();
    post(param, "lineMapInfo");
}

// post -- 站点周边
void HttpTool::requestStationPreMap()
{
    QByteArray param = stationParam();
    post(param, "stationPreMapInfo");
}

// post -- 票价查询
void HttpTool::requestPrice(QString start, QString end)
{
    QJsonObject obj;
    QJsonDocument docum;
    obj.insert("deviceid", m_deviceId);
    obj.insert("beginStationCode", start);
    obj.insert("endStationCode", end);
    docum.setObject(obj);

    QByteArray param = docum.toJson(QJsonDocument::Compact);

    post(param, "priceInfo");
}

// post -- 换乘查询
void HttpTool::requestInterchanges()
{
    QByteArray param = defaultParam();
    post(param, "gainTransferStation");
}

// post -- 运营时刻表
void HttpTool::requestTimeTables()
{
    QByteArray param = defaultParam();
    post(param, "timeInfo");
}

// post -- 热门问题
void HttpTool::requestHotIssues()
{
    QByteArray param = defaultParam();
    post(param, "hotIssues");
}

// post -- 智能问答
void HttpTool::requestAnswer(QString question, QString seesionId)
{
    QJsonObject obj;
    QJsonDocument docum;
    obj.insert("deviceId", m_deviceId);
    obj.insert("sessionId", seesionId);
    obj.insert("question", question);
    docum.setObject(obj);
    QByteArray param = docum.toJson(QJsonDocument::Compact);

    post(param, "intelligentQA");
}

void HttpTool::updateStates(QString readerState)
{
    QJsonObject obj;
    QJsonDocument docum;
    obj.insert("deviceId", m_deviceId);
    obj.insert("writer", readerState);
    docum.setObject(obj);
    QByteArray param = docum.toJson(QJsonDocument::Compact);

    post(param, "deviceStateUpload");

}

void HttpTool::updateTicketInfo(QString type, QString detail, QString state)
{
    QJsonObject obj;
    QJsonDocument docum;
    obj.insert("errorState", type);
    obj.insert("tradeDetail", detail);
    obj.insert("saveStatus", state);
    docum.setObject(obj);
    QByteArray param = docum.toJson(QJsonDocument::Compact);

    post(param, "ticketDataUpload");
}

void HttpTool::setId(const QString &deviceId, const QString &stationId)
{
    m_deviceId = deviceId;
    m_stationId = stationId;
}

void HttpTool::setServUrl(const QString &servIp, const long &port)
{
    m_servIp = servIp;
    m_port = port;
}

// 通用post请求
void HttpTool::post(QByteArray data, QString path)
{
    QString addr = getUrl(path);
    QUrl url(addr);
    QNetworkRequest request(url);

    logger()->info("post: " + addr);
    logger()->info("body: " + data);

    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    QNetworkReply *reply = m_manager->post(request, data);
    qDebug() << reply->error();
}

// 智能客服后台访问路径设置
QString HttpTool::getUrl(QString path)
{
    return QString("http://%1:%2/ism/%3")
            .arg(m_servIp).arg(m_port).arg(path);
}

// 默认预留参数
QByteArray HttpTool::defaultParam()
{
    QJsonObject obj;
    QJsonDocument docum;
    obj.insert("deviceid", m_deviceId);
    docum.setObject(obj);

    return docum.toJson(QJsonDocument::Compact);
}

// 站点参数
QByteArray HttpTool::stationParam()
{
    QJsonObject obj;
    QJsonDocument docum;
    obj.insert("deviceid", m_deviceId);
    obj.insert("stationCode", "0120");
    docum.setObject(obj);

    return docum.toJson(QJsonDocument::Compact);
}

// ism后台返回结果解析
void HttpTool::parse(QJsonObject data)
{
    logger()->info("data: " + QJsonDocument(data).toJson(QJsonDocument::Compact));

    if(data.isEmpty()) {
        return;
    }

    // 线路基本信息
    if (data.contains("lineBasic")) {
        QList<LineInfo *> list = SettingCenter::getThis()->parseLineBasicInfo(data);
        DataCenter::getThis()->setLineList(list);
        return;
    }

    // 线路站点
    if (data.contains("lineStations")) {
        QList<LineStations *> list = SettingCenter::getThis()->parseLineStation(data);
        DataCenter::getThis()->setLineStations(list);
        return;
    }

    // 换乘信息
    if (data.contains("lineInterchanges")) {
        QList<LineInterchangeInfo *> list = SettingCenter::getThis()->parseLineInterchanes(data);
        DataCenter::getThis()->setLineInterchanges(list);
        return;
    }

    // 运营时间表
    if (data.contains("lineTimeTables")) {
        QList<LineTimeTables *> list = SettingCenter::getThis()->parseLineTimeTables(data);
        DataCenter::getThis()->setLineTimeTables(list);
        return;
    }

    // 站点导览图
    if (data.contains("mapBase") && data.value("mapBase").isString()) {
        QString base64Str = data.value("mapBase").toString();
        QPixmap image;
        image.loadFromData(QByteArray::fromBase64(base64Str.toLocal8Bit()));

        QString filePath = QDir::currentPath() + QDir::separator() +
                "images/info/map.png";

        image.save(filePath, "png", 100);
        return;
    }

    // 线路图
    parseLineMaps(data);

    // 站点周边
    if (data.contains("stationPreMapBase") && data.value("stationPreMapBase").isString()) {
        QString base64Str = data.value("stationPreMapBase").toString();
        QPixmap image;
        image.loadFromData(QByteArray::fromBase64(base64Str.toLocal8Bit()));

        QString filePath = QDir::currentPath() + QDir::separator() + "images/info/metroPer.png";

        image.save(filePath, "png", 100);
        return;
    }

    // 票价查询
    if (data.contains("price") && data.value("price").isString()) {
        double price = data.value("price").toString().toDouble();
        emit priceReceived(price);
        return;
    }

    // 热点问题
    if (data.contains("hotIssues") && data.value("hotIssues").isArray()) {
        QJsonArray jsonArray = data.value("hotIssues").toArray();
        QList<QString> hotIssues;
        int count = 1;
        for(auto iter = jsonArray.constBegin(); iter != jsonArray.constEnd(); ++iter)
        {
            QString issue = (*iter).toString();
            hotIssues.append(issue);
            if (count++ >= 5) {
                break;
            }
        }
        emit hotIssuesReceived(hotIssues);
        return;
    }

    // 智能问答
    if (data.contains("answer") && data.value("answer").isString()) {
        QString answer = data.value("answer").toString();
        emit answerReceived(answer);
        return;
    }

    logger()->info("no target keys.");
}

void HttpTool::parseLineMaps(QJsonObject data)
{
    if (data.contains("lineMaps") && data.value("lineMaps").isArray()) {
        QJsonArray jsonArray = data.value("lineMaps").toArray();
        for(auto iter = jsonArray.constBegin(); iter != jsonArray.constEnd(); ++iter)
        {
            QJsonObject jsonObject = (*iter).toObject();
            if (!jsonObject.contains("lineMap") ||
                    !jsonObject.value("lineMap").isString() ||
                    !jsonObject.contains("lineCode")) {
                continue;
            }

            QString lineCode = jsonObject.value("lineCode").toString();
            QString picName = "line" + lineCode + ".png";
            DataCenter::getThis()->setLineMap(lineCode, picName);

            // save file
            QString base64Str = jsonObject.value("lineMap").toString();
            QPixmap image;
            image.loadFromData(QByteArray::fromBase64(base64Str.toLocal8Bit()));
            QString filePath = QDir::currentPath() + QDir::separator() +
                    "images/info/" + picName;

            image.save(filePath, "png", 100);

            logger()->info(filePath);
        }

        SettingCenter::getThis()->saveLineBasicInfo(DataCenter::getThis()->getLineList());

        return;
    }
}
