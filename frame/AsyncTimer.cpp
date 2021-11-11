#include "AsyncTimer.h"

/**
 * @brief AsyncTimer::AsyncTimer
 * 建立线程，定时器
 * @param msec  超时时间
 * @param parent  父节点
 */
AsyncTimer::AsyncTimer(int msec, QObject *parent) : QObject(parent)
{
    timer = new QTimer();
    timer->setInterval(msec);
    timer->moveToThread(&thread);
    connect(timer, SIGNAL(timeout()), this, SLOT(run()), Qt::DirectConnection);
    connect(&thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(&thread, SIGNAL(started()), timer, SLOT(start()));
    connect(this, &AsyncTimer::stop, timer, &QTimer::stop);
    thread.start();
}

/**
 * @brief AsyncTimer::~AsyncTimer
 * 退出线程，释放定时器
 */
AsyncTimer::~AsyncTimer()
{
    waitForQuit();
}

void AsyncTimer::stopTimer()
{
    emit stop();
}

/**
 * @brief AsyncTimer::waitForQuit
 * 等待线程退出函数，在子类析构函数中调用（推荐）。
 * 避免线程还在运行时，子类就开始析构，若此时线程中还在使用被析构的子类成员，则发生段错误。
 */
void AsyncTimer::waitForQuit()
{
    thread.quit();
    thread.wait();
}
