#ifndef ISMHTTPTASK_H
#define ISMHTTPTASK_H

#include "Task.h"

class ISMHttpTask : public CTask
{
public:
    ISMHttpTask();

    int doWork() override;
    QString message() override;
    int taskId() override;
    bool result() override;

};

#endif // ISMHTTPTASK_H
