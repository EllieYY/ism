#ifndef DATACENTER_H
#define DATACENTER_H

#include <QObject>
#include "CommonHead.h"
#include "logger.h"
#include "propertyconfigurator.h"

class LineInfo;
class ISMTimeTable;
class LineTimeTables;
class LineStations;
class LineInterchangeInfo;
class InterchangeInfo;
class TicketInfo;
class TransactionInfo;
class CardInfo;
class QRCodeInfo;
class DataCenter : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit DataCenter(QObject *parent = nullptr);

    static DataCenter* getThis();		// 获取数据类
    static DataCenter* m_pInstance;		// 数据类的指针

public:
    void secEvent();

    int getServiceState() const;
    void setServiceState(int serviceState);

    int getNetState() const;
    void setNetState(int netState);

    QString getStationName() const;
    void setStationName(const QString &stationName);

    QList<LineInfo *> getLineList() const;
    void setLineList(const QList<LineInfo *> &lineList);

    int getLinesTimerCloums(QList<LineTimeTables *> lineList);
    int getLinesInterchangeCloums(QList<LineInterchangeInfo *> lineList);

    TicketInfo *getTicketInfo() const;
    void setTicketInfo(TicketInfo *ticketInfo);
    QList<QTableWidgetItem*> getTicketItems(TicketInfo *ticketInfo);

    QList<TransactionInfo *> getTransInfoList() const;
    void setTransInfoList(const QList<TransactionInfo *> &transInfoList);
    QList<QTableWidgetItem*> getTranscationItems(TransactionInfo* info);


    CardInfo *getCardInfo() const;
    void setCardInfo(CardInfo *cardInfo);

    QRCodeInfo *getQrCodeInfo() const;
    void setQrCodeInfo(QRCodeInfo *qrCodeInfo);

    QList<LineStations *> getLineStations() const;
    void setLineStations(const QList<LineStations *> &lineStations);

    QList<LineTimeTables *> getLineTimeTables() const;
    void setLineTimeTables(const QList<LineTimeTables *> &lineTimeTables);

    QList<LineInterchangeInfo *> getLineInterchanges() const;
    void setLineInterchanges(const QList<LineInterchangeInfo *> &lineInterchanges);

private:
    void init();

private:
    int m_serviceState;                         // 服务状态 0-正常 1-异常 2-暂停
    int m_netState;                             // 网络状态 0-在线 1-离线
    QString m_stationName;                      // 站点名称

    // 线路信息
    QList<LineInfo*> m_lineList;                        // 线路基本信息
    QList<LineStations*> m_lineStations;                // 线路车站
    QList<LineTimeTables*> m_lineTimeTables;             // 线路运营时间表
    QList<LineInterchangeInfo*> m_lineInterchanges;     // 线路车站

    TicketInfo* m_ticketInfo;                   // 车票基本信息
    QList<TransactionInfo*> m_transInfoList;    // 交易信息
    CardInfo* m_cardInfo;                       // 票卡信息
    QRCodeInfo* m_qrCodeInfo;                   // 二维码信息

    QList<QString> m_hotIssues;                 // 热点问题


signals:

};

#endif // DATACENTER_H
