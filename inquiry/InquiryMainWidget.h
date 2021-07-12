#ifndef INQUIRYMAINWIDGET_H
#define INQUIRYMAINWIDGET_H

#include <QWidget>
#include "WidgetBase.h"
#include "QNChatMessage.h"
#include <QListWidgetItem>
#include <QAudioInput>
#include "logger.h"
#include "propertyconfigurator.h"

#include "hci/sdk.h"
#include "hci/freetalk.h"

using hci::weak_ptr;
using hci::safe_ptr;
using hci::array;
using hci::HciError;
using hci::sdk::HciSdk;
using hci::sdk::HciSdkConfig;
using hci::sdk::HciTimer;
using hci::sdk::SdkState;
using hci::sdk::SessionState;

using hci::sdk::asr::AudioEventType;
using hci::sdk::asr::CloudAsrConfig;
using hci::sdk::asr::EndReason;
using hci::sdk::asr::FreetalkConfig;
using hci::sdk::asr::FreetalkEvent;
using hci::sdk::asr::FreetalkHandler;
using hci::sdk::asr::FreetalkMode;
using hci::sdk::asr::FreetalkResult;
using hci::sdk::asr::FreetalkShortAudio;
using hci::sdk::asr::FreetalkTextResult;
using hci::sdk::asr::HciWord;
using hci::sdk::asr::HciWordInfo;
using hci::sdk::asr::Warning;


class ASRTool;
namespace Ui {
class InquiryMainWidget;
}
class QNChatMessage;
class InquiryMainWidget : public WidgetBase
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit InquiryMainWidget(QWidget *parent = nullptr);
    ~InquiryMainWidget();

private:
    bool showData();
    void setStyle();

    void init();
    void initAudio();
    void initHciSdk();
    void setTestData();

    void onMsgSend();
    void onService(QString msg);
    void onHotIssues(QList<QString> issues);
    void onHotIssue(QListWidgetItem* item);
    void onVoice();
    void onStartVoice();
    void onStopVoice();
    void handleStateChanged(QAudio::State newState);
    void audioReadyRead();

    void showMsg(QString msg, QNChatMessage::User_Type type);
    void dealMessageTime(QString curMsgTime);
    void dealMessage(QNChatMessage *messageW,
                     QListWidgetItem *item,
                     QString text, QString time,
                     QNChatMessage::User_Type type);

    qint64 addWavHeader(QString catheFileName , QString wavFileName);

private:
    bool            m_isAudio;     // 是否正在录音
    QFile           m_destinationFile;
    QAudioInput*    m_audio;
    QIODevice*      m_audioDevice;
    ASRTool*        m_asrTool;

    safe_ptr<HciSdk> m_sdk;
    safe_ptr<FreetalkShortAudio> m_session;

private:
    Ui::InquiryMainWidget *ui;
};

#endif // INQUIRYMAINWIDGET_H
