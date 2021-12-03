#ifndef AMOUNTCHECKTIMER_H
#define AMOUNTCHECKTIMER_H

#include <QObject>
#include "AsyncTimer.h"
#include "logger.h"
#include "propertyconfigurator.h"

/* 钱箱接口调用，检查是否投入指定金额
 *
 */
class AmountCheckTimer : public AsyncTimer
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit AmountCheckTimer(QObject *parent = nullptr);
    virtual ~AmountCheckTimer();

protected:
    virtual void run();

private:
    long    m_startTime;

signals:
    void receiveOk(int banknotes, int coins);
    void timeoutReceive(int banknotes, int coins);
    void checkState(int, int, int);

};

#endif // AMOUNTCHECKTIMER_H
