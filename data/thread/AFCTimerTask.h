#ifndef AFCTIMERTASK_H
#define AFCTIMERTASK_H


#include "Task.h"

typedef enum
{
    DEV_STATE            = 1,   //# 设备状态上传
    TRADE_FILE           = 2    //# 交易文件上传
}AFC_WORK_TYPE;

class AFCTimerTask : public CTask
{
public:
    AFCTimerTask(int type);
    ~AFCTimerTask();


    int doWork() override;
    QString message() override;
    int taskId() override;
    bool result() override;

private:
    int m_type;    //

};

#endif // AFCTIMERTASK_H
