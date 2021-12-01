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
class BomParamVersionInfo;

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
    void init();       // 子线程中会用到DataCenter指针，init里有子线程的初始化，故将初始化单独拎出来，不放在构造函数里
    void secEvent();
    void setHrtOffData(int idx);        // 掉线处理

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

    /* ---- ISM后台信息 ----------------------------------*/
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

    // 票卡状态和错误码定义 - 类型转换
    QString getTicketTypeString(int type);
    QString getTicketStateString(int icType, int state);
    QString getULStateStr(int state);
    QString getCPUStateStr(int state);
    QString getOCTStateStr(int state);
    QString getTradeTypeString(int type);
    QString getUpdateTypeString(int type);
    QString getReaderErrorStr(BYTE errorCode);

    // 签到、签退信息记录
    LoginInfo *getLoginInfo() const;
    void setLoginInfo(LoginInfo *loginInfo);
    void setLoginData(QString user, QString pwd);
    bool setLogoutData(QString user, QString pwd);
    bool localAuthentication(QString user, QString pwd);
    QString getOperatorId();
    bool isValidUser(QString userName, QString pwd);    // 使用参数文件定义内容进行鉴权

    /* ---- FTP相关 --------------------------------*/
    void onSoftwareUpdate(QString fileName);   // 读写器程序更新
    void onParamUpdate(QList<int> typeList, int type);  // 参数更新
    QHash<int,long> getParamFileFilter(QList<int> typeList);

    void uploadTradeFile(QString filePath, QString fileName, QByteArray md5Arr, int type);  // 交易文件上传:本地文件路径、文件名称、md5
    void ftpTaskFinished(int taskId);     // 文件上传任务完成
    void ftpAllTaskFinished();            // 文件上传任务完成

private:
    void initData();
    void initDevice();
    void closeDevice();
    void initReaderErrCode();           // 票卡分析错误码

public:
    // 参数文件解析
    void initParamVersion();
    void updateParamVersion();    // 参数更新
    int parseHead(QDataStream &stream);     // 参数文件解析，返回版本号
    bool fileCheck(QByteArray array);       // 文件校验  -- 需改进
    int parseParam1001(QString filePath);
    int parseParam1004(QString filePath);
    int parseParam2002(QString filePath);
    int parseParam2004(QString filePath);
    void param2afc(int paramType, long oldVersion, long newVersion);     //4001 参数上报

public:
    void afcHeart(bool onlineFlag);
    void samInfo2afc();                 // 上传SAM卡号
    void deviceState2afc(BYTE event);   // 设备状态上报
    QString getReaderVersion();         // 读写器版本信息查询
    void setStationMode(int stationMode);   // 车站运行模式设置

    // 系统运行参数
    long getDeviceStateIntervalSec() const;
    long getTradeDataIntervalSec() const;
    long getTradeDataCountLT() const;

    // 设备状态变化
    void setIsTest(bool isTest);
    void setServiceOff(bool serviceOff);
    void setIsLogin(bool isLogin);
    BYTE getDeviceState();

    BasicInfo *getBasicInfo() const;

private:
    long m_hrtCnt[5];                           // 心跳计数
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
    QList<QString> m_hotIssues;                         // 热点问题

    /* ---- 参数文件解析信息 -----------------*/
    // 设备状态和交易信息上传参数
    long m_deviceStateIntervalSec;          //1001： 设备状态上传间隔，单位秒
    long m_tradeDataIntervalSec;            //1001： 交易信息上传时间间隔
    long m_tradeDataCountLT;                //1001： 交易数量上传下限，超过下限即可上传
    long m_timeCount;    // 定时上传时间计数
    QHash<int, QString> m_ticketCodeMap;    //1004：票卡类型对应关系 <code, chName>
    int m_maxCountForLoginFail;             //2002： 连续登录失败最大次数 -- 暂时不用
    QHash<QString, OperatorInfo*> m_operatorMap;  // 2004：操作员登录信息

    // 交易文件相关

    long m_tradeSerial;  // 交易序列号

    int m_numCount;      // 交易文件个数计数


    /* ---- FTP相关 ---------*/
    QUrl m_ftpUrl;                  // 文件服务器连接信息
    TaskThread* m_ftpTaskThread;    // 任务线程
    int taskId;                     // 任务Id
    int m_curParamTaskId;           // 用来记录参数文件下载的任务Id
    int m_curSoftwareTaskId;        // 记录软件程序下载的任务id

    //3001报文 设备状态
    //    0	开(1)/关(0)
    //    1	停止服务(1)/无故障(0)
    //    2	测试(1)/生产(0)
    //    3	EFO(1)/BOM(0)
    //    4	已登录(1)/签退(0)
    //    5	可以充值(1)/不可充值(0)
    //    6	自动发售机构可用(1)/不可用(0)
    //    7	[未定义]
    bool m_isTest;      // 测试状态
    bool m_serviceOff;  // 服务状态
    bool m_isLogin;     // 是否登录

    //2000 降级模式通知
    //0x00    Bit 0	紧急模式            1：生效，0:无效
    //0x02    Bit 1	进站免检模式         1：生效，0:无效
    //0x04    Bit 2	日期免检模式         1：生效，0:无效
    //0x08    Bit 3	时间免检模式         1：生效，0:无效

    //0x10    Bit 4	列车故障模式         1：生效，0:无效
    //0x20    Bit 5	车费免检模式         1：生效，0:无效
    int m_stationMode;               // AFC系统运行模式
    QString m_readerVersion;         // 读写器版本信息 -- 软件、部件版本

    //部件设备状态
    bool m_isReaderUsable;              // 读写器可用状态
    bool m_isSpecieUsable;              // 硬币模块可用状态
    bool m_isBanknotesUsable;           // 纸币模块可用状态

    // 读取本地参数配置信息
    QHash<BYTE, QString>  m_readerErrCodeMap;     // 读写器错误码
    QHash<QString, QString> m_stationCodeMap;     // <code, name>
    QHash<int, BomParamVersionInfo*> m_paramVersionMap;    // 本地应用的SC参数版本信息


    // 子线程
    HeartTimer*  m_timer;    // 心跳检测异步线程
    AFCTaskThread* m_taskThread;

signals:

};

#endif // DATACENTER_H
