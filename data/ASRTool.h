#ifndef ASRTOOL_H
#define ASRTOOL_H

#include <QObject>
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
using hci::sdk::asr::FreetalkStream;
using hci::sdk::asr::FreetalkShortAudio;
using hci::sdk::asr::FreetalkTextResult;
using hci::sdk::asr::HciWord;
using hci::sdk::asr::HciWordInfo;
using hci::sdk::asr::Warning;


class HCI_LOCAL ASRTool : public QObject
{
    Q_OBJECT
public:
    explicit ASRTool(QObject *parent = nullptr);
    ~ASRTool();

public:
    bool init();
    void run(int len, char **data);
    void exit();

    void onSdkClosed();

private:
    safe_ptr<HciSdk> m_sdk;
    safe_ptr<FreetalkStream> m_ftstm;


public:
    weak_ptr<HciSdk> wp;


signals:

};

#endif // ASRTOOL_H
