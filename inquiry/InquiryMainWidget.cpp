#include "InquiryMainWidget.h"
#include "ui_InquiryMainWidget.h"

#include "SettingCenter.h"
#include "QNChatMessage.h"
#include "ASRTool.h"

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
    if (m_asrTool != NULL) {
        delete m_asrTool;
    }

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
    initHciSdk();
    m_isAudio = false;
    ui->voice->setCheckable(true);
    if (m_audio == NULL || m_session == NULL)
    {
        ui->voice->setDisabled(true);
    }

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

// 获取临时目录路径
static const char *temp_folder() {
    // TODO:
    // TMPDIR 为 unix 平台临时目录环境变量
    const char *p = getenv("TMPDIR");
    if (!p) p = getenv("TEMP");  // TEMP 为 windows 平台临时目录环境变量
    if (!p) p = getenv("TMP");   // TMP 为 windows 平台临时目录环境变量
    if (!p) p = "/tmp";  // 未能通过环境变量获取临时目录时使用默认值
    return p;
}

/** sdk初始化 | 创建能力会话 */
void InquiryMainWidget::initHciSdk()
{
    logger()->info("ASR功能初始化");
    HciSdkConfig config;

    // 创建 SDK 对象
    m_sdk = HciSdk::New();
    logger()->info("创建SDK对象");

    // 日志级别设置为 T (输出所有日志级别的内容)
    m_sdk->set_log_level(hci::sdk::LogLevel::T);

    // 日志存储路径
    QString logPath = QDir::currentPath() + QDir::separator() +
            SettingCenter::getThis()->getHciLogPath();

    // 设置 SDK 初始化参数
    config.set_appkey("aicp_app");
    config.set_secret("QWxhZGRpbjpvcGVuIHNlc2FtZQ");
    config.set_sys_url("https://172.17.100.180:22801/");
    config.set_cap_url("http://172.17.100.180:22800/");
    config.set_data_path(temp_folder());
    config.set_verify_ssl(false);

    logger()->info(QString("sys url: %1").arg(config.sys_url()));
    logger()->info(QString("cap url: %1").arg(config.cap_url()));
    logger()->info(QString("hci.log路径: %1").arg(config.data_path()));

    logger()->info("SDK初始化参数设置。");
    HciError code = m_sdk->Init(&config);
    if (code != HciError::OK) {
        logger()->info("sdk初始化失败");
        m_sdk.reset();
    }

    logger()->info("创建云端 FreetalkShortAudio 会话");
    CloudAsrConfig asr_config;
    m_session = FreetalkShortAudio::NewCloud(m_sdk, &asr_config);

    logger()->info("云端 FreetalkShortAudio 会话");
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

    // TODO:调用应答接口
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
    m_audio->start(&m_destinationFile);
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
    if (m_asrTool == NULL) {
        return;
    }

    QByteArray audio = m_audioDevice->readAll();

    size_t length = audio.size();
//    std::unique_ptr<char[]> data(new char[length]);
    char* data = audio.data();
    m_asrTool->run(length, &data);
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

struct WAVFILEHEADER
{
    // RIFF 头
    char RiffName[4];
    unsigned long nRiffLength;

    // 数据类型标识符
    char WavName[4];

    // 格式块中的块头
    char FmtName[4];
    unsigned long nFmtLength;

    // 格式块中的块数据
    unsigned short nAudioFormat;
    unsigned short nChannleNumber;
    unsigned long nSampleRate;
    unsigned long nBytesPerSecond;
    unsigned short nBytesPerSample;
    unsigned short nBitsPerSample;

    // 数据块中的块头
    char    DATANAME[4];
    unsigned long   nDataLength;
};

qint64 InquiryMainWidget::addWavHeader(QString catheFileName , QString wavFileName)
{
    // 开始设置WAV的文件头
    // 这里具体的数据代表什么含义请看上一篇文章（Qt之WAV文件解析）中对wav文件头的介绍
    WAVFILEHEADER WavFileHeader;
    qstrcpy(WavFileHeader.RiffName, "RIFF");
    qstrcpy(WavFileHeader.WavName, "WAVE");
    qstrcpy(WavFileHeader.FmtName, "fmt ");
    qstrcpy(WavFileHeader.DATANAME, "data");

    // 表示 FMT块 的长度
    WavFileHeader.nFmtLength = 16;
    // 表示 按照PCM 编码;
    WavFileHeader.nAudioFormat = 1;
    // 声道数目;
    WavFileHeader.nChannleNumber = 1;
    // 采样频率;
    WavFileHeader.nSampleRate = 8000;

    // nBytesPerSample 和 nBytesPerSecond这两个值通过设置的参数计算得到;
    // 数据块对齐单位(每个采样需要的字节数 = 通道数 × 每次采样得到的样本数据位数 / 8 )
    WavFileHeader.nBytesPerSample = 2;
    // 波形数据传输速率
    // (每秒平均字节数 = 采样频率 × 通道数 × 每次采样得到的样本数据位数 / 8  = 采样频率 × 每个采样需要的字节数 )
    WavFileHeader.nBytesPerSecond = 16000;

    // 每次采样得到的样本数据位数;
    WavFileHeader.nBitsPerSample = 16;

    QFile cacheFile(catheFileName);
    QFile wavFile(wavFileName);

    if (!cacheFile.open(QIODevice::ReadWrite))
    {
        return -1;
    }
    if (!wavFile.open(QIODevice::WriteOnly))
    {
        return -2;
    }

    int nSize = sizeof(WavFileHeader);
    qint64 nFileLen = cacheFile.bytesAvailable();

    WavFileHeader.nRiffLength = nFileLen - 8 + nSize;
    WavFileHeader.nDataLength = nFileLen;

    // 先将wav文件头信息写入，再将音频数据写入;
    wavFile.write((char *)&WavFileHeader, nSize);
    wavFile.write(cacheFile.readAll());

    cacheFile.close();
    wavFile.close();

    return nFileLen;
}


