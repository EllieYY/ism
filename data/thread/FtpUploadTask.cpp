#include "FtpUploadTask.h"
#include <QThread>
#include <QDebug>
#include <QDateTime>
#include "LibcurlFtp.h"

FtpUploadTask::FtpUploadTask(int taskId): m_id(taskId)
{
    m_result = false;
    m_ftp = new LibcurlFtp();
}

FtpUploadTask::~FtpUploadTask()
{
    if (m_ftp) {
        delete m_ftp; m_ftp = nullptr;
    }
}

void FtpUploadTask::setFileInfo(QString remotePath,  QString fileName, QString localPath)
{
    m_remotePath = remotePath;
    m_fileName = fileName;
    m_localPath = localPath;
}


int FtpUploadTask::doWork()
{
//    m_ftp->setHostPort(m_ftpUrl.host(), m_ftpUrl.port());
//    m_ftp->setUserInfo(m_ftpUrl.userName(), m_ftpUrl.password());

    int ret = m_ftp->ftpUpload(m_remotePath, m_fileName, m_localPath);
    m_result = (ret == 0);

    qDebug() << "[CMyTask:" << m_id << "]run in thread:" << QThread::currentThreadId() << " >>> "
             << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz");

    return 0;
}

QString FtpUploadTask::message()
{
    return QString("[%1] uploading...").arg(m_localPath);
}

int FtpUploadTask::taskId()
{
    return m_id;
}

bool FtpUploadTask::result()
{
    return m_result;
}
