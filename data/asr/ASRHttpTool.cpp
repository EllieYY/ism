#include "ASRHttpTool.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "AsrConfig.h"
#include "AsrResult.h"
#include "AsrError.h"
#include "AsrReturn.h"

ASRHttpTool* ASRHttpTool::m_pInstance = NULL;
ASRHttpTool::ASRHttpTool(QObject *parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, &QNetworkAccessManager::finished,
            this, &ASRHttpTool::replyFinished);
    connect(this, &ASRHttpTool::accessTokenOk,
            this, &ASRHttpTool::getAsrResult);
}

ASRHttpTool *ASRHttpTool::getThis()
{
    if (m_pInstance == NULL)
        m_pInstance = new ASRHttpTool();
    return m_pInstance;
}

void ASRHttpTool::postForVoiceToText(AsrConfig *config, QString audio)
{
//    // TODO:测试
//    emit asrResultOk("天气很好");
    m_config = config;
    m_audio = audio;
    getAcessToken();
}

void ASRHttpTool::getAsrResult()
{
    QDateTime timeCur = QDateTime::currentDateTime();
    if (m_expiredTime < timeCur.toTime_t()) {
        getAcessToken();
    }

    QString url = QString("https://%1:%2/v10/asr/freetalk/cn_16k_common/short_audio?appkey=%3")
            .arg(m_ip).arg(m_port).arg(m_appkey);
    logger()->info("post for asr: " + url);

    QByteArray data = packageAsrBody(m_config, m_audio);

    post(data, url);
}

void ASRHttpTool::getAcessToken()
{
//    qDebug() << "------------" << QSslSocket::supportsSsl();
//    qDebug() << "------------" << QSslSocket::sslLibraryBuildVersionString();

    QString url = QString("https://%1:%2/v10/auth/get-access-token?appkey=%3").arg(m_ip).arg(m_port).arg(m_appkey);
    logger()->info("get token: " + url);
    get(url);
}



// 响应处理
void ASRHttpTool::replyFinished(QNetworkReply *reply)
{
    // 获取http状态码
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid()) {
        logger()->info(QString("status code = %1").arg(statusCode.toInt()));
    }

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


    int code = statusCode.toInt();
    // 根节点 -- json格式和错误码判断
    QJsonObject rootObject = doc.object();
    if(rootObject.contains("token")) {
        parseToken(rootObject);
    } else if(rootObject.contains("traceToken")) {
        if (code == 200) {
            parseAsrRet(rootObject);
        } else {
            parseAsrError(rootObject);
        }
    } else {
        logger()->error("Invalid json format.");
        return;
    }

}

// 返回token解析
void ASRHttpTool::parseToken(QJsonObject jsonObject)
{
    if (!jsonObject.contains("token") || !jsonObject.value("token").isString() ||
        !jsonObject.contains("life")) {
        m_expiredTime = 0;
        emit accessTokenOk();
        return;
    }

    m_token = jsonObject.value("token").toString();
    uint expiredSeconds = jsonObject.value("life").toInt();

    QDateTime timeCur = QDateTime::currentDateTime();
    m_expiredTime = timeCur.toTime_t() + expiredSeconds;
    emit accessTokenOk();
}


// ASR结果解析
void ASRHttpTool::parseAsrRet(QJsonObject jsonObject)
{
    if (!jsonObject.contains("traceToken") || !jsonObject.value("traceToken").isString() ||
        !jsonObject.contains("result")) {
        logger()->error(QString("Invalid AsrRet. keys: %1").arg(jsonObject.keys().join(" | ")));
        return;
    }

    QString traceToken = jsonObject.value("traceToken").toString();
    QJsonObject resultObj = jsonObject.value("result").toObject();
    if (!resultObj.contains("text") || !resultObj.value("text").isString() ||
        !resultObj.contains("confidence")) {
        logger()->error(QString("Invalid AsrRet. result keys: %1").arg(resultObj.keys().join(" | ")));
        return;
    }
    QString text = resultObj.value("text").toString();
    float confidence = resultObj.value("confidence").toDouble();
    logger()->info(QString("result[traceToken: %3, text: %1,  confidence: %2]").arg(text).arg(confidence).arg(traceToken));


    AsrResult* result = new AsrResult();
    result->setText(text);
    result->setConfidence(confidence);
    emit asrResultOk(text);
}

void ASRHttpTool::parseAsrError(QJsonObject jsonObject)
{
    if (!jsonObject.contains("traceToken") || !jsonObject.value("traceToken").isString() ||
        !jsonObject.contains("error")) {
        logger()->error(QString("Invalid AsrError. keys: %1").arg(jsonObject.keys().join(" | ")));
        return;
    }

    QString traceToken = jsonObject.value("traceToken").toString();
    QJsonObject errorObj = jsonObject.value("error").toObject();
    if (!errorObj.contains("code") ||
        !errorObj.contains("message") || !errorObj.value("message").isString()) {
        logger()->error(QString("Invalid AsrRet. result keys: %1").arg(errorObj.keys().join(" | ")));
        return;
    }

    int code = errorObj.value("code").toInt();
    QString message = errorObj.value("message").toString();
    logger()->info(QString("error[traceToken: %3, code: %1,  message: %2]")
                   .arg(code).arg(message).arg(traceToken));
}


// 通用 post请求
void ASRHttpTool::post(QByteArray data, QString path)
{
//    logger()->info("post: " + data);

    QUrl url(path);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setRawHeader(QByteArray("X-Hci-Access-Token"), m_token.toUtf8());

    // 发送https请求前准备工作;
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    conf.setProtocol(QSsl::TlsV1SslV3);
    request.setSslConfiguration(conf);


    QNetworkReply *reply = m_manager->post(request,data);
}

// 通用 get请求 basic auth
void ASRHttpTool::get(QString path)
{
    QUrl url(path);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setRawHeader("Authorization", getAuthorizationInfo().toUtf8());

//    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
//    config.setProtocol(QSsl::AnyProtocol);
//    config.setPeerVerifyMode(QSslSocket::VerifyNone);
//    request.setSslConfiguration(config);

    // 发送https请求前准备工作;
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    conf.setProtocol(QSsl::TlsV1SslV3);
    request.setSslConfiguration(conf);

    QNetworkReply *reply = m_manager->get(request);
}

// Basic Auth
QString ASRHttpTool::getAuthorizationInfo()
{
    QString userPwd = QString("%1:%2").arg(m_appkey).arg(m_secret);
    QString userPwdBase64 = userPwd.toUtf8().toBase64();
    QString ret = QString("Basic %1").arg(userPwdBase64);
    return ret;
}


QByteArray ASRHttpTool::packageAsrBody(AsrConfig* config, QString audio)
{
    QJsonDocument doc;
    QJsonObject rootObject;
    QJsonObject branchObject;
    branchObject.insert("audioFormat", config->audioFormat());
    branchObject.insert("addPunc", config->isAddPunc());

    rootObject.insert("config", branchObject);
    rootObject.insert("audio", audio);
    doc.setObject(rootObject);
    return doc.toJson(QJsonDocument::Compact);
}

void ASRHttpTool::setPort(long port)
{
    m_port = port;
}

void ASRHttpTool::setIp(const QString &ip)
{
    m_ip = ip;
}

void ASRHttpTool::setSecret(const QString &secret)
{
    m_secret = secret;
}

void ASRHttpTool::setAppkey(const QString &appkey)
{
    m_appkey = appkey;
}

