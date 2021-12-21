#ifndef HEARTTASK_H
#define HEARTTASK_H

#include "Task.h"

/*
 * 服务心跳检测：
 *  - AFC心跳
 *  - ISM后台心跳
 *  - 语音服务后台心跳
 * 运营状态检测
*/


class HeartTask : public CTask
{
public:
    HeartTask(int taskId);
    ~HeartTask();

    int doWork() override;
    QString message() override;
    int taskId() override;
    bool result() override;

private:
    void serviceStateCheck();

private:
    int m_id;
    bool m_result;

};

#endif // HEARTTASK_H
