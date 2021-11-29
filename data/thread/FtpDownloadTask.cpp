#include "FtpDownloadTask.h"
#include "LibcurlFtp.h"

FtpDownloadTask::FtpDownloadTask(int taskId): m_id(taskId)
{
    m_ftp = new LibcurlFtp();
}


FtpDownloadTask::~FtpDownloadTask()
{
    if (m_ftp) {
        delete m_ftp; m_ftp = nullptr;
    }
}

void FtpDownloadTask::setFileInfo(QUrl server, QString fileName, QString localPath)
{
    m_ftpUrl = server;
    m_fileName = fileName;
    m_localPath = localPath;
}

int FtpDownloadTask::doWork()
{
    m_ftp->setHostPort(m_ftpUrl.host(), m_ftpUrl.port());
    m_ftp->setUserInfo(m_ftpUrl.userName(), m_ftpUrl.password());

    m_ftp->ftpDownload(m_ftpUrl.toString(), m_fileName, m_localPath);

    return 0;
}

QString FtpDownloadTask::message()
{
    return QString("downloading...");
}
