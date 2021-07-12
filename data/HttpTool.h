#ifndef HTTPTOOL_H
#define HTTPTOOL_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "logger.h"
#include "propertyconfigurator.h"

class HttpTool : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit HttpTool(QObject *parent = nullptr);

    static HttpTool* getThis();
    static HttpTool* m_pInstance;

    // 请求响应
    void replyFinished(QNetworkReply *reply);
    // 请求基本信息
    void requestLineBaseInfo();
    void requestLineStations();
    void requestStationMap();
    void requestLineMap();
    void requestStationPreMap();
    void requestPrice(QString start, QString end);
    void requestInterchanges();
    void requestTimeTables();
    void requestHotIssues();
    void requestAnswer(QString question);


    // 来源于配置文件
    void setId(const QString &deviceId, const QString &stationId);
    void setServUrl(const QString &servIp, const long &port);

private:
    void post(QByteArray data, QString path);
    QString getUrl(QString path);
    QByteArray defaultParam();
    QByteArray stationParam();

    void parse(QJsonObject data);

private:
    QNetworkAccessManager* m_manager;
    QString m_servIp;
    long m_port;
    QString m_deviceId;
    QString m_stationId;

signals:
    void priceReceived(double price);
    void answerReceived(QString answer);
    void hotIssuesReceived(QList<QString> hotIssues);

};

#endif // HTTPTOOL_H
