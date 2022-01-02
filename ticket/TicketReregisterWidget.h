#ifndef TICKETREREGISTERWIDGET_H
#define TICKETREREGISTERWIDGET_H

#include <QWidget>
#include "WidgetBase.h"
#include "logger.h"
#include "propertyconfigurator.h"

namespace Ui {
class TicketReregisterWidget;
}

class CompensationFareWidget;
class DeviceManager;
class TicketReregisterWidget : public WidgetBase
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit TicketReregisterWidget(QWidget *parent = nullptr);
    ~TicketReregisterWidget();

public:
    void setDeviceManager(DeviceManager* devManager);

    void setFareWidget(CompensationFareWidget *fareWidget);

private:
    bool showData();
    void initShow();

    void setStyle();

    void init();
    void setTestData();

    void onEnStationSelect();
    void onExStationSelect();
    void onStationSelected(QString lineName, QString stationName, QString stationCode);

    void onUpdateTicket();          // 票卡更新
    void cashSupplementary();       // 现金补票
    void onCalcFare();                // 费用计算

    void onSupplementaryOk(bool result);    //  补票结果返回

    void updateBtnControll();     // 更新按钮显示逻辑控制

    // 现金交易文件
    void writeTradeFile(BYTE icType, BYTE* data);
    QString getFileTypeStr(int icType);
    int getTradeDataLength(int icType);
    int getTradeFileSerial();

private:
    int m_curBtn;               // 0-en  1-ex
    int m_updateType;           // 更新类型
    int m_ticketType;           // 票卡类型
    CompensationFareWidget* m_fareWidget;       // 补差额窗口
    uint  m_difference;       // 待补差价

    QString  m_enStationCode;   // 进站站点编号
    QString  m_exStationCode;   // 出站站点编号
    bool m_isAllowOctPay;       // 是否支持卡内扣费
    float m_banlance;           // 余额
    BYTE m_payType;             // 支付方式

    DeviceManager* m_deviceManager;
    bool m_updateLock;       // 更新锁定状态：现金支付之后，不刷新界面数据

signals:
    void selectStation();    // 选择车站

private:
    Ui::TicketReregisterWidget *ui;
};

#endif // TICKETREREGISTERWIDGET_H
