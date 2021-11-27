#include "FtpUploadTask.h"
#include <QThread>
#include <QDebug>
#include <QDateTime>
#include "LibcurlFtp.h"

FtpUploadTask::FtpUploadTask(int taskId): m_id(taskId)
{
    m_ftp = new LibcurlFtp();
}

FtpUploadTask::~FtpUploadTask()
{
    if (m_ftp) {
        delete m_ftp; m_ftp = nullptr;
    }
}

void FtpUploadTask::setFileInfo(QUrl server, QString localPath)
{
    m_ftpUrl = server;
    m_localPath = localPath;
}


int FtpUploadTask::doWork()
{
    m_ftp->setHostPort(m_ftpUrl.host(), m_ftpUrl.port());
    m_ftp->setUserInfo(m_ftpUrl.userName(), m_ftpUrl.password());

    m_ftp->ftpUpload(m_localPath, m_ftpUrl.toString());

    qDebug() << "[CMyTask:" << m_id << "]run in thread:" << QThread::currentThreadId() << " >>> "
             << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz");

    return 0;
}

QString FtpUploadTask::message()
{
    return QString("[%1] uploading...").arg(m_localPath);
}
