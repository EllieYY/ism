#ifndef ASYNCTIMER_H
#define ASYNCTIMER_H

#include <QObject>
#include <QThread>
#include <QTimer>

class AsyncTimer : public QObject
{
    Q_OBJECT
public:
    explicit AsyncTimer(int msec=1000, QObject *parent = nullptr);
    virtual ~AsyncTimer();

    void stopTimer();

protected:
    void waitForQuit();

protected slots:
    virtual void run() = 0;

private:
    QThread thread;
    QTimer* timer;

signals:
    void stop();

};

#endif // ASYNCTIMER_H
