#include "HeartTimer.h"
#include "NCNetwork_Lib.h"

HeartTimer::HeartTimer(QObject *parent) : AsyncTimer(parent)
{

}

HeartTimer::~HeartTimer()
{

}

void HeartTimer::run()
{
    // AFC 心跳
    BYTE afcHeartRet = TcpLinkTest();
    logger()->info("AFC心跳={%1}", afcHeartRet);

    bool flag = (afcHeartRet == 0) ? true : false;
    emit onlineFlag(flag);
}
