#ifndef TRADEFILEUPLOADTASK_H
#define TRADEFILEUPLOADTASK_H

#include <QObject>
#include "Task.h"
#include <QSet>
//#include "logger.h"
//#include "propertyconfigurator.h"

class LibcurlFtp;
class X7000FileInfo;
class TradeFileUploadTask : public CTask
{
//    Q_OBJECT
//    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit TradeFileUploadTask(int taskId);
    ~TradeFileUploadTask();

    int doWork() override;
    QString message() override;
    int taskId() override;
    bool result() override;

    void setTradeFileInfo(QSet<QString> fileNameList, QString remotePath);

private:
    void packageTradeFile();
    long calcTradeCount(QString typeStr, long srcBytes);

private:
    LibcurlFtp* m_ftp;

    QString m_remotePath;
    QString m_localPath;
    QString m_fileName;
    int m_id;
    int m_result;

    QSet<QString> m_tradeFileNameList;    // 临时交易文件列表 - 需要打包的，类别和序列号确定了唯一性
    QList<X7000FileInfo*> m_fileList;     // 待上传的交易文件

//signals:

};

#endif // TRADEFILEUPLOADTASK_H
