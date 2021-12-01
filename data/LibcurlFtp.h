#ifndef LIBCURLFTP_H
#define LIBCURLFTP_H

#include <QObject>
#include <QUrl>
#include "curl.h"
#include "logger.h"
#include "propertyconfigurator.h"

typedef std::function<void(QString &localpath, QString &filename)> FileDownloadedCallBack;

class BomParamVersionInfo;
class LibcurlFtp : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit LibcurlFtp(QObject *parent = nullptr);

public:
//    void setHostPort(const QString &host, int port);
//    void setUserInfo(const QString &userName, const QString &password);

    int ftpDownload(QString remotePath, QString fileName, QString localPath);
    int ftpUpload(QString remotePath, QString fileName, QString localPath);

    //获得指定ftp目录的文件列表
    bool ftpList(QString &remotpath, QString &myfloderlist, QString &localpath,
                 QHash<int, long> fileFilterInfo, bool isForceUpdate);

private:
    void processLine(QString line, QString& fileName, QChar& type);
    bool paramFileFilter(QString fileName, QHash<int, long> fileFilterInfo, bool isForceUpdate,
                         BomParamVersionInfo* info);

private:
//    QUrl m_pUrl;
    FileDownloadedCallBack m_callBack;

signals:
    void downloadOk(QString fileName);
    void uploadOk(QString fileName);
    void error(QString error);

};

#endif // LIBCURLFTP_H
