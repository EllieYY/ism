#ifndef TASKTHREAD_H
#define TASKTHREAD_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QWaitCondition>
#include <QMutex>

class CTask;
class CEndTask;
class TaskThread : public QThread
{
    Q_OBJECT
public:
    explicit TaskThread(QObject *parent = nullptr);
    ~TaskThread();

    void addTask(CTask *task); // 添加任务的接口
    void stopRun();            // 停止线程执行接口

protected:
    void run() override;

// 定义一些与主线程通信的信号
signals:
    void taskStart(const QString &message);
    void taskDone(int taskId, bool success = true);
    void allTaskDone();

private:
    QQueue<CTask*> taskQueue; // 任务队列
    QWaitCondition taskAdded; // 任务等待条件
    QMutex mutex;             //
    CTask *EndTask;           // 结束任务


};

#endif // TASKTHREAD_H
