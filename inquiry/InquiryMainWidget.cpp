#include "InquiryMainWidget.h"
#include "ui_InquiryMainWidget.h"

#include "SettingCenter.h"
#include "QNChatMessage.h"
#include "HttpTool.h"

InquiryMainWidget::InquiryMainWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::InquiryMainWidget)
{
    ui->setupUi(this);

    init();

#if IS_TEST_MODE
//    setTestData();
#endif
}

InquiryMainWidget::~InquiryMainWidget()
{
    delete ui;
}


bool InquiryMainWidget::showData()
{
    ui->msgListWidget->clear();
    showMsg("请问有什么可以帮您？", QNChatMessage::User_She);
    return true;
}

void InquiryMainWidget::init()
{
    initAudio();
    m_isAudio = false;
    ui->voice->setCheckable(true);
    if (m_audio == NULL)
    {
        ui->voice->setDisabled(true);
    }

    HttpTool::getThis()->requestHotIssues();

    setStyle();
    connect(ui->send, &QPushButton::clicked, this, &InquiryMainWidget::onMsgSend);
    connect(ui->hotListWidget, &QListWidget::itemClicked, this, &InquiryMainWidget::onHotIssue);

    // 录音开始和结束
    connect(ui->voice, &QPushButton::clicked, this, &InquiryMainWidget::onVoice);
}


// 录音机初始化
void InquiryMainWidget::initAudio()
{
    QString filePath = QDir::currentPath() + QDir::separator() + QDir::separator() + "test.raw";
    m_destinationFile.setFileName(filePath);
    m_destinationFile.open( QIODevice::WriteOnly | QIODevice::Truncate );

    QAudioFormat format;
    // Set up the desired format, for example:
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        qWarning() << "Default format not supported, trying to use the nearest.";
        format = info.nearestFormat(format);
    }

    m_audio = NULL;
    m_audio = new QAudioInput(format, this);
    if (m_audio != NULL) {
        connect(m_audio, &QAudioInput::stateChanged, this, &InquiryMainWidget::handleStateChanged);
    }
}


void InquiryMainWidget::setTestData()
{
    QList<QString> issues = QList<QString>() << "票价" << "无法扫码进站" << "无法正常生成乘车二维码";
    onHotIssues(issues);
}

void InquiryMainWidget::onMsgSend()
{
    // 获取文字
    QString msg = ui->lineEdit->text();
    ui->lineEdit->setText("");

    showMsg(msg, QNChatMessage::User_Me);

    // 调用应答接口
    HttpTool::getThis()->requestAnswer(msg);
}

void InquiryMainWidget::onService(QString msg)
{
    showMsg(msg, QNChatMessage::User_She);
}

// 热门问题显示
void InquiryMainWidget::onHotIssues(QList<QString> issues)
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

// 热门问题快速访问
void InquiryMainWidget::onHotIssue(QListWidgetItem* item)
{
    item->setSelected(false);
    QString issue = item->text();
    showMsg(issue, QNChatMessage::User_Me);
    // TODO:
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
//    QTimer::singleShot(6000, this, &InquiryMainWidget::onStopVoice);

    // 开始录音
    m_audioDevice = m_audio->start();
    connect(m_audioDevice, &QIODevice::readyRead,
           this, &InquiryMainWidget::audioReadyRead);

    ui->voice->setChecked(true);
    m_isAudio = true;
}

void InquiryMainWidget::onStopVoice()
{
    // 抬起停止录音，并开始识别
    m_audio->stop();
    qDebug() << "录音设备是否开启:" << m_audioDevice->isOpen();

    ui->voice->setChecked(false);
    m_isAudio = false;
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

void InquiryMainWidget::audioReadyRead()
{

    QByteArray audio = m_audioDevice->readAll();

    size_t length = audio.size();
//    std::unique_ptr<char[]> data(new char[length]);
    char* data = audio.data();
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



