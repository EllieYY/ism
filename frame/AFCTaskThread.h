#ifndef AFCTASKTHREAD_H
#define AFCTASKTHREAD_H

#include <QObject>
#include <QThread>
#include "logger.h"
#include "propertyconfigurator.h"

class AFCTaskThread : public QThread
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit AFCTaskThread(QObject *parent = nullptr);

protected:
    void run() override;

private:
    void afcResp(uchar* type, uchar* body);
    void make4001Resp();          // 参数查询回复
    void make9005Resp();          // 软件版本查询

private:
    int m_seq;          // 消息序号
    int m_respSeq;      // 消息序号

signals:

};

#endif // AFCTASKTHREAD_H
