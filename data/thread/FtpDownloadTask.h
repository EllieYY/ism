#ifndef FTPDOWNLOADTASK_H
#define FTPDOWNLOADTASK_H

#include "Task.h"
#include <QUrl>

class LibcurlFtp;
class FtpDownloadTask : public CTask
{
public:
    explicit FtpDownloadTask(int taskId);
    ~FtpDownloadTask();

    void setFileInfo(QUrl server, QString localPath);

    int doWork() override;
    QString message() override;

private:
    LibcurlFtp* m_ftp;
    QUrl m_ftpUrl;
    QString m_localPath;
    int m_id;
};

#endif // FTPDOWNLOADTASK_H
