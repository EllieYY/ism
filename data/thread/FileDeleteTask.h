#ifndef FILEDELETETASK_H
#define FILEDELETETASK_H

#include "Task.h"

class FileDeleteTask : public CTask
{
public:
    FileDeleteTask(QString filePath, int days);

    int doWork() override;
    QString message() override;
    int taskId() override;
    bool result() override;

private:
    QString m_filePath;
    int m_days;
};

#endif // FILEDELETETASK_H
