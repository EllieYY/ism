#ifndef HEARTTIMER_H
#define HEARTTIMER_H

#include <QObject>
#include "AsyncTimer.h"
#include "logger.h"
#include "propertyconfigurator.h"

class HeartTimer : public AsyncTimer
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit HeartTimer(QObject *parent = nullptr);
    virtual ~HeartTimer();

protected slots:
    virtual void run();

signals:
    void onlineFlag(bool);

};

#endif // HEARTTIMER_H
