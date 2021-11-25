#ifndef LIBCURLFTP_H
#define LIBCURLFTP_H

#include <QObject>
#include <QUrl>
#include "curl.h"
#include "logger.h"
#include "propertyconfigurator.h"

class LibcurlFtp : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit LibcurlFtp(QObject *parent = nullptr);

public:
    void setHostPort(const QString &host, int port);
    void setUserInfo(const QString &userName, const QString &password);

    int ftpDownload(QString localFile, QString targetUrl);
    int ftpUpload(QString localFile, QString targetUrl);

private:
    QUrl m_pUrl;

signals:
    void downloadOk(QString fileName);
    void uploadOk(QString fileName);
    void error(QString error);

};

#endif // LIBCURLFTP_H
