#ifndef AFCTASKTIMER_H
#define AFCTASKTIMER_H

#include <QObject>
#include "AsyncTimer.h"

class AFCTaskTimer : public AsyncTimer
{
    Q_OBJECT
public:
    explicit AFCTaskTimer(int msec, QObject *parent = nullptr);
    virtual ~AFCTaskTimer();

protected slots:
    virtual void run();

signals:

};

#endif // AFCTASKTIMER_H
