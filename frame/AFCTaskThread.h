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

    void onAfcReset();      // AFC服务复位

protected:
    void run() override;

private:
    void networkLibInit();   // 网络库初始化
    void dealCheck();     // 检测要处理的报文 2000 | 3000 | 4000 | 7003 | 9001 | 9004
    void respCheck();     // 检测要回复的报文：4001 | 9005
    void afcResp(int type, uchar* body, int count);    // count是包体可变部分的个数
    void make4001Resp(uchar *msg);          // 参数查询回复
    void make9005Resp(uchar *msg);          // 软件版本查询

    void parse2000(uchar* msg);   // 模式命令
    void parse3000(uchar* msg);   // 设备控制命令
    void parse4000(uchar* msg, int count);   // 参数同步
    void parse9001(uchar* msg);   // 强制时间同步
    void parse9004(uchar* msg);   // 软件、部件版本更新

    void pacakge9002();     // 9002:设备与SAM卡对应关系上传

    QList<QString> getServerFileList(QList<int> typeList);

private:
    int m_dealSeq;      // 待处理消息序号
    int m_respSeq;      // 待响应消息序号

    bool m_9002Ok;      // SAM卡号上传标志

signals:
    void paramTypeUpdate(QList<int> typeList, int type);     // 要更新的参数
    void softwareUpdate(QString fileName);                   // 软件更新

};

#endif // AFCTASKTHREAD_H
