#include "TicketReregisterWidget.h"
#include "ui_TicketReregisterWidget.h"
#include "CommonHead.h"
#include "DataCenter.h"
#include "TicketBasicInfo.h"
#include "StationSelectWidget.h"
#include "CompensationFareWidget.h"
#include "MyHelper.h"
#include "NC_ReaderLib.h"

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

bool TicketReregisterWidget::showData()
{
    ui->calcFareBtn->hide();
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
        m_payType = 0x04;
        ui->cashPollBtn->setDisabled(true);
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
    // 进站时间 | 出站时间 | 更新原因 | 应收费用
    ui->lineEdit1->setText(info->enTime().toString("yyyy-MM-dd HH:mm:ss"));
    ui->lineEdit3->setText(info->exTime().toString("yyyy-MM-dd HH:mm:ss"));
    QString reason = DataCenter::getThis()->getUpdateTypeString(m_updateType);
    ui->lineEdit5->setText(reason);
    ui->lineEdit6->setText(QString::number(info->updateAmount()));
    ui->textTips->setText("");

    // 操作控制
    ui->selectBtn2->setDisabled(true);
    ui->selectBtn3->setDisabled(true);

    // 提示用户无法更新
    if (!info->isAllowUpdate() || m_updateType == NONE_TYPE) {
        ui->tUpdateBtn->setDisabled(true);
        MyHelper::ShowMessageBoxInfo("当前票卡无法进行票卡更新，请联系工作人员处理。");
        return true;
    }

    // 进站更新（付费区）
    m_enStationCode = "";
    if (m_updateType == FARE_EN) {
        if (info->typeNum() == UL_CARD) {
            // 使用当前站点作为进站
            QString curStation = DataCenter::getThis()->getStationName();
            ui->lineEdit2->setText(curStation);
            m_enStationCode = DataCenter::getThis()->getStationCode();
        } else {
            ui->selectBtn2->setDisabled(false);
            ui->lineEdit2->setText("");
            ui->lineEdit6->setText("0");
            ui->textTips->setText("请补充进站信息。");
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
        ui->lineEdit6->setText("--");
        ui->textTips->setText("请补充出站信息。");
    } else {
        QString exStationName = DataCenter::getThis()->stationCode2Name(info->exStationCode());
        ui->lineEdit4->setText(exStationName);
    }

    m_difference = m_difference < 0 ? 0 : m_difference;

    if ((info->isAllowOctPay() && m_difference <= m_banlance) || m_difference <= 0) {
        ui->cashPollBtn->setDisabled(true);
    } else if (m_difference > 0) {
        m_payType = 0x01;      // 余额不足时，现金支付
        ui->calcFareBtn->setDisabled(false);
        ui->tUpdateBtn->setDisabled(true);
    }

    return true;
}

void TicketReregisterWidget::init()
{
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
    m_tradeFileSerial = 1;
}

void TicketReregisterWidget::onStationSelected(QString lineName, QString stationName, QString stationCode)
{
    if (m_curBtn) {
        ui->lineEdit4->setText(stationName);
        m_exStationCode = stationCode;
        ui->calcFareBtn->show();
    } else {
        ui->lineEdit2->setText(stationName);
        m_enStationCode = stationCode;
    }
}

void TicketReregisterWidget::onUpdateTicket()
{
    // TODO:test code
//    DataCenter::getThis()->uploadTradeFile("Y.04200403.20211119230228.000001",
//                                           "Y.04200403.20211119230228.000001",
//                                           QString("b15e9f2bb97712604d12fca506c7548c").toUtf8(),
//                                           0x01);

    BYTE anti = DataCenter::getThis()->getAntiNo();
    UPDATE_CARD_IN updateIn = {0};
    UPDATE_RESP updateTradeData = {0};

    // 参数配置
    // 设备交易流水 | 车站模式 | 车票类型 | 支付方式 | 交易金额 | 更新方式 | 操作员编号 | 更新车站（进站出站更新要填）
    int tradeSerial = DataCenter::getThis()->getTradeSerial();
    MyHelper::intToBigEndianByte(tradeSerial, 4, updateIn.DevTradeSerial);
    int stationMode = DataCenter::getThis()->getStationMode();
    MyHelper::intToBigEndianByte(stationMode, 2, updateIn.StationMode);
    updateIn.CardType = m_ticketType;
    updateIn.PayType = m_payType;
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
    if (ret != 0) {
        MyHelper::ShowMessageBoxError(QString("票卡更新失败{%1}，请重试或者联系工作人员。").arg(QString::number(ret, 16)));
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
//    close();
}


// 现金补缴
void TicketReregisterWidget::cashSupplementary()
{
    if (m_difference > 0) {
        BYTE state = DataCenter::getThis()->getCashboxState();
        m_fareWidget = new CompensationFareWidget();
        connect(m_fareWidget, &CompensationFareWidget::supplementaryOk, this, &TicketReregisterWidget::onSupplementaryOk);
        m_fareWidget->initShow(m_difference, state);
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
    if (ret != 0) {
        MyHelper::ShowMessageBoxError(QString("无法计算更新所需费用{%1}，请重试或联系人工客服。").arg(ret));
        return;
    }

    m_difference = calFareOut.intAmount;
    ui->lineEdit6->setText(QString::number(m_difference * 0.01));
    if (m_isAllowOctPay && m_difference < m_banlance) {
        ui->cashPollBtn->setDisabled(true);
        m_payType = 0x04;      // 卡内扣费
    } else {
        ui->cashPollBtn->setDisabled(false);
        m_payType = 0x01;      // 余额不足时，现金支付
    }
}

void TicketReregisterWidget::onSupplementaryOk(bool result)
{
    // TODO:test code.
    ui->tUpdateBtn->setDisabled(false);

    if (result) {
        ui->textTips->setText("现金支付成功，请点击更新按钮进行更新。");
        ui->tUpdateBtn->setDisabled(false);
    } else {
        ui->tUpdateBtn->setDisabled(true);
        MyHelper::ShowMessageBoxError("现金支付失败，请联系工作人员处理。");
    }
}

void TicketReregisterWidget::writeTradeFile(BYTE icType, BYTE *data)
{
    QString fileType = getFileTypeStr(icType);
    QString deviceId = DataCenter::getThis()->getDeviceId();
    QDateTime curTime = QDateTime::currentDateTime();
    QString curTimeStr = curTime.toString("yyyyMMddHHmmss");
    int serial = getTradeFileSerial();

    //命名规则：交易文件类别（1个字符）+“.”+节点编码（8位）+“.”+YYYYMMDDHHMMSS +“.”+文件序列号（6位）
    QString fileName = QString("%1.%2.%3.%4")
            .arg(fileType).arg(deviceId).arg(curTimeStr)
            .arg(serial, 6, 10, QLatin1Char('0'));
    qDebug() << "trade file name: " << fileName;

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    // 文件头
    QByteArray array;
    array.clear();

    array.append(icType); // 文件类型
    array.append(MyHelper::hexStrToByte(curTimeStr));
    array.append(0x01);
    array.append(MyHelper::hexStrToByte(deviceId));
    array.append(MyHelper::intToBytes(serial, 4));
    int num = 1;
    array.append(MyHelper::intToBytes(num, 4));

    qDebug() << "文件头 size = " << array.size();
    QString headStr = array.toHex().toUpper();
    qDebug() << "文件头内容：" << headStr;

    // 文件内容
    int length = num * getTradeDataLength(icType);
    qDebug() << "num = " << num << "; length = " << length;
    array.append((char*)(data + 8), length);

    qDebug() << "文件内容 size = " << array.size();
    QString bodyStr = array.toHex().toUpper();
    qDebug() << "文件头内容：" << bodyStr;

   // MD5校验值
    QByteArray md5Arr =  QCryptographicHash::hash(array, QCryptographicHash::Md5);
    QString md5Str = md5Arr.toHex();
    qDebug() << "md5: " << md5Str;
    array.append(MyHelper::hexStrToByte(md5Str));
    qDebug() << "内容大小：" << array.size();

    file.write(array);   //这种方式也不会有多余字节
    file.close();

    // 文件上传
    DataCenter::getThis()->uploadTradeFile(fileName, fileName, md5Arr, icType);
}

QString TicketReregisterWidget::getFileTypeStr(int icType)
{
    QString str = "Y";
    switch(icType) {
    case UL_CARD:
        str = "S";
        break;
    case METRO_CARD:
        str = "V";
        break;
    case OCT_CARD:
        str = "Y";
        break;
    default:
        break;
    }
    return str;

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
    default:
        break;
    }
    return length;

}

int TicketReregisterWidget::getTradeFileSerial()
{
    if(m_tradeFileSerial > 9999) {
        m_tradeFileSerial = 0;
    }
    return m_tradeFileSerial++;
}


void TicketReregisterWidget::setStyle()
{
    ui->lineEdit1->setDisabled(true);
    ui->lineEdit2->setDisabled(true);
    ui->lineEdit3->setDisabled(true);
    ui->lineEdit4->setDisabled(true);
    ui->lineEdit5->setDisabled(true);
    ui->lineEdit6->setDisabled(true);

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
