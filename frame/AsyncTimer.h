#ifndef ASYNCTIMER_H
#define ASYNCTIMER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QMutex>

class TimerThread;
class AsyncTimer : public QObject
{
    Q_OBJECT
public:
    explicit AsyncTimer(QObject *parent = nullptr);
    virtual ~AsyncTimer();

    void startTimer(int msec = 1000);
    void pauseAsyncTimer();
    void resumeAsyncTimer();
    void stopTimer();

protected:
    void waitForQuit();

protected slots:
    virtual void run() = 0;

protected:
    QThread* thread;
    QTimer* timer;
    volatile bool pauseFlag;
    QMutex m_lock;

signals:
    void stop();

};

#endif // ASYNCTIMER_H
