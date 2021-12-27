#include "AFCTimerTask.h"
#include "DataCenter.h"
#include "CommonHead.h"

AFCTimerTask::AFCTimerTask(int type)
{
    m_type = type;
}


AFCTimerTask::~AFCTimerTask()
{
}


int AFCTimerTask::doWork()
{
    if (m_type == DEV_STATE) {
        DataCenter::getThis()->deviceState2afc(DEV_OK);
    } else if (m_type == TRADE_FILE) {
//        DataCenter::getThis()->packageTradeFile();
    }

    return 0;
}

QString AFCTimerTask::message()
{
    return QString("AFCTimerTask...");
}

int AFCTimerTask::taskId()
{
    return AFC_TIMER_TASK_ID;
}

bool AFCTimerTask::result()
{
    return true;
}

