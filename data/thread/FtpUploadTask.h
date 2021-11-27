#ifndef FTPUPLOADTASK_H
#define FTPUPLOADTASK_H

#include <QObject>
#include "Task.h"
#include <QUrl>

class LibcurlFtp;
class FtpUploadTask : public CTask
{
public:
    explicit FtpUploadTask(int taskId);
    ~FtpUploadTask();

    void setFileInfo(QUrl server, QString localPath);

    int doWork() override;
    QString message() override;

private:
    LibcurlFtp* m_ftp;
    QUrl m_ftpUrl;
    QString m_localPath;
    int m_id;

};

#endif // FTPUPLOADTASK_H
