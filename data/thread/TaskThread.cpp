#include "TaskThread.h"
#include <QMutexLocker>
#include "Task.h"
#include <QDebug>

TaskThread::TaskThread(QObject *parent) : QThread(parent), EndTask(new CEndTask)
{
    start();
}

TaskThread::~TaskThread()
{
    {
        QMutexLocker locker(&mutex);
        // 清空任务队列
        while (!taskQueue.isEmpty()) {
            delete taskQueue.dequeue();
        }
    }

    if (this->isRunning()) {
        stopRun();   // stop thread
    }

    wait();

    delete EndTask;
}

void TaskThread::addTask(CTask *task)
{
    qDebug() << "addTask";
    // 必须加锁访问任务队列，防止并发访问发生冲突
    QMutexLocker locker(&mutex);
    qDebug() << "addTask ok:" << task->taskId();
    taskQueue.enqueue(task); // 添加任务
    qDebug() << "add taskQueue size :" << taskQueue.size();
    taskAdded.wakeOne();
}

void TaskThread::stopRun()
{
    QMutexLocker locker(&mutex);
    taskQueue.enqueue(this->EndTask); // 添加结束任务
    taskAdded.wakeOne();
}

void TaskThread::run()
{
    CTask *task = nullptr;
    qDebug() << "Task Thread ID:" << QThread::currentThreadId();

    forever {
        {
            qDebug() << "TaskThread.";
            QMutexLocker locker(&mutex);

            qDebug() << "taskQueue size=" << taskQueue.size();
            if (taskQueue.isEmpty()) {
                // 任务队列为空，等待有任务时唤醒
                taskAdded.wait(&mutex);
            }

            // 获取一个任务
            task = taskQueue.dequeue();

            // 判断是否是退出任务
            if (task == EndTask) {
                task->doWork(); // 执行结束任务
                break; // 跳出循环，结束线程
            }
        }

        if (task) {
            // 发送任务开始执行信号
            emit taskStart(task->message());

            // 执行任务
            task->doWork();
//            qDebug() << "Task Done Now!";
            emit taskDone(task->taskId(), task->result());
            // 执行完成将其删除
            delete task; task = nullptr;

        }

        // 检查任务是否全部完成
        {
            QMutexLocker locker(&mutex);
            if (taskQueue.isEmpty())
//                qDebug() << "All Task Is Done Now!";
                emit allTaskDone();
        }
    }
    qDebug() << "Thread Run End!";
}
