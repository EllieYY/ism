#ifndef AMOUNTCHECKTIMER_H
#define AMOUNTCHECKTIMER_H

#include <QObject>
#include "AsyncTimer.h"


/* 钱箱接口调用，检查是否投入指定金额
 *
 */
class AmountCheckTimer : public AsyncTimer
{
    Q_OBJECT
public:
    explicit AmountCheckTimer(int msec, QObject *parent = nullptr);
    virtual ~AmountCheckTimer();

protected:
    virtual void run();

signals:
    void statusChanged(int result);

};

#endif // AMOUNTCHECKTIMER_H
