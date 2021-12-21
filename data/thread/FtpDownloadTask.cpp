#include "FtpDownloadTask.h"
#include "LibcurlFtp.h"

FtpDownloadTask::FtpDownloadTask(int taskId): m_id(taskId)
{
    m_result = false;
    m_isFileList = false;
    m_isForceUpdate = false;
    m_ftp = new LibcurlFtp();
}


FtpDownloadTask::~FtpDownloadTask()
{
    if (m_ftp) {
        delete m_ftp;
        m_ftp = nullptr;
    }
}

void FtpDownloadTask::setFileInfo(QString remotePath, QString fileName, QString localPath)
{
    m_remotePath = remotePath;
    m_fileName = fileName;
    m_localPath = localPath;
}

void FtpDownloadTask::setFilter(QHash<int, long> filter, bool isForceUpdate)
{
    m_isFileList = true;
    m_isForceUpdate = isForceUpdate;
    m_filter = filter;
}

int FtpDownloadTask::doWork()
{
//    m_ftp->setHostPort(m_ftpUrl.host(), m_ftpUrl.port());
//    m_ftp->setUserInfo(m_ftpUrl.userName(), m_ftpUrl.password());

    int ret = -1;
    if (m_isFileList) {
        ret = m_ftp->ftpList(m_remotePath, m_fileName, m_localPath, m_filter, m_isForceUpdate);
    } else {
        ret = m_ftp->ftpDownload(m_remotePath, m_fileName, m_localPath);
    }

    m_result = (ret == 0);

    return 0;
}

QString FtpDownloadTask::message()
{
    return QString("downloading...");
}

int FtpDownloadTask::taskId()
{
    return m_id;
}

bool FtpDownloadTask::result()
{
    return m_result;
}
