#ifndef ASYNCTIMER_H
#define ASYNCTIMER_H

#include <QObject>
#include <QThread>
#include <QTimer>

class TimerThread;
class AsyncTimer : public QObject
{
    Q_OBJECT
public:
    explicit AsyncTimer(QObject *parent = nullptr);
    virtual ~AsyncTimer();

    void startTimer(int msec = 1000);
    void pauseTimer();
    void resumeTimer();
    void stopTimer();

protected:
    void waitForQuit();

protected slots:
    virtual void run() = 0;

protected:
    QThread* thread;
    QTimer* timer;
    bool pauseFlag;

signals:
    void stop();

};

#endif // ASYNCTIMER_H
