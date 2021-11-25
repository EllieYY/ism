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

    int ftpDownload(const char* localFile, const char* targetUrl);
    int ftpUpload(const char* localFile, const char* targetUrl);
    QList<QString> getFileNameList(const char* targetUrl);

private:
    QUrl m_pUrl;

signals:

};

#endif // LIBCURLFTP_H
