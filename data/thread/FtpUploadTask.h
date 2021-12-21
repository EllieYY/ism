#ifndef FTPUPLOADTASK_H
#define FTPUPLOADTASK_H

#include "Task.h"

class LibcurlFtp;
class FtpUploadTask : public CTask
{
public:
    explicit FtpUploadTask(int taskId);
    ~FtpUploadTask();

    void setFileInfo(QString remotePath,  QString fileName, QString localPath);

    int doWork() override;
    QString message() override;
    int taskId() override;
    bool result() override;

private:
    LibcurlFtp* m_ftp;

    QString m_remotePath;
    QString m_localPath;
    QString m_fileName;
    int m_id;
    int m_result;

};

#endif // FTPUPLOADTASK_H
