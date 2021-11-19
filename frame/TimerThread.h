/*
 * 可挂起线程
*/

#ifndef TIMERTHREAD_H
#define TIMERTHREAD_H

#include <QObject>
#include <QThread>
#include <atomic>
#include <QMutex>
#include <QWaitCondition>

class TimerThread : public QThread
{
    Q_OBJECT
public:
    explicit TimerThread(QObject *parent = nullptr);
    ~TimerThread() override;

    enum State
    {
        Stoped,     ///<停止状态，包括从未启动过和启动后被停止
        Running,    ///<运行状态
        Paused      ///<暂停状态
    };

    State state() const;

public slots:
    void start(Priority pri = InheritPriority);
    void stop();
    void pause();
    void resume();

protected:
    virtual void run() override final;
    virtual void process() = 0;

private:
    std::atomic_bool pauseFlag;
    std::atomic_bool stopFlag;
    QMutex mutex;
    QWaitCondition condition;

signals:

};

#endif // TIMERTHREAD_H
