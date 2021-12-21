#include "HeartTask.h"
#include "NCNetwork_Lib.h"
#include "DataCenter.h"
#include <QDate>
#include <QDateTime>


HeartTask::HeartTask(int taskId): m_id(taskId)
{
    m_result = false;
}

HeartTask::~HeartTask()
{
}


int HeartTask::doWork()
{
    serviceStateCheck();

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
    return m_id;
}

bool HeartTask::result()
{
    return m_result;
}


// 运营状态检测
void HeartTask::serviceStateCheck()
{
    QDateTime dayStart = QDate::currentDate().startOfDay();
    QDateTime now = QDateTime::currentDateTime();

    long nowSec = dayStart.secsTo(now);

    long startSec = DataCenter::getThis()->getServiceStartTime();
    long endSec = DataCenter::getThis()->getServiceEndTime();

//    // TODO:test code
//    startSec = 14 * 3600 + 41 * 60;
//    endSec = 14 * 3600 + 45 * 60;

    // 交集为反
    bool type = true;
    if (endSec < startSec) {
        type = false;
    }

    // 运营结束
    bool serviceOn = false;
    if (nowSec >= qMin(startSec, endSec) && nowSec < qMax(startSec, endSec)) {
        serviceOn = true;
    }


    bool serviceOff = serviceOn ^ type;

    QString info = QString("start:%1, end:%2, serviceOff:%3").arg(startSec).arg(endSec).arg(serviceOff);
    qDebug() << "serviceStateCheck = " << info;

    DataCenter::getThis()->setServiceOff(serviceOff);
}
