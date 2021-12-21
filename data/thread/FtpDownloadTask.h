#ifndef FTPDOWNLOADTASK_H
#define FTPDOWNLOADTASK_H

#include "Task.h"
#include <QUrl>
#include <QHash>

class LibcurlFtp;
class FtpDownloadTask : public CTask
{
public:
    explicit FtpDownloadTask(int taskId);
    ~FtpDownloadTask();

    void setFileInfo(QString remotePath, QString fileName, QString localPath);
    void setFilter(QHash<int, long> filter, bool isForceUpdate);

    int doWork() override;
    QString message() override;
    int taskId() override;
    bool result() override;

private:
    LibcurlFtp* m_ftp;

    QString m_remotePath;
    QString m_localPath;
    QString m_fileName;

    bool m_isFileList;     // 是否是拉取文件列表
    QHash<int,long> m_filter;    // 参数文件过滤条件
    bool m_isForceUpdate;     // 是否是强制同步

    int m_id;
    bool m_result;
};

#endif // FTPDOWNLOADTASK_H
