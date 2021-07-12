#include "ASRHandler.h"



static HciError get_audio_metrics(const char *audio_format,
                                  hci::HciAudioMetrics *m) {
    m->set_channels(1);
    if (strcmp(audio_format, "pcm_s16le_16k") == 0) {
        m->set_format(hci::HciSampleFormat::S16LE);
        m->set_sample_rate(16000);
    } else if (strcmp(audio_format, "pcm_s16le_8k") == 0) {
        m->set_format(hci::HciSampleFormat::S16LE);
        m->set_sample_rate(8000);
    } else if (strcmp(audio_format, "alaw_8k") == 0) {
        m->set_format(hci::HciSampleFormat::ALAW);
        m->set_sample_rate(8000);
    } else if (strcmp(audio_format, "alaw_16k") == 0) {
        m->set_format(hci::HciSampleFormat::ALAW);
        m->set_sample_rate(16000);
    } else if (strcmp(audio_format, "ulaw_8k") == 0) {
        m->set_format(hci::HciSampleFormat::ULAW);
        m->set_sample_rate(8000);
    } else if (strcmp(audio_format, "ulaw_16k") == 0) {
        m->set_format(hci::HciSampleFormat::ULAW);
        m->set_sample_rate(16000);
    } else {
        return HciError::UNIMPLEMENTED;
    }
    return HciError::OK;
}

extern void DoCount(HciError code);
void DoRecognize(FreetalkStream *session, MYCONTEXT *ctx) {
    const char *audio_format = "pcm_s16le_16k";
    int slice = 200;  // 读取分片长度为 200ms

    safe_ptr<hci::HciAudioSource> source;

    hci::HciAudioMetrics m;
    get_audio_metrics(audio_format, &m);
    m.set_frame_time(slice);

    delete ctx->handler;
    ctx->handler = nullptr;

    source = hci::NewDataAudioSource(&m, ctx->data, ctx->len, true);
    if (!source) {
        HciLogE(ctx->sdk, "failed to load audio source: %s, %d", *ctx->data, ctx->len);
        return;
    }

    FreetalkConfig config;  // 请根据开发手册进行参数调整
    config.set_property("cn_16k_common");
    config.set_audio_format(audio_format);
    config.set_mode(FreetalkMode::SHORT_STREAM);
    config.set_add_punc(true);
    config.set_word_type(hci::sdk::asr::HciWordType::WORD);
    config.set_interim_results(true);
    config.set_timeout(60 * 1000);
    config.set_slice(slice);
    config.set_dps(800);  // 数据读取速率，每秒读取 800ms 音频

    ctx->handler = new ASRHandler();  // 必须在识别期间保持可用
    session->Start(&config, source.get(), ctx->handler, true);
}


static bool quit = false;

static void Output(const FreetalkTextResult *result) {
    printf("    text = %s\n", result->text());
    if (result->has_words()) {
//        printf("    words = %s\n", format(result->words()).c_str());
    }
    printf("    confidence = %g\n", result->confidence());
}


ASRHandler::ASRHandler() : FreetalkHandler()
{

}

void ASRHandler::OnStart(FreetalkStream *s, hci::HciError code, const array<Warning> *)
{
    if (code != HciError::OK) {
        // 会话无法启动，不会再调用其他回调
        assert(s->state() != SessionState::Running);  // Idle/Closing/Closed
        DoCount(code);
        printf("  session start fail\n");
        if (!quit)
            DoRecognize(s, reinterpret_cast<MYCONTEXT *>(s->UserGet()));
    } else {
        // 会话启动成功
        printf("  session started\n");
    }

}

void ASRHandler::OnEnd(FreetalkStream *s, EndReason reason)
{
    // 会话结束
    switch (reason) {
    case EndReason::NORMAL:
        printf("  session ended normaly\n");
        break;
    case EndReason::CANCEL:
        printf("  session cancelled\n");
        break;
    case EndReason::ABNORMAL:
        printf("  session ended abnormaly\n");
        break;
    }
    DoCount(m_errCode);
    if (!quit)
        DoRecognize(s, reinterpret_cast<MYCONTEXT *>(s->UserGet()));

}


void ASRHandler::OnError(FreetalkStream *, hci::HciError code)
{
    m_errCode = code;
}

void ASRHandler::OnResult(FreetalkStream *, const FreetalkResult *sentence)
{
    if (sentence->has_result()) {
        // 打印首选结果
        printf("  %s:\n", "sentence");
        printf("    %s = %d\n", "start_time", sentence->start_time());
        printf("    %s = %d\n", "end_time", sentence->end_time());
        printf("    %s = %s\n", "is_final", sentence->is_final() ? "true" : "false");
        Output(sentence->result());
        if (sentence->has_alternatives()) {
            // 有候选结果，逐个打印候选结果
            auto ar = sentence->alternatives();
            size_t i, count = ar->count();
            for (i = 0; i < count; ++i) {
                printf("   %s: %zd\n", " alter result", i);
                Output(ar->at(i));
            }
        }
    } else {
        printf("  %s\n", "empty result");
    }
}

void ASRHandler::OnEvent(FreetalkStream *, const FreetalkEvent *event)
{
    const char *type = nullptr;
    switch (event->type()) {
    case AudioEventType::EXCEEDED_SILENCE:
        type = "ExceededSilence";
        break;
    case AudioEventType::EXCEEDED_END_SILENCE:
        type = "ExceededEndSilence";
        break;
    case AudioEventType::EXCEEDED_AUDIO:
        type = "ExceededAudio";
        break;
    case AudioEventType::VOICE_START:
        type = "VoiceStart";
        break;
    case AudioEventType::VOICE_END:
        type = "VoiceEnd";
        break;
    case AudioEventType::NONE:
        type = "None";
        break;
    case AudioEventType::NOISE:
        type = "Noise";
        break;
    }
    printf("  audio event:\n");
    printf("     timestamp = %d\n", event->timestamp());
    printf("     type = %s\n", type);

}
