#include "InquiryMainWidget.h"
#include "ui_InquiryMainWidget.h"

#include "SettingCenter.h"
#include "QNChatMessage.h"
#include "HttpTool.h"
#include "ASRHttpTool.h"
#include "AsrResult.h"
#include "AsrConfig.h"
#include "DataCenter.h"
#include "MyHelper.h"
#include "ISMWaiting.h"

InquiryMainWidget::InquiryMainWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::InquiryMainWidget)
{
    ui->setupUi(this);

    init();

#if IS_TEST_MODE
    setTestData();
#endif
}

InquiryMainWidget::~InquiryMainWidget()
{
    delete ui;
}


bool InquiryMainWidget::showData()
{
    HttpTool::getThis()->requestHotIssues();
    ui->msgListWidget->clear();
    showMsg("请问有什么可以帮您？", QNChatMessage::User_She);

    m_sessionId = DataCenter::getThis()->getDeviceId() + QString::number(QDateTime::currentSecsSinceEpoch());
    HttpTool::getThis()->requestHotIssues();
    return true;
}

void InquiryMainWidget::init()
{
    m_asrOk = false;
    initAudio();
    m_isAudio = false;
    ui->voice->setCheckable(true);
    if (m_audio == NULL)
    {
        ui->voice->setDisabled(true);
    }

    setStyle();
    connect(ui->send, &QPushButton::clicked, this, &InquiryMainWidget::onMsgSend);
    connect(ui->hotListWidget, &QListWidget::itemClicked, this, &InquiryMainWidget::onQuickHotIssue);
    connect(ui->voice, &QPushButton::clicked, this, &InquiryMainWidget::onVoice);

    // 热点问题&智能问答
    connect(HttpTool::getThis(), &HttpTool::hotIssuesReceived, this, &InquiryMainWidget::onHotIssuesShow);
    connect(HttpTool::getThis(), &HttpTool::answerReceived, this, &InquiryMainWidget::onAnswerShow);
    connect(ASRHttpTool::getThis(), &ASRHttpTool::asrResultOk, this, &InquiryMainWidget::onAsrResultShow);

}


// 录音机初始化
void InquiryMainWidget::initAudio()
{
    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if(info.isNull()) {
        MyHelper::ShowMessageBoxError("未找到录音设备，语音提问功能暂时不可用。");
        logger()->error("未找到录音设备");
        return;
    }
    if (!info.isFormatSupported(format)) {
        logger()->warn("Default format not supported, trying to use the nearest.");
        format = info.nearestFormat(format);
    }

    m_audio = NULL;
    m_audio = new QAudioInput(info, format, this);
    if (m_audio != NULL) {
        connect(m_audio, &QAudioInput::stateChanged, this, &InquiryMainWidget::handleStateChanged);
    }

    // 内存IO对象
    m_bufDevice.setBuffer(&m_voiceData);

    // 语音转文字配置信息
    m_asrConfig = new AsrConfig();
    m_asrConfig->setAudioFormat("pcm_s16le_16k");
    m_asrConfig->setIsAddPunc(true);
}


void InquiryMainWidget::setTestData()
{
    QList<QString> issues = QList<QString>() << "票价" << "无法扫码进站" << "无法正常生成乘车二维码";
    onHotIssuesShow(issues);
}

void InquiryMainWidget::onMsgSend()
{
    // 获取文字
    QString msg = ui->lineEdit->text();
    if (msg.isEmpty()) {
        return;
    }
    ui->lineEdit->setText("");

    showMsg(msg, QNChatMessage::User_Me);

    // 调用应答接口
    HttpTool::getThis()->requestAnswer(msg, m_sessionId);
}

void InquiryMainWidget::onService(QString msg)
{
    showMsg(msg, QNChatMessage::User_She);
}

// 热门问题显示
void InquiryMainWidget::onHotIssuesShow(QList<QString> issues)
{
    ui->hotListWidget->clear();
    for (QString issue:issues) {
        ui->hotListWidget->addItem(issue);
    }
}

// 问答结果显示
void InquiryMainWidget::onAnswerShow(QString answer)
{
    showMsg(answer, QNChatMessage::User_She);
}

// 语音转文字结果处理
void InquiryMainWidget::onAsrResultShow(QString text)
{
//    m_waiting->close();
    // 对于识别结果不好的，进行提示
    if (text.isEmpty()) {
        MyHelper::ShowMessageBoxInfo("语音识别效果不好，请您再说一遍。");
        return;
    }
    ui->lineEdit->setText(text);
}

// 热门问题快速访问
void InquiryMainWidget::onQuickHotIssue(QListWidgetItem* item)
{
    item->setSelected(false);
    QString issue = item->text();
    showMsg(issue, QNChatMessage::User_Me);

    // 调用应答接口
    HttpTool::getThis()->requestAnswer(issue, m_sessionId);
}

void InquiryMainWidget::onVoice()
{
    if (m_isAudio) {
        onStopVoice();

    } else {
        onStartVoice();
    }
}

void InquiryMainWidget::onStartVoice()
{
    m_bufDevice.open(QIODevice::WriteOnly | QIODevice::Truncate);
    m_audio->start(&m_bufDevice);

    ui->voice->setChecked(true);
    m_isAudio = true;
}

void InquiryMainWidget::onStopVoice()
{
    // 抬起停止录音，并开始识别
    m_audio->stop();
    QString data = m_bufDevice.buffer().toBase64();
    ASRHttpTool::getThis()->postForVoiceToText(m_asrConfig, data);

    m_bufDevice.close();
    ui->voice->setChecked(false);
    m_isAudio = false;

    // 开启等待窗口
//    m_waiting = new ISMWaiting();
//    m_waiting->show();
}

void InquiryMainWidget::handleStateChanged(QAudio::State newState)
{
    switch (newState) {
    case QAudio::StoppedState:
        if (m_audio->error() != QAudio::NoError) {
            // Error handling
        } else {
            // Finished recording
        }
        break;

    case QAudio::ActiveState:
        // Started recording - read from IO device
        break;

    default:
        // ... other cases as appropriate
        break;
    }
}

void InquiryMainWidget::showMsg(QString msg, QNChatMessage::User_Type type)
{
    QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳

    dealMessageTime(time);
    QNChatMessage* messageW = new QNChatMessage(ui->msgListWidget);
    QListWidgetItem* item = new QListWidgetItem(ui->msgListWidget);
    dealMessage(messageW, item, msg, time, type);
    ui->msgListWidget->setCurrentRow(ui->msgListWidget->count() - 1);
}

void InquiryMainWidget::dealMessage(QNChatMessage *messageW, QListWidgetItem *item,
                                    QString text, QString time,  QNChatMessage::User_Type type)
{
    messageW->setFixedWidth(ui->msgListWidget->width());
    QSize size = messageW->fontRect(text);
    item->setSizeHint(size);
    messageW->setText(text, time, size, type);
    ui->msgListWidget->setItemWidget(item, messageW);
}

void InquiryMainWidget::dealMessageTime(QString curMsgTime)
{
    bool isShowTime = false;
    if(ui->msgListWidget->count() > 0) {
        QListWidgetItem* lastItem = ui->msgListWidget->item(ui->msgListWidget->count() - 1);
        QNChatMessage* messageW = (QNChatMessage*)ui->msgListWidget->itemWidget(lastItem);
        int lastTime = messageW->time().toInt();
        int curTime = curMsgTime.toInt();
        qDebug() << "curTime lastTime:" << curTime - lastTime;
        isShowTime = ((curTime - lastTime) > 60); // 两个消息相差一分钟
//        isShowTime = true;
    } else {
        isShowTime = true;
    }
    if(isShowTime) {
        QNChatMessage* messageTime = new QNChatMessage(ui->msgListWidget);
        QListWidgetItem* itemTime = new QListWidgetItem(ui->msgListWidget);

        QSize size = QSize(ui->msgListWidget->width(), 40);
        messageTime->resize(size);
        itemTime->setSizeHint(size);
        messageTime->setText(curMsgTime, curMsgTime, size, QNChatMessage::User_Time);
        ui->msgListWidget->setItemWidget(itemTime, messageTime);
    }
}

void InquiryMainWidget::setStyle()
{
    setStyleSheet(
        "QListWidget"
        "{"
        "    background-color: rgb(247, 247, 247); "
        "    color:rgb(51,51,51); "
        "    border: 1px solid  rgb(247, 247, 247);"
        "    outline:0px;"
        "}"
        "QListWidget::Item"
        "{"
        "    background-color: rgb(247, 247, 247);"
        "}"
        "QListWidget::Item:hover"
        "{"
        "    background-color: rgb(247, 247, 247); "
        "}"
        "QListWidget::item:selected"
        "{"
        "	background-color: rgb(247, 247, 247);"
        "	color:black; "
        "    border: 1px solid  rgb(247, 247, 247);"
        "}"
        "QListWidget::item:selected:!active"
        "{"
        "    border: 1px solid  rgb(247, 247, 247); "
        "    background-color: rgb(247, 247, 247); "
        "    color:rgb(51,51,51); "
        "}"

        "QListWidget#hotListWidget"
        "{"
        "    background-color: transparent; "
        "    color: #09262A; "
        "    font-size: 26px; "
        "    border: none;"
        "    outline: 0px;"
        "}"
        "QListWidget#hotListWidget::Item"
        "{"
        "    background-color: transparent;"
        "    color: #09262A; "
        "    height: 70px; "
        "    margin-left: 10px; "
        "    border-bottom: 1px solid #EEECEB;"
        "}"
        "QListWidget#hotListWidget::item:selected"
        "{"
        "	background-color: #C1EAD8;"
        "   border: 1px solid #C1EAD8;"
        "}"
    );

}



