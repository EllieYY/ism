#ifndef CTASK_H
#define CTASK_H

#include <QObject>

// 任务接口类
class CTask
{
public:
    virtual ~CTask() {}
    virtual int doWork() = 0;
    virtual QString message() = 0;
    virtual int taskId() = 0;
    virtual bool result() = 0;     // 任务执行结果
};

// 结束任务，不做实质性操作，一般仅用作结束线程的标志
class CEndTask : public CTask
{
public:
    int doWork() override;
    QString message() override;
    int taskId() override;
    bool result() override;
};

#endif // CTASK_H
