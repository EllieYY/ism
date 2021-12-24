#include "ISMHttpTask.h"
#include "CommonHead.h"
#include "HttpTool.h"

ISMHttpTask::ISMHttpTask()
{

}


int ISMHttpTask::doWork()
{
    qDebug() << "ISMHttpTask::doWork";
    // 获取基础数据并更新数据 #7
    HttpTool::getThis()->requestLineBaseInfo();
    HttpTool::getThis()->requestLineStations();
    HttpTool::getThis()->requestInterchanges();
    HttpTool::getThis()->requestTimeTables();
    HttpTool::getThis()->requestStationMap();
    HttpTool::getThis()->requestStationPreMap();
    HttpTool::getThis()->requestLineMap();

    return 0;
}

QString ISMHttpTask::message()
{
    return QString("ISMHttpTask...");
}

int ISMHttpTask::taskId()
{
    return ISM_HTTP_TASK;
}

bool ISMHttpTask::result()
{
    return true;
}
