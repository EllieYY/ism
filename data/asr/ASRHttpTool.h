#ifndef ASRHTTPTOOL_H
#define ASRHTTPTOOL_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "logger.h"
#include "propertyconfigurator.h"

class AsrConfig;
class AsrResult;
class ASRHttpTool : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit ASRHttpTool(QObject *parent = nullptr);

    static ASRHttpTool* getThis();
    static ASRHttpTool* m_pInstance;

    void postForVoiceToText(AsrConfig* config, QString audio);

    void setAppkey(const QString &appkey);
    void setSecret(const QString &secret);
    void setIp(const QString &ip);
    void setPort(long port);

private:
    void getAcessToken();
    void getAsrResult();
    void parseToken(QJsonObject data);
    void parseAsrRet(QJsonObject data);
    void parseAsrError(QJsonObject data);

    // 请求响应
    void replyFinished(QNetworkReply *reply);

    void post(QByteArray data, QString path);
    void get(QString path);
    QString getAuthorizationInfo();
    QByteArray packageAsrBody(AsrConfig* config, QString audio);

private:
    QString m_appkey;
    QString m_secret;
    QString m_ip;
    long m_port;

    QNetworkAccessManager* m_manager;

    QString m_token;
    long m_expiredTime;    // token失效时间

    AsrConfig* m_config;
    QString m_audio;

signals:
    void accessTokenOk();
    void asrResultOk(QString text);

};

#endif // ASRHTTPTOOL_H
