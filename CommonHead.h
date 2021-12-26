#ifndef COMMONHEAD_H
#define COMMONHEAD_H

#define IS_TEST_MODE		1

#include <QtCore>
#include <QtGui>
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QPainter>
#include <QEvent>
#include <QTime>
#include <QMessageBox>
#include <QTreeView>
#include <QFrame>
#include <QDialog>
#include <QString>
#include <QMap>
#include <QList>
#include <QWidget>
#include <QDateTime>
#include <QKeyEvent>
#include <QButtonGroup>
#include <QDir>
#include <QSettings>
#include <QtGui/qcursor.h>
#include <QScrollBar>
#include <QFont>
#include <QColor>
#include <QSize>
#include <QLabel>
#include <QThread>
#include <QKeyEvent>
#include <QPushButton>
#include <QToolButton>
#include <QTableWidgetItem>


#define PARAM_FILE_PATH "sc\\parameter"
#define SOFT_FILE_PATH "sc\\soft"
#define TRADE_FILE_PATH "sc\\transcation"
#define CASHBOX_FILE_PATH "sc\\cashbox"

#define ADMIN_USER "04326688"
#define ADMIN_PWD "1234qwer."


// 低频周期任务ID，固定使用
typedef enum
{
    HEART_TASK_ID            = 2,                    //# 心跳
    AFC_TIMER_TASK_ID        = 3,
    FILE_DELET_TASK          = 4,                    //# 文件删除
    ISM_HTTP_TASK            = 5,                    //# ISM后台数据拉取
//    FARE_EN              = 3,                    //# 进站更新（付费区）
//    OVER_TRIP            = 4,                    //# 超程更新
//    OVER_TIME            = 5,                    //# 超时更新
    RESERVED       = 32                          //# 预留
}TASK_ID;


typedef enum
{
    MAIN_DLG,                   //# 主界面
    CARD_DLG,                   //# 票卡处理界面
    QRCODE_DLG,                 //# 二维码
    INQUIRY_DLG,                //# 智能问询
    INFO_DLG,                   //# 信息查询
    GUID_DLG,                   //# 服务指南
    PURCHASE_DLG,               //# 票卡-购票界面
    REFUND_DLG,                 //# 票卡-退款
    QUERY_DLG,                  //# 票卡-票卡查询
    REREGISTER_DLG,             //# 票卡-票卡补登
    PAYMENT_DLG,                //# 票卡-支付
    QR_QUERY_DLG,               //# 二维码-查询
    QR_REREGISTER_DLG,          //# 二维码-补登
    MAP_DLG,                    //# 地图导览
    LINE_DLG,                   //# 线路查询
    PRICE_DLG,                  //# 票价信息
    TRANSFER_DLG,               //# 换乘查询
    TIME_DLG,                   //# 时刻表
    PERIPHERY_DLG,              //# 地铁周边
    TEST_DLG                    //# 测试页面
}EN_WGT_ID;


#define MAX_DATA_TYPE_NUM	14
typedef enum
{
    BACK_UP                         = 0,   // 预留
    AFC_ONLINE_STATE_ID             = 1,   // AFC服务状态
    ISM_ONLINE_STATE_ID             = 2,   // ISM服务状态
    ASR_ONLINE_STATE_ID             = 3,   // ASR服务状态
    LINE_TIME                       = 4,   // 运行时间表
    LINE_INTERCHANGE                = 5,   // 站点换乘信息
    LINE_STATION_LIST               = 6,   // 线路站点名称
    READER_STATE                    = 7,   // 读写器可用状态
    COINS_STATE                     = 8,   // 硬币模块可用状态
    PAPER_MONEY_STATE               = 9,   // 纸币模块可用状态
    TICKET_BASIC                    = 10,  // 票卡基本信息
    TICKET_TRANS                    = 11,  // 票卡交易信息
    TICKET_REREGISTER               = 12,   // 票卡更新信息
    STATION_MODE                    = 13    // 系统运行模式
}EN_PARAM_ID;

typedef enum
{
    NONE_TYPE            = 8,                    //# 无需更新
    FREE_EX              = 1,                    //# 免费出站更新
    FARE_EX              = 2,                    //# 付费出站更新
    FARE_EN              = 3,                    //# 进站更新（付费区）
    OVER_TRIP            = 4,                    //# 超程更新
    OVER_TIME            = 5,                    //# 超时更新
    OVER_TIME_TRIP       = 6                     //# 超时超程更新
}REREGISTER_TYPE;

typedef enum
{
    UL_CARD            = 0x01,                    //# token票
    METRO_CARD         = 0x02,                    //# 地铁储值卡
    OCT_CARD           = 0x03,                    //# 洪城一卡通
    TU_CARD            = 0x07,                    //# 交通部卡
    UNKONW             = 0x00                     //# 未识别
}TICKET_TYPE;

typedef enum
{
    VALID              = 0,                    //# token票
    INVALID            = 1                     //# 地铁储值卡
}TICKET_STATE;

//typedef enum  {
//    CARD_NONE = 0X00,       //
//    CARD_UL = 0x01,         // UL单程票
//    CARD_MTR_CPU = 0x02,    // 地铁CPU
//    CARD_OCT_CPU = 0x03,    // 洪城通卡
//    CARD_PBOC = 0x05,       // 银联卡
//    CARD_TU_CPU = 0x07     // 交通部卡
//}CardMidia;

typedef enum  {
    FREE = 0x00,       // 非付费区
    PAY = 0x01          // 付费区
}ZONE;

typedef enum  {
    DEV_OK = 0,       // 正常
    DEV_HARDWARE_ERR = 8,           // 硬件故障
    DEV_CLOSE = 9,    // 设备故障
    DEV_LOGIN = 13,    // 登录
    DEV_SERVICE_END = 19,   // 营运结束
    DEV_SERVICE_OFF = 34,   // 停止服务
    DEV_SERVICE_ON = 20,    // 开始服务
    DEV_RW_ERR = 44    // 读写器故障
}DEVICE_EVENT;


typedef struct
{
    BYTE currentTime[7];
    BYTE modeCode[2];
    BYTE operatorID[4];
} AFC_2000_PKG_BODY, *pAFC_2000_PKG_BODY;

/// 6.2.2.2   2001-降级模式通知
typedef struct
{
    BYTE currentTime[7];
    BYTE deviceID[4];
    BYTE modeCode[2];
    BYTE modeApplyTime[7];
} AFC_2001_PKG_BODY, *pAFC_2001_PKG_BODY;

typedef struct
{
    BYTE deviceID[4];
    BYTE modeCode[2];
    BYTE modeApplyTime[7];
} AFC_2002_PKG_BODY, *pAFC_2002_PKG_BODY;

/// 6.3.2.1   3000-设备控制命令
typedef struct
{
    BYTE sendID[4];
    BYTE receiveID[4];
    BYTE cmdCode;
    BYTE operatorID[4];
} AFC_3000_PKG_BODY, *pAFC_3000_PKG_BODY;

/// 3001 设备状态 包体固定部分  6.3.2.2.2
typedef struct
{
    BYTE deviceID[4];
    BYTE currentTime[7];
    BYTE deviceType;
    /// 机器状态 0、1
    BYTE status1;
    BYTE status2;
} AFC_3001_PKG_BODY, *pAFC_3001_PKG_BODY;

#define MAX_PARAM_NUM 40
/// 6.4.2.1  4000-参数同步  固定部分
typedef struct
{
    /// 节点标识码
    BYTE deviceID[4];
    BYTE syncType;
    BYTE paramCode[MAX_PARAM_NUM][2];
} AFC_4000_PKG_BODY, *pAFC_4000_PKG_BODY;

typedef struct
{
    BYTE deviceID[4];
    BYTE syncType;
} AFC_4001_PKG_BODY, *pAFC_4001_PKG_BODY;

// typedef struct
// {
//     BYTE paramCode[2];
//     BYTE paramType;
//     BYTE paramVer[4];
// }AFC_4001_PKG_BODY_R, *pAFC_4001_PKG_BODY_R;

/// 6.4.2.3  4002-参数应用上报
typedef struct
{
    BYTE deviceID[4];
    BYTE ParamType[2];
    BYTE versionOld[4];
    BYTE versionNew[4];
    BYTE applyTime[7];
} AFC_4002_PKG_BODY, *pAFC_4002_PKG_BODY;

/// 6.8.2.1  7000-文件传输通知
typedef struct
{
    BYTE currentTime[7];
    BYTE deviceID[4];
    BYTE fileType;
    BYTE fileName[50];
    BYTE fileMd5[16];
} AFC_7000_PKG_BODY, *pAFC_7000_PKG_BODY;

/// 6.8.2.2  7001-文件传输确认
typedef struct
{
    BYTE fileType;
    BYTE fileName[50];
    BYTE result;
} AFC_7001_PKG_BODY, *pAFC_7001_PKG_BODY;

typedef struct
{
    BYTE cur_time[7];      // 当前时间
    BYTE operation_day[4]; // 运营日
    BYTE file_type;        // 文件类型
    BYTE node_num[4];      // 节点编号
    BYTE up_method;        // 上传方式 0：上传所有当日未上传的文件 1：上传指定文件
    BYTE file_seq[4];      // 文件序列号
} AFC_7002_PKG_BODY, *pAFC_7002_PKG_BODY;

typedef struct
{
    BYTE cur_time[7];      // 当前时间
    BYTE operation_day[4]; // 运营日
    BYTE file_type;        // 文件类型
    BYTE node_num[4];      // 节点编号
    BYTE start_session[4]; // 开始交易流水号
    BYTE end_session[4];   // 结束交易流水号
} AFC_7003_PKG_BODY, *pAFC_7003_PKG_BODY;

/// 6.10.2  9001-强制时间同步
typedef struct
{
    BYTE deviceID[4];
    BYTE currentTime[7];
} AFC_9001_PKG_BODY, *pAFC_9001_PKG_BODY;

/// 6.10.3   9002-设备与SAM卡对应关系 循环部分 ----  另外加 设备节点标识码
typedef struct
{
    BYTE samType;
    BYTE readerType;
    BYTE samID[6];
} AFC_9002_PKG_BODY, *pAFC_9002_PKG_BODY;

/// 6.10.4   9003-操作员登录/注销
typedef struct
{
    /// 操作员编号
    BYTE operatorID[4];
    BYTE deviceID[4];
    BYTE event;
    BYTE operatorType;
} AFC_9003_PKG_BODY, *pAFC_9003_PKG_BODY;

/// 6.10.5   9004-软件/部件版本更新
typedef struct
{
    BYTE TargetID[4];
    BYTE AppType;
    char AppName[50];
} AFC_9004_PKG_BODY, *pAFC_9004_PKG_BODY;

/// 6.10.6   9005-软件/部件版本查询
// typedef struct
// {
//     BYTE PartID[4];
//     BYTE AppType;
//     BYTE AppVer[4];
//     BYTE reserve1[4];
//     BYTE reserve2[4];
// }AFC_9005_PKG_BODY_R, *pAFC_9005_PKG_BODY_R;

/// 6.10.7   9006-软件更新上报
typedef struct
{
    BYTE deviceID[4];
    BYTE applicationType;
    BYTE version[4];
    BYTE currentTime[7];
} AFC_9006_PKG_BODY, *pAFC_9006_PKG_BODY;



#endif // COMMONHEAD_H
