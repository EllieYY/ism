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
    PERIPHERY_DLG               //# 地铁周边
}EN_WGT_ID;


#define MAX_DATA_TYPE_NUM	13
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
    TICKET_REREGISTER               = 12   // 票卡更新信息
}EN_PARAM_ID;

typedef enum
{
    NONE_TYPE            = 0,                    //# 无需更新
    EN_LACK              = 1,                    //# 进站更新
    EX_LACK              = 2,                    //# 出站更新
    OVERTIME             = 3,                    //# 超时更新
    OVER_TRIP            = 4,                    //# 超程更新
    OVER_TIME_TRIP       = 5                     //# 超时超程更新
}REREGISTER_TYPE;

typedef enum
{
    UL_CARD              = 0x01,                    //# token票
    METRO_CARD         = 0x02,                    //# 地铁储值卡
    OCT_CARD           = 0x03,                    //# 洪城一卡通
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
    FREE = 0X00,       // 非付费区
    PAY = 0x01          // 付费区
}ZONE;

//操作员事件：
//0–签退
//1–[保留]
//2–用口令登录
//3–自动签退
//4~6-[保留]

/*
操作员类型：
00 - 票款员
01 - 售票员
02 - 维护人员
03 - 车站站长
04 - 设备技术人员
05 - 编码员
*/


#endif // COMMONHEAD_H
