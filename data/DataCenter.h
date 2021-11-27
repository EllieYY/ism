#ifndef DATACENTER_H
#define DATACENTER_H

#include <QObject>
#include "CommonHead.h"
#include "logger.h"
#include "propertyconfigurator.h"
#include <QUrl>

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
class HeartTimer;
class AFCTaskThread;
class OperatorInfo;
class TaskThread;

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

    // 站点信息
    QString getStationName() const;
    QString getStationCode() const;
    bool    isPayZone() const;
    QString stationCode2Name(QString code) const;
    int getTradeSerial();   // 交易序列号
    int getStationMode();   // 车站模式

    // 设备id
    QString getDeviceId() const;
    int getAntiNo() const;

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
    QString getTicketStateString(int type);
    QString getTradeTypeString(int type);
    QString getUpdateTypeString(int type);

    // 签到信息记录
    LoginInfo *getLoginInfo() const;
    void setLoginInfo(LoginInfo *loginInfo);
    void setLoginData(QString user, QString pwd);
    bool setLogoutData(QString user, QString pwd);
    bool localAuthentication(QString user, QString pwd);
    QString getOperatorId();

    // 读写器错误提示
    QString getReaderErrorStr(BYTE errorCode);

    // 读写器程序下载
    void onSoftwareUpdate(QString fileName);

    // 参数更新
    void onParamUpdate(QList<int> typeList);
    QList<QString> getServerFileList(QList<int> typeList);

    // 交易文件上传:本地文件路径、文件名称、md5
    void uploadTradeFile(QString filePath, QString fileName, QByteArray md5Arr, int type);

private:
    void init();
    void initData();
    void initDevice();
    void closeDevice();
    void initReaderErrCode();
    void initParamVersion();            // 参数版本号解析

public:
    // 参数文件解析，返回版本号
    int parseHead(QDataStream &stream);
    bool fileCheck(QByteArray array);   // 文件校验
    int parseParam1001(QString filePath);
    int parseParam1004(QString filePath);
    int parseParam2002(QString filePath);
    int parseParam2004(QString filePath);

    bool isValidUser(QString userName, QString pwd);

    void ftpTaskFinished();   // 文件上传任务完成

    void setHrtOffData(int idx);        // 掉线处理

public:
    void samInfo2afc();                 // 上传SAM卡号
    void afcHeart(bool onlineFlag);
    void deviceState2afc();             // 设备状态上报
    void param2afc();                   // 参数上报

    void setStationMode(int stationMode);

    QString getReaderVersion();

    // 系统运行参数
    long getDeviceStateIntervalSec() const;
    long getTradeDataIntervalSec() const;
    long getTradeDataCountLT() const;

    QUrl getFtpUrl();

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
    long m_tradeSerial;    // 交易序列号
    QHash<QString, QString> m_stationCodeMap;     // <code, name>

    HeartTimer*  m_timer;    // 心跳检测异步线程
    AFCTaskThread* m_taskThread;

    int m_stationMode;               // AFC系统运行模式
    QString m_readerVersion;         // 读写器版本信息 -- 软件、部件版本
    QList<QString> m_paramVersion;   // 参数版本号信息

    // 1001：设备状态和交易信息上传参数
    long m_deviceStateIntervalSec;    // 设备状态上传间隔，单位秒
    long m_tradeDataIntervalSec;      // 交易信息上传时间间隔
    long m_tradeDataCountLT;          // 交易数量上传下限，超过下限即可上传

    // 1004：票卡类型对应关系
    QHash<int, QString> m_ticketCodeMap;     // <code, chName>

    // 2002：连续登录失败最大次数
    int m_maxCountForLoginFail;    // 连续登录失败最大次数

    // 2004：操作员登录信息
    QHash<QString, OperatorInfo*> m_operatorMap;

    QUrl m_ftpUrl;     // 文件服务器连接信息

    TaskThread* m_ftpTaskThread;
    int taskId;

signals:
    void lineReceived();

};

#endif // DATACENTER_H
