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
class TradeFileInfo;
class ReaderSoftFileInfo;

// 各节点心跳ID
#define HEART_NUM 5
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

    // AFC网络连接状态
    int getNetState() const;
    void setNetState(int netState);

    // 设备状态信息：读写器、硬币模块、纸币模块、纸币找零模块
    // -1：未初始化，0：正常可用，其他值含义按接口定义来
    bool getIsReaderUsable() const;
    void setReaderState(int readerState);
    void readerOnline(bool isOnline);    // 心跳
    QString getReaderVersion();         // 读写器版本信息查询
    void setReaderVersion(QString version);
    void setSAMInfo(BYTE* mtrSam, BYTE* octSam, BYTE* jtbSam);
    void getSAMInfo(BYTE* mtrSam, BYTE* octSam, BYTE* jtbSam);
    bool getIsSamOk() const;

    long getCashboxInitRet() const;
    void setCashboxInitRet(long cashboxInitRet);
    BYTE getCashboxState();   // 钱箱状态获取 0-故障 1-硬币 2-纸币 3-硬币纸币均可

    // [bit0]:brc [bit1]:bim [bit2]:f53
    void setCashboxState(int coinState, int banknotes, int banknotesRe, uchar validDevice);
    void cashboxOnline(bool coin, bool banknotes, bool banknotesRe);

    // 站点信息
    QString getStationName() const;
    QString getStationCode() const;
    bool    isPayZone() const;
    QString stationCode2Name(QString code) const;
    int getStationMode();   // 车站模式
    void setStationMode(int stationMode);   // 车站运行模式设置

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
    QString getTUStateStr(int state);

    QString getTradeTypeString(int type);
    QString getUpdateTypeString(int type);
    QString getReaderErrorStr(BYTE errorCode);
    QString getTradeFileTypeStr(int icType);
    int getTradeFileType(QString icTypeStr);

    // 签到、签退信息记录
    LoginInfo *getLoginInfo() const;
    void setLoginInfo(LoginInfo *loginInfo);
    bool setLoginData(QString user, QString pwd);
    bool setLogoutData(QString user, QString pwd);
    bool autoLogout();     // 自动签退
    bool localAuthentication(QString user, QString pwd);
    QString getOperatorId();
    bool isValidUser(QString userName, QString pwd);    // 使用参数文件定义内容进行鉴权

    /* ---- FTP相关 --------------------------------*/
    void onSoftwareUpdate(QString fileName);   // 读写器程序更新
    void onParamUpdate(QList<int> typeList, int type);  // 参数更新
    QHash<int,long> getParamFileFilter(QList<int> typeList);    // 参数文件更新筛选

    void uploadTradeFile(QString filePath, QString fileName, QByteArray md5Arr, int type);  // 交易文件上传:本地文件路径、文件名称、md5
    void taskFinished(int taskId, bool success = true);     // 任务完成
    void allTaskFinished();            // 任务完成

    /* ---- 交易文件相关 ---------------------------*/
    void addTradeFileInfo(QString fileName);   // 暂存交易文件
    int packageTradeFile();        // 交易文件组包
    ulong getTradeFileSerial();         // 交易文件序列号
    ulong getDeviceTradeSerial();   // 终端交易流水号
    bool findFileForDelete(const QString filePath, int days);    // 定时删除交易文件

private:
    void initData();
    void closeDevice();
    void initReaderErrCode();           // 票卡分析错误码

    int getTaskId();
    void serviceStateCheck();      // 运营时间结束检测
    void serviceOffHandle();       // 运营结束处理

    void testTask();

public:
    // 参数文件解析
    void initParamVersion();
    void updateParamVersion();    // 参数更新
    int ismParamParse(int type, QString filePath);
    int parseHead(QDataStream &stream);     // 参数文件解析，返回版本号
    bool fileCheck(QByteArray array);       // 文件校验  -- 需改进
    int parseParam1001(QString filePath);
    int parseParam1004(QString filePath);
    int parseParam2002(QString filePath);
    int parseParam2004(QString filePath);
    int parseParam2005(QString filePath);
    void param2afc(int paramType, long oldVersion, long newVersion);     //4001 参数上报

public:
    void afcHeart(bool onlineFlag);
    void samInfo2afc();                 // 上传SAM卡号
    void deviceState2afc(BYTE event);   // 设备状态上报

    // 系统运行参数
    long getDeviceStateIntervalSec() const;
    long getTradeDataIntervalSec() const;
    long getTradeDataCountLT() const;

    // ISM设备状态变化
    void setIsTest(bool isTest);
    void setServiceOff(bool serviceOff);
    void setIsLogin(bool isLogin);
    BYTE getDeviceState();

    BasicInfo *getBasicInfo() const;

    long getServiceStartTime() const;
    long getServiceEndTime() const;

    bool getIsLogin() const;
    bool getServiceOff() const;

private:
    long m_hrtCnt[HEART_NUM];                   // 心跳计数
    int m_afcNetState;                          // AFC网络状态 0-在线 1-离线

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
    long m_timeCount;               // 定时上传时间计数
    QHash<int, QString> m_ticketCodeMap;    //1004： 票卡类型对应关系 <code, chName>
    int m_maxCountForLoginFail;             //2002： 连续登录失败最大次数 -- 暂时不用
    QHash<QString, OperatorInfo*> m_operatorMap;  // 2004：操作员登录信息
    long m_serviceStartTime;                //2005： 运营开始时间：距运营日0点的秒数   -- 对应去修改m_serviceOff
    long m_serviceEndTime;                  //2005： 运营开始时间：距运营日0点的秒数   -- 对应去修改m_serviceOff

    // 交易文件相关
    TradeFileInfo* m_tradeFileInfo;     // 交易文件信息记录
    long m_tradeSerial;                 // 交易序列号
    int m_numCount;                     // 交易文件个数计数

    /* ---- FTP相关 ----------------------*/
    QUrl m_ftpUrl;                      // 文件服务器连接信息

    /* ---- 读写器程序更新 ----------------*/
    ReaderSoftFileInfo* m_readerSoftInfo;     // 读写器程序更新信息

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

    bool m_hasAutoLogout;    // 控制自动签退

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
    long m_cashboxInitRet;        // 钱箱初始化返回信息
    bool m_isReaderUsable;              // 读写器可用状态
    bool m_isSpecieUsable;              // 硬币模块可用状态
    bool m_isBanknotesUsable;           // 纸币模块可用状态

    bool m_isSamOk;    // SAM卡信息读取完成
    BYTE m_mtrSam[6];
    BYTE m_octSam[6];
    BYTE m_jtbSam[6];

    // 读取本地参数配置信息
    QHash<BYTE, QString>  m_readerErrCodeMap;     // 读写器错误码
    QHash<QString, QString> m_stationCodeMap;     // <code, name>
    QHash<int, BomParamVersionInfo*> m_paramVersionMap;    // 本地应用的SC参数版本信息

    // 子线程
    AFCTaskThread* m_afcTaskThread; // AFC业务监听线程
    TaskThread* m_taskThread;       // 任务线程
    int taskId;                     // 任务Id
    int m_curParamTaskId;           // 用来记录参数文件下载的任务Id
    int m_curSoftwareTaskId;        // 记录软件程序下载的任务id

signals:
    void sigSerivceOff();           // 运营结束信号
    void sigAfcReset();
    void sigReaderReset();          // 读写器复位
    void sigCashboxReset(uchar device);  // 钱箱复位 [bit0]:brc [bit1]:bim [bit2]:f53

};

#endif // DATACENTER_H
