#include "Task.h"
#include <QDebug>
#include <QThread>
#include <QDateTime>

int CEndTask::doWork()
{
    qDebug() << "[CEndTask Thread:" << QThread::currentThreadId() << "] >>> "
             << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz");
    return 0;
}

QString CEndTask::message()
{
    return QString("CEndTask");
}
