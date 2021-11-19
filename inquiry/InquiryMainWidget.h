#ifndef INQUIRYMAINWIDGET_H
#define INQUIRYMAINWIDGET_H

#include <QWidget>
#include "WidgetBase.h"
#include "QNChatMessage.h"
#include <QListWidgetItem>
#include <QAudioInput>
#include "logger.h"
#include "propertyconfigurator.h"


namespace Ui {
class InquiryMainWidget;
}
class QNChatMessage;
class AsrResult;
class AsrConfig;
class ISMWaiting;
class InquiryMainWidget : public WidgetBase
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit InquiryMainWidget(QWidget *parent = nullptr);
    ~InquiryMainWidget();

    void onHotIssuesShow(QList<QString> issues);        //# 热点问题显示
    void onAnswerShow(QString answer);                  //# 问答结果显示
    void onAsrResultShow(QString text);                 //# 语音转文字结果显示

private:
    bool showData();
    void setStyle();

    void init();
    void initAudio();
    void setTestData();

    void onMsgSend();                                       //# 问题发送
    void onService(QString msg);
    void onQuickHotIssue(QListWidgetItem* item);            //# 热点问题快速问答
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

private:
    bool            m_isAudio;     // 是否正在录音
    QAudioInput*    m_audio;
    QIODevice*      m_audioDevice;
    AsrConfig*      m_asrConfig;

    // test.
    QByteArray      m_voiceData;
    QBuffer         m_bufDevice;
    QString         m_sessionId;    // 会话Id

    bool            m_asrOk;

    ISMWaiting*     m_waiting;    // 等待窗口

private:
    Ui::InquiryMainWidget *ui;
};

#endif // INQUIRYMAINWIDGET_H
