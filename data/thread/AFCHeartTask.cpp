#include "AFCHeartTask.h"
#include "NCNetwork_Lib.h"
#include "DataCenter.h"

AFCHeartTask::AFCHeartTask(int taskId): m_id(taskId)
{

}

AFCHeartTask::~AFCHeartTask()
{
}


int AFCHeartTask::doWork()
{
    // AFC 心跳
    int afcHeartRet = TcpLinkTest();
    bool flag = (afcHeartRet == 0) ? true : false;
    DataCenter::getThis()->afcHeart(flag);
    return 0;
}

QString AFCHeartTask::message()
{
    return QString("afc heart task...");
}

int AFCHeartTask::taskId()
{
    return m_id;
}
