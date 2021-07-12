
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/tts.h
 *
 * 语音合成
 */
#ifndef HCI_TTS_H_
#define HCI_TTS_H_

#include <hci/sdk.h>
#include <hci/audio.h>
#include <hci/tts/common.h>

HCI_NS_BEGIN(hci, sdk, tts);

/**
 * 数字读法
 */
enum struct DigitMode : uint8_t {
  NUMBER_AUTO = 0,  ///< 在自动判断规则的前提下，优先数目读法
  TELEGRAM_AUTO = 1,  ///< 在自动判断规则的前提下，优先电报读法
  TELEGRAM_ONLY = 2,  ///< 统一电报读法，如“16”读作“幺六”
  NUMBER_ONLY = 3,    ///< 统一数目读法，如“16”读作“十六”
};
HCI_ENUM_NAME(DigitMode, NUMBER_AUTO, TELEGRAM_AUTO, TELEGRAM_ONLY,
              NUMBER_ONLY);

#ifdef ECHO
#undef ECHO
#endif

/**
 * 音效
 */
enum struct SoundEffect : uint8_t {
  NONE = 0,     ///< 无音效
  REVERB = 1,   ///< 混响
  ECHO = 2,     ///< 回声
  CHORUS = 3,   ///< 合唱
  NEARFAR = 4,  ///< 忽远忽近
  ROBOT = 5,    ///< 机器人
};
HCI_ENUM_NAME(SoundEffect, NONE, REVERB, ECHO, CHORUS, NEARFAR, ROBOT);

/**
 * TTS 能力会话识别设置
 *
 *   - 10.5.0 版本可用
 */
// clang-format off
HCI_DECL_TABLE(SynthConfig)
  HCI_XPSTR_FIELD(const char *, access_token, true);  // [LP]
  HCI_XPSTR_FIELD(const char *, property, true);      // [LP]
  HCI_XPSTR_FIELD(const char *, format, true);        // [LP]
  HCI_XPSTR_FIELD(const char *, extra_info, true);    // [LP]

  HCI_PLAIN_FIELD(int32_t, sample_rate, true);  // [I4]
  HCI_PLAIN_FIELD(int32_t, pitch, true);        // [I4] 范围: [-500, 500]
  HCI_PLAIN_FIELD(int32_t, volume, true);       // [I4] 范围: [0, 100]
  HCI_PLAIN_FIELD(int32_t, speed, true);        // [I4] 范围: [-500, 500]
  HCI_PLAIN_FIELD(int32_t, timeout, true);      // [I4] 合成超时，单位: ms，默认: 无超时
  HCI_PLAIN_FIELD(int32_t, slice, true);        // [I4] 分片

  HCI_PLAIN_FIELD(DigitMode, digit_mode, true);      // [B1]
  HCI_PLAIN_FIELD(SoundEffect, sound_effect, true);  // [B1]
  HCI_PLAIN_FIELD(bool, punc_mode, true);            // [B1]
  HCI_PLAIN_FIELD(bool, use_s3ml, true);             // [B1]
HCI_DECL_TABLE_END(SynthConfig)
// clang-format on

// clang-format off
HCI_DECL_TABLE(SynthResult)
  HCI_XPSTR_FIELD(const char *, type, true);  // [LP]
  HCI_BYTES_FIELD(void *, audio, true);       // [LP]
HCI_DECL_TABLE_END(SynthResult)
// clang-format on

/**
 * 警告消息。
 */
// clang-format off
HCI_DECL_TABLE(Warning)
  HCI_XPSTR_FIELD(const char *, message, true);
  HCI_PLAIN_FIELD(int32_t, code, true);
HCI_DECL_TABLE_END(Warning);
// clang-format on

/**
 * 会话结束原因
 */
enum struct EndReason : uint8_t {
  NORMAL = 0,    // 正常结束
  CANCEL = 1,    // 用户取消
  ABNORMAL = 2,  // 发生错误
};
HCI_ENUM_NAME(EndReason, NORMAL, CANCEL, ABNORMAL);

class HCI_LOCAL SynthShortText;

HCI_DECL_CALLBACK(SynthCB, void, SynthShortText *, HciError, SynthResult *);
IS_HCI_PTR(is_synth_cb_, SynthCB);

#ifndef HCI_EXPLICIT_IMPL
HCI_IMPL_CALLBACK(SynthCB, void, SynthShortText *, HciError, SynthResult *);
#endif

class HCI_LOCAL SynthStream;

class HCI_LOCAL SynthHandler {
 public:
  virtual ~SynthHandler() = 0;

  /**
   * 会话启动成功或失败时被调用
   */
  virtual void OnStart(SynthStream *s, HciError code,
                       const array<Warning> *warnings) = 0;
  /**
   * 会话结束时被调用
   */
  virtual void OnEnd(SynthStream *s, EndReason reason) = 0;
  /**
   * 收到音频数据时被调用
   */
  virtual void OnAudio(SynthStream *s, const void *data, size_t size,
                       void (*free)(const void *)) = 0;
  /**
   * 发生错误时被调用
   */
  virtual void OnError(SynthStream *s, HciError code) = 0;
};

inline SynthHandler::~SynthHandler() {}

/**
 * 一次性短文本语音合成
 */
class HCI_LOCAL SynthShortText : public HciSession {
  typedef SynthShortText SELF;

 public:
  /**
   * 静态方法，创建本地 SynthShortText 能力会话
   */
  template <typename T>
  static safe_ptr<SELF> NewLocal(T &&sdk, LocalTtsConfig *config) {
    config->set_reserved1(0);
    return wrap(api::_hci_new_local_synth_shorttext(
        pass<HciSdk>(std::forward<T>(sdk)).get(), config));
  }
  /**
   * 静态方法，创建云端 SynthShortText 能力会话
   */
  template <typename T>
  static safe_ptr<SELF> NewCloud(T &&sdk, CloudTtsConfig *config) {
    config->set_reserved1(0);
    return wrap(api::_hci_new_cloud_synth_shorttext(
        pass<HciSdk>(std::forward<T>(sdk)).get(), config));
  }

 public:
  virtual void _Synthetize(const char *text, const SynthConfig *config,
                           pass_ptr<SynthCB> callback) throw() = 0;
  virtual void Cancel() throw() = 0;

  /**
   */
  template <typename T, HCI_VALID_IF(is_synth_cb_<T>::value)>
  void AsyncSynth(T &&callback, const char *text, const SynthConfig *config) {
    _Synthetize(text, config, pass<SynthCB>(std::forward<T>(callback)));
  }
#ifndef HCI_EXPLICIT_IMPL
  template <typename F, HCI_VALID_IF(func_traits<F>::is_func)>
  void AsyncSynth(F &&func, const char *text, const SynthConfig *config) {
    auto cb = SynthCB::New<F>(std::forward<F>(func));
    _Synthetize(text, config, cb.pass());
  }
  template <typename F, typename D, HCI_VALID_IF(func_traits<F>::value)>
  void AsyncSynth(F &&func, D &&data, const char *text,
                  const SynthConfig *config) {
    auto cb = SynthCB::New<F, D>(  //
        std::forward<F>(func), std::forward<D>(data));
    _Synthetize(text, config, cb.pass());
  }
  HciError Synthetize(const char *text, const SynthConfig *config,
                      SynthResult *res) {
    if (sdk()->in_loop_thread())
      hci::api::hci_abort("hci/tts.h", __LINE__, "in loop thread");
    HciEvent event;
    auto func = [&](SynthShortText *, HciError code, SynthResult *p) {
      if (code == HciError::OK) *res = *p;
      event.Set(code);
    };
    AsyncSynth(std::move(func), text, config);
    return event.Wait();
  }
#endif
};

/**
 * 流式语音合成
 */
class HCI_LOCAL SynthStream : public HciSession {
  typedef SynthStream SELF;

 public:
  /**
   * 静态方法，创建本地 SynthStream 能力会话
   */
  template <typename T>
  static safe_ptr<SELF> NewLocal(T &&sdk, LocalTtsConfig *config) {
    config->set_reserved1(0);
    return wrap(api::_hci_new_local_synth_stream(
        pass<HciSdk>(std::forward<T>(sdk)).get(), config));
  }
  /**
   * 静态方法，创建云端 SynthStream 能力会话
   */
  template <typename T>
  static safe_ptr<SELF> NewCloud(T &&sdk, CloudTtsConfig *config) {
    config->set_reserved1(0);
    return wrap(api::_hci_new_cloud_synth_stream(
        pass<HciSdk>(std::forward<T>(sdk)).get(), config));
  }

 public:
  virtual void Start(const char *text, const SynthConfig *config,
                     SynthHandler *handler, bool async) throw() = 0;
  virtual void Cancel() throw() = 0;
};

HCI_NS_END(hci, sdk, tts);

#endif  // HCI_TTS_H_
