#include "TimerThread.h"
#include <QDebug>


TimerThread::TimerThread(QObject *parent)
    : QThread(parent),
      pauseFlag(false),
      stopFlag(false)
{


}

TimerThread::~TimerThread()
{
    stop();
}

TimerThread::State TimerThread::state() const
{
    State s = Stoped;
    if (!QThread::isRunning())
    {
        s = Stoped;
    }
    else if (QThread::isRunning() && pauseFlag)
    {
        s = Paused;
    }
    else if (QThread::isRunning() && (!pauseFlag))
    {
        s = Running;
    }
    return s;
}

void TimerThread::start(Priority pri)
{
    QThread::start(pri);
}

void TimerThread::stop()
{
    if (QThread::isRunning())
    {
        stopFlag = true;
        condition.wakeAll();
        QThread::quit();
        QThread::wait();
    }
}

void TimerThread::pause()
{
    if (QThread::isRunning())
    {
        pauseFlag = true;
    }
}

void TimerThread::resume()
{
    if (QThread::isRunning())
    {
        pauseFlag = false;
        condition.wakeAll();
        emit start();
    }
}

void TimerThread::run()
{
    qDebug() << "enter thread : " << QThread::currentThreadId();

    while (!stopFlag)
    {
        process();
        if (pauseFlag)
        {
            mutex.lock();
            condition.wait(&mutex);
            mutex.unlock();
        }
    }
    pauseFlag = false;
    stopFlag = false;

    qDebug() << "exit thread : " << QThread::currentThreadId();
}
