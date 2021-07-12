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
        int rtCode = rootObject.value("").toInt();
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
        logger()->error("Invalid json format.");
        return;
    }

    parse(rootObject.value("data").toObject());

}

void HttpTool::requestLineBaseInfo()
{
    QByteArray param = defaultParam();
    post(param, "lineBaseInfo");
}

void HttpTool::requestLineStations()
{
    QByteArray param = defaultParam();
    post(param, "stationInfo");
}

void HttpTool::requestStationMap()
{
    QByteArray param = stationParam();
    post(param, "stationMapInfo");
}

void HttpTool::requestLineMap()
{
    QByteArray param = defaultParam();
    post(param, "lineMapInfo");
}

void HttpTool::requestStationPreMap()
{
    QByteArray param = stationParam();
    post(param, "stationPreMapInfo");
}

void HttpTool::requestPrice(QString start, QString end)
{
    QJsonObject obj;
    QJsonDocument docum;
    obj.insert("deviceid", m_deviceId);
    obj.insert("inStationNum", start);
    obj.insert("outStationNum", end);
    docum.setObject(obj);

    QByteArray param = docum.toJson(QJsonDocument::Compact);

    post(param, "priceInfo");
}

void HttpTool::requestInterchanges()
{
    QByteArray param = defaultParam();
    post(param, "gainTransferStation");
}

void HttpTool::requestTimeTables()
{
    QByteArray param = defaultParam();
    post(param, "timeInfo");
}

void HttpTool::requestHotIssues()
{
    QByteArray param = defaultParam();
    post(param, "hotIssues");
}

void HttpTool::requestAnswer(QString question)
{
    QJsonObject obj;
    QJsonDocument docum;
    obj.insert("deviceId", m_deviceId);
    obj.insert("question", question);
    docum.setObject(obj);
    QByteArray param = docum.toJson(QJsonDocument::Compact);

    post(param, "answer");
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

void HttpTool::post(QByteArray data, QString path)
{
    QString addr = getUrl(path);
    QUrl url(addr);
    QNetworkRequest request(url);  request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkReply *reply = m_manager->post(request,data);
    qDebug()<<reply->error();
}

QString HttpTool::getUrl(QString path)
{
    return QString("http://%1:%2/%3")
            .arg(m_servIp).arg(m_port).arg(path);
}

QByteArray HttpTool::defaultParam()
{
    QJsonObject obj;
    QJsonDocument docum;
    obj.insert("deviceid",m_deviceId);
    docum.setObject(obj);

    return docum.toJson(QJsonDocument::Compact);
}

QByteArray HttpTool::stationParam()
{
    QJsonObject obj;
    QJsonDocument docum;
    obj.insert("deviceId",m_deviceId);
    obj.insert("stationId",m_stationId);
    docum.setObject(obj);

    return docum.toJson(QJsonDocument::Compact);
}

void HttpTool::parse(QJsonObject data)
{
    if(data.isEmpty()) {
        return;
    }

    // 线路基本信息
    if (data.contains("lineBasic")) {
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
    if (data.contains("lineMaps") && data.value("lineMaps").isArray()) {
        QJsonArray jsonArray = data.value("lineMaps").toArray();
        for(auto iter = jsonArray.constBegin(); iter != jsonArray.constEnd(); ++iter)
        {
            QJsonObject jsonObject = (*iter).toObject();
            if (!jsonObject.contains("lineMap") ||
                    !jsonObject.value("lineMap").isString() ||
                    !jsonObject.contains("lineNo")) {
                continue;
            }

            int lineNo = jsonObject.value("lineNo").toInt();
            QString base64Str = data.value("lineMap").toString();
            QPixmap image;
            image.loadFromData(QByteArray::fromBase64(base64Str.toLocal8Bit()));
            QString filePath = QDir::currentPath() + QDir::separator() +
                    "images/info/line" + lineNo + ".png";

            image.save(filePath, "png", 100);
        }

        return;
    }

    // 站点周边
    if (data.contains("stationPerMap") && data.value("stationPerMap").isString()) {
        QString base64Str = data.value("stationPerMap").toString();
        QPixmap image;
        image.loadFromData(QByteArray::fromBase64(base64Str.toLocal8Bit()));

        QString filePath = QDir::currentPath() + QDir::separator() + "images/info/metroPer.png";

        image.save(filePath, "png", 100);
        return;
    }

    // 票价查询
    if (data.contains("price") && data.value("price").isDouble()) {
        double price = data.value("price").toDouble();
        emit priceReceived(price);
        return;
    }

    // 热点问题
    if (data.contains("hotIssues") && data.value("hotIssues").isArray()) {
        QJsonArray jsonArray = data.value("hotIssues").toArray();
        QList<QString> hotIssues;
        for(auto iter = jsonArray.constBegin(); iter != jsonArray.constEnd(); ++iter)
        {
            QString issue = (*iter).toString();
            hotIssues.append(issue);
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

}
