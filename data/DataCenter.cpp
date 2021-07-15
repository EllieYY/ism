#include "DataCenter.h"
#include "WidgetMng.h"
#include "LineInfo.h"
#include "LineTimeTables.h"
#include "LineInterchangeInfo.h"
#include "LineStations.h"
#include "TicketInfo.h"
#include "TransactionInfo.h"
#include "SettingCenter.h"
#include "HttpTool.h"

DataCenter* DataCenter::m_pInstance = NULL;
DataCenter::DataCenter(QObject *parent) : QObject(parent)
{
    init();
}

DataCenter::~DataCenter()
{
    // 数据备份
    SettingCenter::getThis()->saveLineBasicInfo(m_lineList);
    SettingCenter::getThis()->saveLineStations(m_lineStations);
    SettingCenter::getThis()->saveLineInterchanes(m_lineInterchanges);
    SettingCenter::getThis()->saveLineTimeTables(m_lineTimeTables);
}

DataCenter *DataCenter::getThis()
{
    if (m_pInstance == NULL)
        m_pInstance = new DataCenter();
    return m_pInstance;
}

// 心跳连接可以在这里处理
void DataCenter::secEvent()
{
    // TODO:
}


void DataCenter::init()
{
    logger()->info("基础数据更新");
    // 获取基础数据并更新数据 #7
//    HttpTool::getThis()->requestLineBaseInfo();
//    HttpTool::getThis()->requestLineStations();
//    HttpTool::getThis()->requestInterchanges();
//    HttpTool::getThis()->requestTimeTables();
//    HttpTool::getThis()->requestStationMap();
//    HttpTool::getThis()->requestStationPreMap();
//    HttpTool::getThis()->requestLineMap();

    logger()->info("读取配置文件。");
    m_stationName = SettingCenter::getThis()->getBasicInfo();
    m_lineStations.append(SettingCenter::getThis()->getLineStations());
    m_lineTimeTables.append(SettingCenter::getThis()->getLineTimeTables());
    m_lineInterchanges.append(SettingCenter::getThis()->getLineInterchanes());
    m_lineList.append(SettingCenter::getThis()->getLineBasicInfo());
    logger()->info("配置文件读取完毕。");

    emit lineReceived();

    // 测试数据生成
    // TODO:
    m_serviceState = 0;
    m_netState = 0;
}

QList<LineInterchangeInfo *> DataCenter::getLineInterchanges() const
{
    return m_lineInterchanges;
}

void DataCenter::setLineInterchanges(const QList<LineInterchangeInfo *> &lineInterchanges)
{
    m_lineInterchanges = lineInterchanges;
    WidgetMng::notify(LINE_INTERCHANGE);
    SettingCenter::getThis()->saveLineInterchanes(lineInterchanges);
}

QList<LineTimeTables *> DataCenter::getLineTimeTables() const
{
    return m_lineTimeTables;
}

void DataCenter::setLineTimeTables(const QList<LineTimeTables *> &lineTimeTables)
{
    m_lineTimeTables = lineTimeTables;
    WidgetMng::notify(LINE_TIME);
    SettingCenter::getThis()->saveLineTimeTables(lineTimeTables);
}

QList<LineStations *> DataCenter::getLineStations() const
{
    return m_lineStations;
}

void DataCenter::setLineStations(const QList<LineStations *> &lineStations)
{
    m_lineStations = lineStations;
    WidgetMng::notify(LINE_STATION_LIST);
    SettingCenter::getThis()->saveLineStations(lineStations);
}

QRCodeInfo *DataCenter::getQrCodeInfo() const
{
    return m_qrCodeInfo;
}

void DataCenter::setQrCodeInfo(QRCodeInfo *qrCodeInfo)
{
    m_qrCodeInfo = qrCodeInfo;
}

CardInfo *DataCenter::getCardInfo() const
{
    return m_cardInfo;
}

void DataCenter::setCardInfo(CardInfo *cardInfo)
{
    m_cardInfo = cardInfo;
}

QList<TransactionInfo *> DataCenter::getTransInfoList() const
{
    return m_transInfoList;
}

void DataCenter::setTransInfoList(const QList<TransactionInfo *> &transInfoList)
{
    m_transInfoList = transInfoList;
}

QList<QTableWidgetItem *> DataCenter::getTranscationItems(TransactionInfo *info)
{
    QList<QTableWidgetItem*> itemList;

    QTableWidgetItem* item1 = new QTableWidgetItem(info->createTime().toString("yyyy-MM-dd HH:mm:ss"));

    // TODO:类型文字
    QTableWidgetItem* item2 = new QTableWidgetItem(QString("%1").arg(info->type()));

    QString comStr = QString("%1").arg(info->consumptionNum(), 0, 'f', 1) + "/" +
            QString("%1").arg(info->consumptionCount());
    QTableWidgetItem* item3 = new QTableWidgetItem(comStr);

    QString validStr = QString("%1").arg(info->balance(), 0, 'f', 1) + "/" +
            QString("%1").arg(info->validCount());
    QTableWidgetItem* item4 = new QTableWidgetItem(validStr);


    itemList.append(item1);
    itemList.append(item2);
    itemList.append(item3);
    itemList.append(item4);

    return itemList;
}

TicketInfo *DataCenter::getTicketInfo() const
{
    return m_ticketInfo;
}

void DataCenter::setTicketInfo(TicketInfo *ticketInfo)
{
    m_ticketInfo = ticketInfo;
}

// 车票基本信息格式化
QList<QTableWidgetItem *> DataCenter::getTicketItems(TicketInfo *info)
{
    QList<QTableWidgetItem*> itemList;

    QTableWidgetItem* item1 = new QTableWidgetItem(info->type());
    QTableWidgetItem* item2 = new QTableWidgetItem(info->number());
    QTableWidgetItem* item3 = new QTableWidgetItem(info->createTime().toString("yyyy-MM-dd"));
    QTableWidgetItem* item4 = new QTableWidgetItem(info->validDate().toString("yyyy-MM-dd"));

    // TODO:状态文字
    QTableWidgetItem* item5 = new QTableWidgetItem(QString("%1").arg(info->cardState()));
    QTableWidgetItem* item6 = new QTableWidgetItem(QString("%1").arg(info->tripState()));

    QTableWidgetItem* item7 = new QTableWidgetItem(QString("%1").arg(info->balance(), 0, 'f', 1));

    itemList.append(item1);
    itemList.append(item2);
    itemList.append(item3);
    itemList.append(item4);
    itemList.append(item5);
    itemList.append(item6);
    itemList.append(item7);

    return itemList;
}

int DataCenter::getServiceState() const
{
    return m_serviceState;
}

void DataCenter::setServiceState(int serviceState)
{
    m_serviceState = serviceState;
}

int DataCenter::getNetState() const
{
    return m_netState;
}

void DataCenter::setNetState(int netState)
{
    m_netState = netState;
}

QString DataCenter::getStationName() const
{
    return m_stationName;
}

void DataCenter::setStationName(const QString &stationName)
{
    m_stationName = stationName;
}

QList<LineInfo *> DataCenter::getLineList() const
{
    return m_lineList;
}

void DataCenter::setLineList(const QList<LineInfo *> &lineList)
{
    m_lineList.clear();
    m_lineList.append(lineList);
}

void DataCenter::setLineMap(QString lineCode, QString mapPath)
{
    for (LineInfo* info:m_lineList) {
        if (info->getCode().compare(lineCode) == 0) {
            info->setPicPath(mapPath);
            break;
        }
    }
}


// 获取线路时间表要显示的行数
int DataCenter::getLinesTimerCloums(QList<LineTimeTables *> lineList)
{
    int count = 0;
    for (LineTimeTables* line:lineList) {
        count += line->timeTable().size();
    }

    return count;
}

// 获取线路时间表要显示的行数
int DataCenter::getLinesInterchangeCloums(QList<LineInterchangeInfo *> lineList)
{
    int count = 0;
    for (LineInterchangeInfo* line:lineList) {
        count += line->interchangeList().size();
    }

    return count;
}


