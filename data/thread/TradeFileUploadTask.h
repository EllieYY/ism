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

    void packageTradeFile(int fileCount, QSet<QString> fileNameList, QString remotePath);

private:
    LibcurlFtp* m_ftp;

    QString m_remotePath;
    QString m_localPath;
    QString m_fileName;
    int m_id;
    int m_result;

    QList<X7000FileInfo*> m_fileList;

//signals:

};

#endif // TRADEFILEUPLOADTASK_H
