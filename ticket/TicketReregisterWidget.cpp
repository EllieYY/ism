#include "TicketReregisterWidget.h"
#include "ui_TicketReregisterWidget.h"
#include "CommonHead.h"
#include "DataCenter.h"
#include "TicketBasicInfo.h"
#include "StationSelectWidget.h"
#include "CompensationFareWidget.h"
#include "MyHelper.h"
#include "NC_ReaderLib.h"
#include "DeviceManager.h"

TicketReregisterWidget::TicketReregisterWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::TicketReregisterWidget)
{
    ui->setupUi(this);

    init();

#if IS_TEST_MODE
    setTestData();
#endif
}

TicketReregisterWidget::~TicketReregisterWidget()
{
    delete ui;
}

void TicketReregisterWidget::setDeviceManager(DeviceManager *devManager)
{
    m_deviceManager = devManager;
}

bool TicketReregisterWidget::showData()
{
    // 更新锁定状态
    if (m_updateLock) {
        return true;
    }

    ui->calcFareBtn->hide();
    ui->cashPollBtn->setDisabled(true);
    ui->tUpdateBtn->setDisabled(true);

    if (!m_dataUpdateNum[TICKET_REREGISTER]) {
        return true;
    }
    m_dataUpdateNum[TICKET_REREGISTER] = false;

    TicketBasicInfo* info = DataCenter::getThis()->getTicketBasicInfo();
    m_ticketType = info->typeNum();
    m_updateType = info->updateType();
    m_difference = info->updateAmount();    

    m_isAllowOctPay = info->isAllowOctPay();

    m_banlance = info->balance();
    m_payType = 0x01;       // 现金支付
    if (m_isAllowOctPay) {  // 卡内扣费
        ui->cashPollBtn->setDisabled(true);
        if (info->icType() == OCT_CARD) {
            m_payType = 0x04;
        }
    }

    // 票卡基本信息显示
    QList<QTableWidgetItem*> itemList = DataCenter::getThis()->getTicketItems(info);
    int index = 0;
    for (QTableWidgetItem* item:itemList) {
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont("Microsoft YaHei",18,500));
        item->setForeground(QColor("#09262A"));
        ui->tableWidget->setItem(0, index++, item);
    }

    // 限制一卡通的使用
    if (info->icType() == OCT_CARD) {
        ui->selectBtn2->setDisabled(true);
        ui->selectBtn3->setDisabled(true);

        ui->textTips->setText("无法操作洪城一卡通，请联系人工处理。");
        ui->cashPollBtn->setDisabled(true);
        ui->tUpdateBtn->setDisabled(true);

        return true;
    }

    // 进站时间 | 出站时间 | 更新原因 | 应收费用
    if (info->enTime().toSecsSinceEpoch() > 1000) {
        ui->lineEdit1->setText(info->enTime().toString("yyyy-MM-dd HH:mm:ss"));
    }

    if (info->exTime().toSecsSinceEpoch() > 1000) {
        ui->lineEdit3->setText(info->exTime().toString("yyyy-MM-dd HH:mm:ss"));
    }

    float updateAmount = 0.01 * info->updateAmount();
    ui->lineEdit6->setText(QString::number(updateAmount));
    ui->textTips->setText("");

    int errorCode = info->errorCode();
    QString errMsg = DataCenter::getThis()->getReaderErrorStr(errorCode);
//    QString reason = DataCenter::getThis()->getUpdateTypeString(m_updateType);
    ui->lineEdit5->setText(errMsg);

    // 操作控制
    ui->selectBtn2->setDisabled(true);
    ui->selectBtn3->setDisabled(true);

    if (!info->isAllowUpdate()) {
        // 无需更新
        if (info->errorCode() == 0) {
            ui->tUpdateBtn->setDisabled(true);
            ui->textTips->setText("票卡无需更新");
            ui->lineEdit5->setText("无需更新");
            MyHelper::ShowMessageBoxInfo("当前票卡无需更新。");
        } else {
            // 不可更新
            ui->tUpdateBtn->setDisabled(true);
            ui->textTips->setText("票卡不可更新");
            ui->lineEdit5->setText("不可更新");

            QString tipsStr = QString("%1，无法进行票卡更新[%2]，请联系工作人员处理。")
                    .arg(errMsg)
                    .arg(info->errorCode());
            MyHelper::ShowMessageBoxInfo(tipsStr);          
        }
        return true;
    }

    bool fareOk = true;
    // 进站更新（付费区）
    m_enStationCode = "";
    if (m_updateType == FARE_EN) {
        if (info->icType() == UL_CARD) {
            // 使用当前站点作为进站
            QString curStation = DataCenter::getThis()->getStationName();
            ui->lineEdit2->setText(curStation);
            m_enStationCode = DataCenter::getThis()->getStationCode();

            fareOk = true;
        } else {
            ui->selectBtn2->setDisabled(false);
            ui->lineEdit2->setText("");
//            ui->lineEdit6->setText("1");   // TODO:delete
            ui->textTips->setText("请补充进站信息。");
            fareOk = false;
        }
    } else {
        QString enStationName = DataCenter::getThis()->stationCode2Name(info->enStationCode());
        ui->lineEdit2->setText(enStationName);
    }

    // 付费出站更新
    m_exStationCode = "";
    if (m_updateType == FARE_EX ) {
        ui->selectBtn3->setDisabled(false);
        ui->lineEdit4->setText("");
        ui->lineEdit6->setText("");  // TODO:delete
        ui->textTips->setText("请补充出站信息。");
        fareOk = false;
    } else {
        QString exStationName = DataCenter::getThis()->stationCode2Name(info->exStationCode());
        ui->lineEdit4->setText(exStationName);
        fareOk = true;
    }

    if (fareOk) {
        m_difference = m_difference < 0 ? 0 : m_difference;

        if ((info->isAllowOctPay() && m_difference <= m_banlance) || m_difference <= 0) {
            ui->cashPollBtn->setDisabled(true);
            ui->tUpdateBtn->setDisabled(false);
        } else if (m_difference > 0) {
            m_payType = 0x01;      // 余额不足时，现金支付
            ui->cashPollBtn->setDisabled(false);
            ui->tUpdateBtn->setDisabled(true);
        }
    } else {
        ui->cashPollBtn->setDisabled(true);
        ui->tUpdateBtn->setDisabled(true);
    }

    return true;
}

void TicketReregisterWidget::init()
{
    m_updateLock = false;
    setStyle();

    m_curBtn = 0;
    connect(ui->selectBtn2, &QPushButton::clicked, this, &TicketReregisterWidget::onEnStationSelect);
    connect(ui->selectBtn3, &QPushButton::clicked, this, &TicketReregisterWidget::onExStationSelect);

    //# 选择车站
    StationSelectWidget* stationSelectedWidget = new StationSelectWidget(this);
    stationSelectedWidget->hide();
    connect(this, &TicketReregisterWidget::selectStation, stationSelectedWidget, &StationSelectWidget::showData);
    connect(stationSelectedWidget, &StationSelectWidget::stationSelected, this, &TicketReregisterWidget::onStationSelected);

    m_updateType = NONE_TYPE;
    connect(ui->tUpdateBtn, &QPushButton::clicked, this, &TicketReregisterWidget::onUpdateTicket);
    connect(ui->cashPollBtn, &QPushButton::clicked, this, &TicketReregisterWidget::cashSupplementary);
    connect(ui->calcFareBtn, &QPushButton::clicked, this, &TicketReregisterWidget::onCalcFare);

    m_difference = 0;

    // 现金交易窗口
//    m_fareWidget = new CompensationFareWidget();
//    connect(m_fareWidget, &CompensationFareWidget::startChecking, m_deviceManager, &DeviceManager::onCheckingCashbox);
//    connect(m_fareWidget, &CompensationFareWidget::supplementaryOk, this, &TicketReregisterWidget::onSupplementaryOk);
//    m_fareWidget->hide();
}

void TicketReregisterWidget::onStationSelected(QString lineName, QString stationName, QString stationCode)
{
    if (m_curBtn) {
        ui->lineEdit4->setText(stationName);
        m_exStationCode = stationCode;
        ui->calcFareBtn->show();
        ui->calcFareBtn->setDisabled(false);
        ui->textTips->setText("请计算补票费用。");
    } else {
        ui->lineEdit2->setText(stationName);
        m_enStationCode = stationCode;
    }
}

void TicketReregisterWidget::onUpdateTicket()
{
    m_updateLock = true;
    BYTE anti = DataCenter::getThis()->getAntiNo();
    UPDATE_CARD_IN updateIn = {0};
    UPDATE_RESP updateTradeData = {0};

    // 参数配置
    // 设备交易流水 | 车站模式 | 车票类型 | 支付方式 | 交易金额 | 更新方式 | 操作员编号 | 更新车站（进站出站更新要填）

    // 设备交易流水号，不是交易文件序号
    ulong tradeSerial = DataCenter::getThis()->getDeviceTradeSerial();

    MyHelper::intToBigEndianByte(tradeSerial, 4, updateIn.DevTradeSerial);
    int stationMode = DataCenter::getThis()->getStationMode();
    MyHelper::intToBigEndianByte(stationMode, 2, updateIn.StationMode);
    updateIn.CardType = m_ticketType;

    if (m_difference <= 0) {
        m_payType = 0x01;
    }
    updateIn.PayType = m_payType;

//    int updateAmount = m_difference * 100;    // 单位是分
//    qDebug() << "updateAmount=" << updateAmount;
    MyHelper::intToBigEndianByte(m_difference, 4, updateIn.Amount);

    updateIn.UpdateType = m_updateType;
    QString operatorIdStr = DataCenter::getThis()->getOperatorId();
    MyHelper::hexStrToByte(operatorIdStr, 4, updateIn.OperatorID);

    if (m_updateType == FARE_EX) {
        MyHelper::hexStrToByte(m_exStationCode, 2, updateIn.UpdateStation);
    } else if (m_updateType == FARE_EN) {
        MyHelper::hexStrToByte(m_enStationCode, 2, updateIn.UpdateStation);
    }

    // 票卡更新参数打印
    QByteArray array;
    array.append((char*)&updateIn, sizeof(updateIn));
    QString str = array.toHex().toUpper();
    logger()->info("票卡更新参数：%1", str);

    // 票卡更新
    BYTE ret = cardUpdate(anti, &updateIn, &updateTradeData);
    if (ret == 0x05 || ret == 0x06) {
        MyHelper::ShowMessageBoxInfo("请将票卡正确放置在感应区域内。");
        m_updateLock = false;
        return;
    } else if (ret != 0) {
        QString errMsg = DataCenter::getThis()->getReaderErrorStr(ret);
        MyHelper::ShowMessageBoxError(QString("票卡更新失败{%1}:%2，请重试或者联系工作人员。")
                                      .arg(QString::number(ret, 16))
                                      .arg(errMsg));
        m_updateLock = false;
        return;
    }

    // 票卡更新参数打印
    QByteArray array1;
    array1.append((char*)&updateTradeData, sizeof(UPDATE_RESP));
    QString str1 = array1.toHex().toUpper();
    logger()->info("票卡更新返回：%1", str1);

    // 交易记录写文件
    this->writeTradeFile(updateTradeData.ICType, updateTradeData.TraderRespData);

    // TODO:票卡更新返回同步到ISM后台

    // 更新成功提示。
    MyHelper::ShowMessageBoxInfo("票卡更新成功。");
    ui->tUpdateBtn->setDisabled(true);
    ui->cashPollBtn->setDisabled(true);
//    close();

    m_updateLock = false;
}


// 现金补缴
void TicketReregisterWidget::cashSupplementary()
{
    //TODO:test code
    //    m_difference = ui->lineEdit6->text().toInt();

    float amount = ui->lineEdit6->text().toFloat();
    int iAmount = qCeil(amount);
    m_difference = 100 * iAmount;

    if (m_difference > 0) {
        BYTE state = DataCenter::getThis()->getCashboxState();
        m_fareWidget->initShow(iAmount, state);
    } else {
        MyHelper::ShowMessageBoxInfo("无需现金缴费，请直接更新。");
        ui->cashPollBtn->setDisabled(true);
        ui->tUpdateBtn->setDisabled(false);
    }
}


// 费用计算
void TicketReregisterWidget::onCalcFare()
{
    if (m_updateType != FARE_EX) {
        return;
    }

    FARECAL_IN calFareIn = {0};
    FARECAL_OUT calFareOut = {0};

    calFareIn.CalMode = 0x06;
    calFareIn.CardType = m_ticketType;
    QByteArray code = MyHelper::hexStrToByte(m_exStationCode);
    BYTE* pCode = (BYTE*)code.data();
    memcpy(calFareIn.TargetStation, pCode, 2);

    BYTE ret = calcFare(&calFareIn, &calFareOut);

    // TODO:test code
//    ret = 0;
//    calFareOut.intAmount = 300;

    if (ret != 0) {
        MyHelper::ShowMessageBoxError(QString("无法计算更新所需费用{%1}，请重试或联系人工客服。").arg(ret));
        return;
    }

    // 参数打印
    QByteArray arrayIn;
    arrayIn.append((char*)&calFareIn, sizeof(FARECAL_IN));
    QString strIn = arrayIn.toHex().toUpper();

    QByteArray arrayOut;
    arrayOut.append((char*)&calFareIn, sizeof(FARECAL_IN));
    QString strOut = arrayOut.toHex().toUpper();

    logger()->info("[calcFare] amount = {%3}, 入参{%1}, 出参{%2}", strIn, strOut, calFareOut.intAmount);

    m_difference = calFareOut.intAmount;  // 单位是分

    float updateAmount = 0.01 * calFareOut.intAmount;
    ui->lineEdit6->setText(QString::number(updateAmount));

    if (m_isAllowOctPay && m_difference < m_banlance) {
        ui->cashPollBtn->setDisabled(true);
        ui->tUpdateBtn->setDisabled(false);
    } else {
        ui->cashPollBtn->setDisabled(false);
        m_payType = 0x01;      // 余额不足时，现金支付
    }
}

void TicketReregisterWidget::onSupplementaryOk(bool result)
{
    // TODO:test code.
//    ui->tUpdateBtn->setDisabled(false);

    if (result) {
//        m_updateLock = true;
        ui->textTips->setText("现金支付成功，请点击更新按钮进行更新。");
        ui->cashPollBtn->setDisabled(true);
        ui->tUpdateBtn->setDisabled(false);
    } else {
        ui->tUpdateBtn->setDisabled(true);
        ui->cashPollBtn->setDisabled(false);
        MyHelper::ShowMessageBoxError("现金支付失败。");
    }
}

void TicketReregisterWidget::writeTradeFile(BYTE icType, BYTE *data)
{
    QString fileType = DataCenter::getThis()->getTradeFileTypeStr(icType);
    QString deviceId = DataCenter::getThis()->getDeviceId();
    QDateTime curTime = QDateTime::currentDateTime();
    QString curTimeStr = curTime.toString("yyyyMMdd");
    int serial = DataCenter::getThis()->getTradeFileSerial();

    QString filePath = QDir::currentPath() + QDir::separator() +
            "sc" + QDir::separator() +
            "transcation" + QDir::separator();

    //临存文件命名规则：交易文件类别（1个字符）+“.”+节点编码（8位）+“.”+YYYYMMDD +“.”+文件序列号（6位）
    QString fileName = QString("%1.%2.%3.%4")
            .arg(fileType).arg(deviceId).arg(curTimeStr)
            .arg(serial, 6, 10, QLatin1Char('0'));
    logger()->info("[writeTradeFile]trade file name: %1", fileName);

    QFile file(filePath + fileName);
    file.open(QIODevice::ReadWrite | QIODevice::Append);   // 追加读写

    QByteArray array;
    array.clear();

    // 文件内容
    int length = getTradeDataLength(icType);
    if (icType == UL_CARD) {
        array.append((char*)(data), length);
    } else {
        array.append((char*)(data + 8), length);
    }

    qDebug() << "文件内容 size = " << array.size();
    QString bodyStr = array.toHex().toUpper();
    logger()->info("[writeTradeFile]fileName={%1},文件内容：", fileName, bodyStr);

    file.write(array);   //这种方式不会有多余字节
    file.close();

    DataCenter::getThis()->addTradeFileInfo(fileName);
}

int TicketReregisterWidget::getTradeDataLength(int icType)
{
    int length = sizeof(MTRCARD_TRADE_INFO);
    switch(icType) {
    case UL_CARD:
        length = sizeof(MTRCARD_TRADE_INFO);
        break;
    case METRO_CARD:
        length = sizeof(MTRCARD_TRADE_INFO);
        break;
    case OCT_CARD:
        length = sizeof(OCTCARD_TRADE_INFO);
        break;
    case TU_CARD:
        length = sizeof(TUCARD_TRADE_INFO);
        break;
    default:
        break;
    }
    return length;

}

void TicketReregisterWidget::setFareWidget(CompensationFareWidget *fareWidget)
{
    m_fareWidget = fareWidget;
    connect(m_fareWidget, &CompensationFareWidget::supplementaryOk, this, &TicketReregisterWidget::onSupplementaryOk);
}


void TicketReregisterWidget::setStyle()
{
    ui->lineEdit1->setDisabled(true);
    ui->lineEdit2->setDisabled(true);
    ui->lineEdit3->setDisabled(true);
    ui->lineEdit4->setDisabled(true);
    ui->lineEdit5->setDisabled(true);
    ui->lineEdit6->setDisabled(true);   // TODO:放开限制

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(72);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setFrameShape(QFrame::NoFrame);

    ui->tableWidget->verticalHeader()->setVisible(false);                 // 列表头不可见
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 表格不可编辑

    ui->tableWidget->setAlternatingRowColors(true);

}

void TicketReregisterWidget::setTestData()
{
    QDateTime enterTime = QDateTime::currentDateTime().addDays(-3);
    QDateTime exitTime = enterTime.addMSecs(32);

//    ReregisterInfo* info = new ReregisterInfo(this);
//    info->setType(1);
//    info->setNumber("100289996");
//    info->setState(0);
//    info->setCreateTime(QDate(2019, 10, 1));
//    info->setValidDate(QDate(2022, 10, 1));
//    info->setBalance(35.6);

//    info->setEnterTime(enterTime);
//    info->setExitTime(exitTime);
//    info->setEnterStation("秋水广场站");
//    info->setExitStation("市民中心站");
//    int reregisterType = OVER_TIME_TRIP;
//    info->setReregisterType(reregisterType);
//    info->setReason(DataCenter::getThis()->getReregisterTypeString(reregisterType));

//    DataCenter::getThis()->setReregisterInfo(info);
}

void TicketReregisterWidget::onEnStationSelect()
{
    m_curBtn = 0;
    emit selectStation();
}

void TicketReregisterWidget::onExStationSelect()
{
    m_curBtn = 1;
    emit selectStation();
}
