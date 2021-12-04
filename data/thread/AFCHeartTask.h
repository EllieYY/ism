#ifndef AFCHEARTTASK_H
#define AFCHEARTTASK_H

#include "Task.h"

class AFCHeartTask : public CTask
{
public:
    AFCHeartTask(int taskId);
    ~AFCHeartTask();

    int doWork() override;
    QString message() override;
    int taskId() override;

private:
    int m_id;

};

#endif // AFCHEARTTASK_H
