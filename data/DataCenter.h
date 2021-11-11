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
class TicketBasicInfo;
class TransactionInfo;
class ReregisterInfo;
class QRCodeInfo;
class BasicInfo;
class LoginInfo;


// 各节点心跳ID
typedef enum
{
    AFC_HRT = 0,
    READER_HRT = 1,
    BIM_HRT = 2,
    F53_HRT = 3,
    BRC_HRT = 4
}ST_HRT_ID;

class DataCenter : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit DataCenter(QObject *parent = nullptr);
    ~DataCenter();

    static DataCenter* getThis();		// 获取数据类
    static DataCenter* m_pInstance;		// 数据类的指针

public:
    void secEvent();

    // 服务连接状态
    int getServiceState() const;
    void setServiceState(int serviceState);

    // 网络连接状态
    int getNetState() const;
    void setNetState(int netState);

    // 钱箱状态获取 0-故障 1-硬币 2-纸币 3-硬币纸币均可
    BYTE getCashboxState();

    // 站点名称
    QString getStationName() const;
    QString getStationCode() const;
    bool    isPayZone() const;

    // 设备id
    QString getDeviceId() const;

    // 线路列表
    QList<LineInfo *> getLineList() const;
    void setLineList(const QList<LineInfo *> &lineList);
    void setLineMap(QString lineCode,  QString mapPath);

    // 运行时间 | 换乘
    int getLinesTimerCloums(QList<LineTimeTables *> lineList);
    int getLinesInterchangeCloums(QList<LineInterchangeInfo *> lineList);

    // 票卡信息
    TicketBasicInfo *getTicketBasicInfo() const;
    void setTicketBasicInfo(TicketBasicInfo *ticketInfo);
    QList<QTableWidgetItem*> getTicketItems(TicketBasicInfo *ticketInfo);

    // 交易信息
    QList<TransactionInfo *> getTransInfoList() const;
    void setTransInfoList(const QList<TransactionInfo *> &transInfoList);
    QList<QTableWidgetItem*> getTranscationItems(TransactionInfo* info);

    // 票卡补登信息
    ReregisterInfo *getReregisterInfo() const;
    void setReregisterInfo(ReregisterInfo *reregisterInfo);
    QList<QTableWidgetItem*> getRegisterItems(ReregisterInfo *ticketInfo);


    // 线路站点信息
    QList<LineStations *> getLineStations() const;
    void setLineStations(const QList<LineStations *> &lineStations);

    // 运行时间表
    QList<LineTimeTables *> getLineTimeTables() const;
    void setLineTimeTables(const QList<LineTimeTables *> &lineTimeTables);

    // 线路换乘
    QList<LineInterchangeInfo *> getLineInterchanges() const;
    void setLineInterchanges(const QList<LineInterchangeInfo *> &lineInterchanges);

    // 定义与文字转换
    QString getTicketTypeString(int type);
    QString getReregisterTypeString(int type);
    QString getTicketStateString(int type);
    QString getTradeTypeString(int type);

    // 签到信息记录
    LoginInfo *getLoginInfo() const;
    void setLoginInfo(LoginInfo *loginInfo);
    void setLoginData(QString user, QString pwd);
    bool setLogoutData(QString user, QString pwd);
    bool localAuthentication(QString user, QString pwd);

    // 读写器错误提示
    QString getReaderErrorStr(BYTE errorCode);

private:

    void init();
    void initData();
    void initDevice();
    void closeDevice();
    void initReaderErrCode();

    void setHrtOffData(int idx);        // 掉线处理

    void samInfo2afc();                 // 上传SAM卡号
    void afcHeart();
    void deviceState2afc();             // 设备状态上报

private:
    int m_serviceState;                         // 服务状态 0-正常 1-异常 2-暂停
    int m_netState;                             // 网络状态 0-在线 1-离线

    BasicInfo* m_basicInfo;                     // 站点基础信息
    LoginInfo* m_loginInfo;                     // 登录信息

    // 线路信息
    QList<LineInfo*> m_lineList;                        // 线路基本信息
    QList<LineStations*> m_lineStations;                // 线路车站
    QList<LineTimeTables*> m_lineTimeTables;            // 线路运营时间表
    QList<LineInterchangeInfo*> m_lineInterchanges;     // 线路车站

    TicketBasicInfo* m_ticketBasicInfo;                 // 车票基本信息
    QList<TransactionInfo*> m_transInfoList;            // 交易信息
    ReregisterInfo* m_reregisterInfo;                   // 票卡补登信息
//    QRCodeInfo* m_qrCodeInfo;                         // 二维码信息

    QList<QString> m_hotIssues;                         // 热点问题

    bool m_isReaderUsable;              // 读写器可用状态
    bool m_isSpecieUsable;              // 硬币模块可用状态
    bool m_isBanknotesUsable;           // 纸币模块可用状态

    QHash<BYTE, QString>  m_readerErrCodeMap;   // 读写器错误码

    long m_hrtCnt[5];      // 心跳计数

signals:
    void lineReceived();
    void afcHeartDetect();      // AFC心跳 -- 异步

};

#endif // DATACENTER_H
