#include "HeartTask.h"
#include "NCNetwork_Lib.h"
#include "DataCenter.h"
#include <QDate>
#include <QDateTime>


HeartTask::HeartTask()
{
    m_result = false;
}

HeartTask::~HeartTask()
{
}


int HeartTask::doWork()
{
//    serviceStateCheck();

    // AFC 心跳
    int afcHeartRet = TcpLinkTest();
    m_result = (afcHeartRet == 0);
    DataCenter::getThis()->afcHeart(m_result);
    return 0;
}

QString HeartTask::message()
{
    return QString("afc heart task...");
}

int HeartTask::taskId()
{
    return HEART_TASK_ID;
}

bool HeartTask::result()
{
    return m_result;
}

