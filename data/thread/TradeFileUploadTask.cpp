#include "TradeFileUploadTask.h"
#include <QDebug>
#include <QDateTime>
#include "LibcurlFtp.h"
#include "DataCenter.h"
#include <QFile>
#include <QDir>
#include "MyHelper.h"
#include "X7000FileInfo.h"
#include "NCNetwork_Lib.h"

TradeFileUploadTask::TradeFileUploadTask(int taskId) : m_id(taskId)
{
    m_fileList.clear();
    m_ftp = new LibcurlFtp();
}

TradeFileUploadTask::~TradeFileUploadTask()
{
    int size = m_fileList.size();
    for (int i = 0; i < size; i++) {
        delete m_fileList[i];
        m_fileList[i] = nullptr;
    }
    m_fileList.clear();

    if (m_ftp) {
        delete m_ftp; m_ftp = nullptr;
    }
}

void TradeFileUploadTask::packageTradeFile(int fileCount, QSet<QString> fileNameList, QString remotePath)
{
    m_remotePath = remotePath;
    QString filePath = QDir::currentPath() + QDir::separator() +
            "sc" + QDir::separator() +
            "transcation" + QDir::separator();

    m_localPath = filePath;

    QList<QString> list;
    for (QString file:fileNameList) {
        QFile srcFile(filePath + file);
        if (!srcFile.open(QIODevice::ReadOnly)) {
//            logger()->info("[packageTradeFile] cannot open file %: " , file);
            continue;
        }

        QByteArray srcArray = srcFile.readAll();
        srcFile.close();

        QString fileTypeStr = file.mid(0, 1);
        QString deviceId = DataCenter::getThis()->getDeviceId();
        QDateTime curTime = QDateTime::currentDateTime();
        QString curTimeStr = curTime.toString("yyyyMMddHHmmss");
        int serial = DataCenter::getThis()->getTradeFileSerial();

        //命名规则：交易文件类别（1个字符）+“.”+节点编码（8位）+“.”+YYYYMMDDHHMMSS +“.”+文件序列号（6位）
        QString targetFileName = QString("%1.%2.%3.%4")
                .arg(fileTypeStr).arg(deviceId).arg(curTimeStr).arg(serial, 6, 10, QLatin1Char('0'));

        QFile desFile(filePath + targetFileName);
        if (!desFile.open(QIODevice::WriteOnly)) {
//            logger()->info("[packageTradeFile] cannot open file %: " , file);
            continue;
        }

        QByteArray array;
        array.clear();

        // 文件头
        BYTE icType = DataCenter::getThis()->getTradeFileType(fileTypeStr);
        array.append(icType); // 文件类型
        array.append(MyHelper::hexStrToByte(curTimeStr));
        array.append(0x01);
        array.append(MyHelper::hexStrToByte(deviceId));
        array.append(MyHelper::intToBytes(serial, 4));
        array.append(MyHelper::intToBytes(fileCount, 4));
        QString headStr = array.toHex().toUpper();

        // 文件内容
        array.append(srcArray);

        // MD5校验值
        QByteArray md5Arr =  QCryptographicHash::hash(array, QCryptographicHash::Md5);
        QString md5Str = md5Arr.toHex();
        array.append(MyHelper::hexStrToByte(md5Str));

        desFile.write(array);
        desFile.close();

//        logger()->info("[packageTradeFile]文件头内容：%1, MD5=%2，size=%3",
//                       headStr, md5Str, array.size());

        // 交易文件列表获取
        X7000FileInfo* info = new X7000FileInfo();
        info->setFileName(targetFileName);
        info->setMd5Arr(md5Arr.toHex());
        info->setType(icType);
        m_fileList.append(info);
    }
}


int TradeFileUploadTask::doWork()
{
    for (X7000FileInfo* info : m_fileList) {
        QString fileName = info->fileName();
        m_fileName = fileName;
        m_ftp->ftpUpload(m_remotePath, fileName, m_localPath);

        // 发送7000报文
        QByteArray fileNameArr = fileName.toUtf8();
        int ret = FileDownloadNotify(info->type(), (BYTE*)fileNameArr.data(), (BYTE*) info->md5Arr().data());

        QString md5Str = info->md5Arr().toHex();
//        logger()->info("[FileDownloadNotify]={%1}, filename=%2, md5=%3", ret, fileName, md5Str);
    }

    qDebug() << "[CMyTask:" << m_id << "]run in thread:" << QThread::currentThreadId() << " >>> "
             << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz");

    return 0;
}

QString TradeFileUploadTask::message()
{
    return QString("[%1] uploading...").arg(m_localPath + m_fileName);
}

int TradeFileUploadTask::taskId()
{
    return m_id;
}
