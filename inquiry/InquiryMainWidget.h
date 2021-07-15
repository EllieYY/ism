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
class InquiryMainWidget : public WidgetBase
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit InquiryMainWidget(QWidget *parent = nullptr);
    ~InquiryMainWidget();

    void onHotIssues(QList<QString> issues);
    void onAnswerShow(QString answer);

private:
    bool showData();
    void setStyle();

    void init();
    void initAudio();
    void setTestData();

    void onMsgSend();
    void onService(QString msg);
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

private:
    bool            m_isAudio;     // 是否正在录音
    QFile           m_destinationFile;
    QAudioInput*    m_audio;
    QIODevice*      m_audioDevice;

private:
    Ui::InquiryMainWidget *ui;
};

#endif // INQUIRYMAINWIDGET_H
