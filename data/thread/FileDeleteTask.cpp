#include "FileDeleteTask.h"
#include "CommonHead.h"
#include "DataCenter.h"

FileDeleteTask::FileDeleteTask(QString filePath, int days)
{
    m_filePath = filePath;
    m_days = days;
}


int FileDeleteTask::doWork()
{
    DataCenter::getThis()->findFileForDelete(m_filePath, m_days);
    return 0;
}

QString FileDeleteTask::message()
{
    return QString("FileDeleteTask...");
}

int FileDeleteTask::taskId()
{
    return FILE_DELET_TASK;
}

bool FileDeleteTask::result()
{
    return true;
}
