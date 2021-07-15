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
    QRCODE_DLG,                //# 二维码
    INQUIRY_DLG,               //# 智能问询
    INFO_DLG,               //# 信息查询
    GUID_DLG,               //# 服务指南
    PURCHASE_DLG,                   //# 票卡-购票界面
    REFUND_DLG,                //# 票卡-退款
    QUERY_DLG,                 //# 票卡-票卡查询
    REREGISTER_DLG,                   //# 票卡-票卡补登
    PAYMENT_DLG,                //# 票卡-支付
    QR_QUERY_DLG,                 //# 二维码-查询
    QR_REREGISTER_DLG,                   //# 二维码-补登
    MAP_DLG,                   //# 地图导览
    LINE_DLG,                 //# 线路查询
    PRICE_DLG,                //# 票价信息
    TRANSFER_DLG,                   //# 换乘查询
    TIME_DLG,                    //# 时刻表
    PERIPHERY_DLG              //# 地铁周边
}EN_WGT_ID;

typedef enum
{
    ST_SYSTEM_PARAM_ID          = 1,   // 系统参数
    ST_ONLINE_STATE_ID            = 2,   // 节点状态界面所需数据
    ST_DTC_DATA_ID				    = 3,	// 故障诊断界面所需数据
    LINE_TIME				    = 4,   // 运行时间表
    LINE_INTERCHANGE = 5,               // 站点换乘信息
    LINE_STATION_LIST                    = 6   // 线路站点名称

}EN_PARAM_ID;


#endif // COMMONHEAD_H
