#ifndef ASRHANDLER_H
#define ASRHANDLER_H

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
using hci::sdk::asr::FreetalkTextResult;
using hci::sdk::asr::HciWord;
using hci::sdk::asr::HciWordInfo;
using hci::sdk::asr::Warning;



/**
 * FreetalkStream 识别中使用的回调
 */
class HCI_LOCAL ASRHandler : public FreetalkHandler
{
public:
    explicit ASRHandler();

public:
    void OnStart(FreetalkStream *s, HciError code,
                   const array<Warning> * /*warnings*/) override;
    void OnEnd(FreetalkStream *s, EndReason reason) override;
    void OnEvent(FreetalkStream *, const FreetalkEvent *event) override;
    void OnResult(FreetalkStream *, const FreetalkResult *sentence) override;
    void OnError(FreetalkStream *, HciError code) override;


public:
    HciError m_errCode;
};

/**
 * 会话上下文，与 FreetalkStream 能力会话一对一绑定。
 * 在能力会话对象被释放时删除
 */
struct HCI_LOCAL MYCONTEXT {
    MYCONTEXT(HciSdk *sdk) : sdk(sdk), handler(nullptr) {
//        HciLogW(sdk, "%s: %s this = %p", tag, FUNCNAME, this);
    }
    ~MYCONTEXT() {  //
//        HciLogW(sdk, "%s: %s this = %p", tag, FUNCNAME, this);
        delete handler;  // C++ 中允许 delete 空指针，是一个空操作
    }

    static MYCONTEXT *cast_from(void *p) {
        return reinterpret_cast<MYCONTEXT *>(p);
    }

    static void alloc(hci::HciObject *obj, HciSdk *sdk) {
        MYCONTEXT *ctx = new MYCONTEXT(sdk);
        if (!obj->UserSet(ctx, [](void *p) { delete cast_from(p); })) {
            delete ctx;
        }
    }

    static MYCONTEXT *get(hci::HciObject *obj) {
        return cast_from(obj->UserGet());
    }

    HciSdk *sdk;  // 因为是一对一绑定，可以保证 MYCONTEXT 被销毁前一直可用
    ASRHandler *handler;  // 在能力会话运行期间可用
    safe_ptr<HciTimer> timer;
    FreetalkResult res;

    int seq;
    int len;
    char **data;
};

#endif // ASRHANDLER_H
