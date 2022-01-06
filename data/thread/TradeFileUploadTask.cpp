﻿#include "TradeFileUploadTask.h"
#include <QDebug>
#include <QDateTime>
#include "LibcurlFtp.h"
#include "DataCenter.h"
#include <QFile>
#include <QDir>
#include "MyHelper.h"
#include "X7000FileInfo.h"
#include "NCNetwork_Lib.h"
#include "NC_ReaderLib.h"
#include "SettingCenter.h"

TradeFileUploadTask::TradeFileUploadTask(int taskId) : m_id(taskId)
{
    m_result = false;
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

void TradeFileUploadTask::packageTradeFile()
{
    QString filePath = QDir::currentPath() + QDir::separator() +
            "sc" + QDir::separator() +
            "transcation" + QDir::separator();

    m_localPath = filePath;

    QList<QString> list;
    for (QString file : m_tradeFileNameList) {
        QFile srcFile(filePath + file);
        if (!srcFile.open(QIODevice::ReadOnly)) {
            qDebug() << "[临时交易文件无法打开][packageTradeFile] cannot open srcFile %: " << file;
            continue;
        }

        QByteArray srcArray = srcFile.readAll();
        srcFile.close();

        QString fileTypeStr = file.mid(0, 1);
        QString deviceId = DataCenter::getThis()->getDeviceId();
        QDateTime curTime = QDateTime::currentDateTime();
        QString curTimeStr = curTime.toString("yyyyMMddHHmmss");
        QString serialStr = file.mid(20, 6);
        int serial = serialStr.toLong();
//        int serial = DataCenter::getThis()->getTradeFileSerial();

        //命名规则：交易文件类别（1个字符）+“.”+节点编码（8位）+“.”+YYYYMMDDHHMMSS +“.”+文件序列号（6位）
        QString targetFileName = QString("%1.%2.%3.%4")
                .arg(fileTypeStr).arg(deviceId).arg(curTimeStr).arg(serialStr);

        QFile desFile(filePath + targetFileName);
        if (!desFile.open(QIODevice::WriteOnly)) {
            qDebug() << "[目标交易文件无法生成][packageTradeFile] cannot open desFile %: " << targetFileName;
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

        //文件长度计算
        int tradeCount = calcTradeCount(fileTypeStr, srcArray.size());
        array.append(MyHelper::intToBytes(tradeCount, 4));
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

        if (tradeCount == 0) {
            qDebug() << "[交易文件格式错误]类型=" << fileTypeStr << ", 文件名=" << file;
            continue;
        }

        // 交易文件列表获取
        X7000FileInfo* info = new X7000FileInfo();
        info->setFileName(targetFileName);
        info->setMd5Arr(md5Arr.toHex());
        info->setType(icType);

        // TODO:功能待完善
//        SettingCenter::getThis()->addTradeFileInfo(info);
        m_fileList.append(info);
    }
}

long TradeFileUploadTask::calcTradeCount(QString typeStr, long srcBytes)
{
    int count = 0;
    int length = sizeof(MTRCARD_TRADE_INFO);
    if (typeStr == "S" || typeStr == "V") {
        length = sizeof(MTRCARD_TRADE_INFO);
    } else if (typeStr == "Y") {
        length = sizeof(OCTCARD_TRADE_INFO);
    } else if (typeStr == "J") {
        length = sizeof(TUCARD_TRADE_INFO);
    } else {
        length = 0;
    }

    if (length != 0) {
        count = srcBytes / length;
    }

    return count;
}


int TradeFileUploadTask::doWork()
{
    // 交易文件打包
    packageTradeFile();

    // 交易文件上传
    for (X7000FileInfo* info : m_fileList) {
        bool fileOk = false;
        QString fileName = info->fileName();
        m_fileName = fileName;
        int retFtp = m_ftp->ftpUpload(m_remotePath, fileName, m_localPath);
        qDebug() << "交易文件：" << m_fileName << " 上传结果：" << retFtp;

        // 上传成功，发送7000报文
        if (retFtp == 0) {
            int i = 0;
            while (i++ < 4) {
                QByteArray fileNameArr = fileName.toUtf8();
                int ret = FileDownloadNotify(info->type(), (BYTE*)fileNameArr.data(), (BYTE*) info->md5Arr().data());

                if (ret == 0) {
                    fileOk = true;
                    QString md5Str = info->md5Arr().toHex();
                    qDebug() << "文件上传通知：" << ret << ", 文件名:" << fileName << ", md5:" << md5Str;
                    break;
                }
            }
        }

        // 上传失败，记录文件信息
        if (!fileOk) {

        }
    }

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

bool TradeFileUploadTask::result()
{
    return m_result;
}

void TradeFileUploadTask::setTradeFileInfo(QSet<QString> fileNameList, QString remotePath)
{
    m_tradeFileNameList.clear();
    for (QString item:fileNameList) {
        m_tradeFileNameList.insert(item);
    }

    m_remotePath = remotePath;
}
