
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/freetalk.h
 *
 * 自由说识别
 */
#ifndef HCI_FREETALK_H_
#define HCI_FREETALK_H_

#ifndef HCI_FT_API
#define HCI_FT_API HCI_IMPORT
#endif

#include <hci/sdk.h>
#include <hci/sdk/json.h>
#include <hci/audio.h>
#include <hci/callback.h>
#include <hci/asr/common.h>

HCI_NS_BEGIN(hci, sdk, asr);

/**
 * 警告消息。10.1.0 版本可用
 */
// clang-format off
HCI_DECL_TABLE(Warning)
  HCI_XPSTR_FIELD(const char *, message, true);
  HCI_PLAIN_FIELD(int32_t, code, true);
HCI_DECL_TABLE_END(Warning);
// clang-format on

/**
 * 识别结果文本信息
 *
 *   - 10.2.0 版本进行了不兼容的字段调整
 */
// clang-format off
HCI_DECL_TABLE(FreetalkTextResult)
  HCI_XPSTR_FIELD(const char *, text, true);    // [LP] 文本
  HCI_XPSTR_FIELD(const char *, pinyin, true);  // [LP] 10.2.0 可用
  HCI_ARRAY_FIELD(HciWordInfo, words, true);    // [LP] 对应分词数组
  HCI_PLAIN_FIELD(float, confidence, true);     // [F4] 置信度, 范围: [0, 1.0]
HCI_DECL_TABLE_END(FreetalkTextResult);
// clang-format on

/**
 * 质检结果信息
 *
 *   - 10.2.0 版本可用
 */
// clang-format off
HCI_DECL_TABLE(FreetalkAnalysisResult);
  HCI_ARRAY_FIELD(HciEmotionInfo, emotions, true); // [LP] 情绪信息数组
  HCI_PLAIN_FIELD(float, speed, true);             // [F4]
  HCI_PLAIN_FIELD(float, avg_vol, true);           // [F4]
  HCI_PLAIN_FIELD(float, max_vol, true);           // [F4]
  HCI_BLANK_FIELD(int32_t);

  HCI_TABLE_FIELD(HciGenderInfo, gender);          // [LP] 10.3.0 可用
HCI_DECL_TABLE_END(FreetalkAnalysisResult);
// clang-format on

/**
 * 识别结果
 *
 *   - 10.2.0 版本进行了不兼容的字段调整
 *   - analysis 字段仅在 FreetalkStream 识别结果中使用
 */
// clang-format off
HCI_DECL_TABLE(FreetalkResult)
  HCI_PLAIN_FIELD(int32_t, start_time, true);     // [I4] 开始时间，单位: ms
  HCI_PLAIN_FIELD(int32_t, end_time, true);       // [I4] 结束时间，单位: ms
  HCI_TABLE_FIELD(FreetalkTextResult, result, true);        // [LP] 首选结果
  HCI_ARRAY_FIELD(FreetalkTextResult, alternatives, true);  // [LP] 候选结果
  HCI_TABLE_FIELD(FreetalkAnalysisResult, analysis, true);  // [LP] 10.2.0 可用
  HCI_PLAIN_FIELD(JsonValue, extend_result, true);          // [LP] 10.2.0 可用
  HCI_PLAIN_FIELD(bool, is_final, true);              // [B1] 是否为最终结果
HCI_DECL_TABLE_END(FreetalkResult);
// clang-format on

/**
 * 音频事件
 */
// clang-format off
HCI_DECL_TABLE(FreetalkEvent)
  HCI_PLAIN_FIELD(int32_t, timestamp, true);    // [I4] 音频时间偏移，单位: ms
  HCI_PLAIN_FIELD(AudioEventType, type, true);  // [B1] 音频事件类型
HCI_DECL_TABLE_END(FreetalkEvent);
// clang-format on

/**
 * 识别模式
 */
enum struct FreetalkMode : uint8_t {
  SHORT_AUDIO = 0,       // short_audio，一句话识别
  SHORT_STREAM = 1,      // short_stream，流式一句话识别
  UTTERANCE_STREAM = 2,  // utterance_stream，实时转写首句模式
  CONTINUE_STREAM = 3,   // continue_stream，实时转写连续模式
  UTTERANCE_DIALOG = 10,  // utterance_dialog, 实时转写首句模式 + NLU 语义处理
};

HCI_ENUM(FreetalkMode, uint8_t);

HCI_ENUM_NAME(FreetalkMode, SHORT_AUDIO, SHORT_STREAM, UTTERANCE_STREAM,
              CONTINUE_STREAM, UTTERANCE_DIALOG);

/**
 * 质检参数设置
 *
 *   - 10.2.0 版本可用
 */
// clang-format off
HCI_DECL_TABLE(FreetalkSaConfig);
  HCI_PLAIN_FIELD(bool, check_emotion, true);   // [B1]
  HCI_PLAIN_FIELD(bool, output_speed, true);    // [B1]
  HCI_PLAIN_FIELD(bool, output_volume, true);   // [B1]
  HCI_PLAIN_FIELD(bool, check_gender);          // [B1]  10.3.0 可用
HCI_DECL_TABLE_END(FreetalkSaConfig);
// clang-format on

/**
 * FreetalkShortAudio 能力会话识别设置
 *
 *   - 10.2.0 版本进行不兼容的字段调整
 */
// clang-format off
HCI_DECL_TABLE(ShortAudioConfig)
  HCI_XPSTR_FIELD(const char *, access_token);        // [LP] 10.2.0 可用
  HCI_XPSTR_FIELD(const char *, property, true);      // [LP]
  HCI_XPSTR_FIELD(const char *, audio_format, true);  // [LP]
  HCI_XPSTR_FIELD(const char *, extra_info, true);    // [LP]
  HCI_XPSTR_FIELD(const char *, record_id, true);     // [LP] 10.2.0 可用
  HCI_XPSTR_FIELD(const char *, vocab, true);         // [LP] 10.1.0 可用
  HCI_XPSTR_FIELD(const char *, vocab_id, true);      // [LP] 10.1.0 可用

  HCI_PLAIN_FIELD(FreetalkMode, mode, true);          // [B1]
  HCI_PLAIN_FIELD(HciWordType, word_type, true);      // [B1]
  HCI_PLAIN_FIELD(bool, output_pinyin, true);         // [B1] 10.2.0 可用
  HCI_PLAIN_FIELD(bool, add_punc, true);              // [B1]

  HCI_PLAIN_FIELD(bool, digit_norm, true);            // [B1] 10.2.0 可用
  HCI_PLAIN_FIELD(bool, text_smooth, true);           // [B1] 10.2.0 可用
  HCI_PLAIN_FIELD(bool, word_filter, true);           // [B1] 10.2.0 可用
  HCI_PLAIN_FIELD(bool, word_tpp, true);              // [B1] 10.2.0 可用

  HCI_PLAIN_FIELD(int32_t, nbest, true);              // [I4]
  HCI_PLAIN_FIELD(int32_t, timeout, true);            // [I4]

  HCI_XPSTR_FIELD(const char *, sensword);            // [LP] 10.3.0 可用
  HCI_XPSTR_FIELD(const char *, sensword_id);         // [LP] 10.3.0 可用

  HCI_TABLE_FIELD(FreetalkSaConfig, sa);              // [LP] 10.3.0 可用

HCI_DECL_TABLE_END(ShortAudioConfig);
// clang-format on

/**
 * FreetalkShortAudio 识别异步回调接口，当识别完成或中止时被调用
 *
 * 允许多个会话使用同一个回调接口，可通过第一个参数区分不同的会话。
 *
 * 原型如下:
 *
 * @code
 *
 * class HCI_LOCAL ShortAudioCB: public HciObject {
 *  public:
 *   virtual void Invoke(FreetalkShortAudio *s, HciError code,
 *       FreetalkResult *result, const array<Warning> *warnings) = 0;
 *
 *   template<typename F>
 *   static safe_ptr<ShortAudioCB> New(F &&func);
 *
 *   template<typename F, typename D>
 *   static safe_ptr<ShortAudioCB> New(F &&func, D &&data);
 * };
 *
 * @endcode
 */
HCI_DECL_CALLBACK(ShortAudioCB, void, FreetalkShortAudio *, HciError,
                  FreetalkResult *, const array<Warning> *);
IS_HCI_PTR(is_short_audio_cb_, ShortAudioCB);

#ifndef HCI_EXPLICIT_IMPL
HCI_IMPL_CALLBACK(ShortAudioCB, void, FreetalkShortAudio *, HciError,
                  FreetalkResult *, const array<Warning> *);
#endif

/**
 * FreetalkShortAudio 能力会话
 */
class HCI_LOCAL FreetalkShortAudio : public HciSession {
  HCI_INTERFACE(FreetalkShortAudio);

  typedef FreetalkShortAudio SELF;

 public:
  /**
   * 静态方法，创建本地 FreetalkShortAudio 能力会话
   */
  template <typename T>
  static safe_ptr<SELF> NewLocal(T &&sdk, LocalAsrConfig *config) {
    config->set_reserved1(2);  // 10.2.0 设置为 2
    return wrap(api::_hci_new_local_ft_shortaudio(
        pass<HciSdk>(std::forward<T>(sdk)).get(), config));
  }
  /**
   * 静态方法，创建云端 FreetalkShortAudio 能力会话
   */
  template <typename T>
  static safe_ptr<SELF> NewCloud(T &&sdk, CloudAsrConfig *config) {
    config->set_reserved1(2);  // 10.2.0 设置为 2
    return wrap(api::_hci_new_cloud_ft_shortaudio(
        pass<HciSdk>(std::forward<T>(sdk)).get(), config));
  }

 public:
  /**
   * 识别语音数据，异步操作
   */
  virtual void _Recognize(const void *audio_data, size_t audio_length,
                          const ShortAudioConfig *config,
                          pass_ptr<ShortAudioCB> callback) = 0;
  /**
   * 取消识别，异步操作
   */
  virtual void Cancel() = 0;

  /**
   * 识别语音数据，异步操作
   */
  template <typename T, HCI_VALID_IF(is_short_audio_cb_<T>::value)>
  void AsyncRecognize(T &&callback, const void *audio_data, size_t audio_length,
                      const ShortAudioConfig *config) {
    _Recognize(audio_data, audio_length, config,
               pass<ShortAudioCB>(std::forward<T>(callback)));
  }

#ifndef HCI_EXPLICIT_IMPL
  /**
   * 识别语音数据，异步操作
   */
  template <typename F, HCI_VALID_IF(func_traits<F>::is_func)>
  void AsyncRecognize(F &&func, const void *audio_data, size_t audio_length,
                      const ShortAudioConfig *config) {
    auto cb = ShortAudioCB::New<F>(std::forward<F>(func));
    _Recognize(audio_data, audio_length, config, cb.pass());
  }

  /**
   * 识别语音数据，异步操作
   */
  template <typename F, typename D, HCI_VALID_IF(func_traits<F>::value)>
  void AsyncRecognize(F &&func, D &&data, const void *audio_data,
                      size_t audio_length, const ShortAudioConfig *config) {
    auto cb = ShortAudioCB::New<F, D>(  //
        std::forward<F>(func), std::forward<D>(data));
    _Recognize(audio_data, audio_length, config, cb.pass());
  }

  /**
   * 识别语音数据，同步操作
   */
  HciError Recognize(const void *audio_data, size_t audio_length,
                     const ShortAudioConfig *config, FreetalkResult *result) {
    if (sdk()->in_loop_thread())
      hci::api::hci_abort("hci/freetalk.h", __LINE__, "in loop thread");
    HciEvent event;
    auto func = [&](FreetalkShortAudio *, HciError code, FreetalkResult *res,
                    const array<Warning> *) {
      if (res != nullptr)  // 保存结果
        *result = *res;
      event.Set(code);
    };
    AsyncRecognize(std::move(func), audio_data, audio_length, config);
    return event.Wait();
  }
#endif
};

HCI_INTERFACE_(FreetalkShortAudio);

/**
 * FreetalkStream 能力会话识别设置
 */
// clang-format off
HCI_DECL_TABLE(FreetalkConfig);
  HCI_XPSTR_FIELD(const char *, access_token, true);    // [LP] 10.2.0 可用
  HCI_XPSTR_FIELD(const char *, property, true);        // [LP]
  HCI_XPSTR_FIELD(const char *, audio_format, true);    // [LP]
  HCI_XPSTR_FIELD(const char *, extra_info, true);      // [LP]
  HCI_XPSTR_FIELD(const char *, record_id, true);       // [LP] 10.2.0 可用
  HCI_XPSTR_FIELD(const char *, vocab, true);           // [LP] 10.1.0 可用
  HCI_XPSTR_FIELD(const char *, vocab_id, true);        // [LP] 10.1.0 可用
  HCI_XPSTR_FIELD(const char *, encode_options, true);  // [LP] 10.1.0 可用
  HCI_TABLE_FIELD(FreetalkSaConfig, sa, true);          // [LP] 10.2.0 可用
  HCI_PLAIN_FIELD(JsonValue, extend_config, true);      // [LP] 10.2.0 可用

  HCI_PLAIN_FIELD(FreetalkMode, mode, true);            // [B1]
  HCI_PLAIN_FIELD(HciWordType, word_type, true);        // [B1]
  HCI_PLAIN_FIELD(bool, interim_results, true);         // [B1]
  HCI_PLAIN_FIELD(bool, output_pinyin, true);           // [B1] 10.2.0 可用

  HCI_PLAIN_FIELD(bool, add_punc, true);                // [B1]
  HCI_PLAIN_FIELD(bool, digit_norm, true);              // [B1] 10.2.0 可用
  HCI_PLAIN_FIELD(bool, text_smooth, true);             // [B1] 10.2.0 可用
  HCI_PLAIN_FIELD(bool, word_filter, true);             // [B1] 10.2.0 可用

  HCI_PLAIN_FIELD(bool, word_tpp, true);                // [B1] 10.2.0 可用
  HCI_PLAIN_FIELD(bool, make_paragraph, true);          // [B1] 10.2.0 可用
  HCI_BLANK_FIELD(bool);
  HCI_BLANK_FIELD(bool);

  HCI_PLAIN_FIELD(int32_t, nbest, true);                // [I4]

  HCI_PLAIN_FIELD(int32_t, vad_head, true);             // [I4]
  HCI_PLAIN_FIELD(int32_t, vad_tail, true);             // [I4]
  HCI_PLAIN_FIELD(int32_t, vad_end, true);              // [I4] 10.2.0 可用
  HCI_PLAIN_FIELD(int32_t, vad_max_segment, true);      // [I4]
  HCI_PLAIN_FIELD(int32_t, vad_threshold, true);        // [I4] 10.1.0 可用
  HCI_PLAIN_FIELD(int32_t, start_offset, true);         // [I4]
  HCI_PLAIN_FIELD(int32_t, slice, true);                // [I4]
  HCI_PLAIN_FIELD(int32_t, dps, true);                  // [I4]
  HCI_PLAIN_FIELD(int32_t, timeout, true);              // [I4]
  HCI_PLAIN_FIELD(int32_t, tpp_context_range);          // [I4] 10.4.0 可用

  HCI_XPSTR_FIELD(const char *, sensword_id);           // [LP] 10.3.0 可用
  HCI_XPSTR_FIELD(const char *, sensword);              // [LP] 10.3.0 可用
HCI_DECL_TABLE_END(FreetalkConfig);
// clang-format on

/**
 * 识别结束原因
 */
enum struct EndReason : uint8_t {
  NORMAL = 0,    // 正常结束
  CANCEL = 1,    // 用户取消
  ABNORMAL = 2,  // 发生错误
};
HCI_ENUM_NAME(EndReason, NORMAL, CANCEL, ABNORMAL);

/**
 * FreetalkStream 能力会话回调接口
 */
class HCI_LOCAL FreetalkHandler {
 public:
  virtual ~FreetalkHandler() = 0;

  /**
   * 会话启动成功或失败时被调用
   */
  virtual void OnStart(FreetalkStream *s, HciError code,
                       const array<Warning> *warnings) = 0;
  /**
   * 会话结束时被调用
   */
  virtual void OnEnd(FreetalkStream *s, EndReason reason) = 0;
  /**
   * 输出音频事件时被调用
   */
  virtual void OnEvent(FreetalkStream *s, const FreetalkEvent *event) = 0;
  /**
   * 输出识别结果时被调用
   */
  virtual void OnResult(FreetalkStream *s, const FreetalkResult *sentence) = 0;
  /**
   * 发生错误时被调用
   */
  virtual void OnError(FreetalkStream *s, HciError code) = 0;
};

inline FreetalkHandler::~FreetalkHandler() = default;

/**
 * FreetalkStream 能力会话
 */
class HCI_LOCAL FreetalkStream : public HciSession {
  HCI_INTERFACE(FreetalkStream);

  typedef FreetalkStream SELF;

 public:
  /**
   * 静态方法，创建本地 FreetalkStream 能力会话
   */
  template <typename T, HCI_VALID_IF(is_sdk_<T>::value)>
  static safe_ptr<SELF> NewLocal(T &&sdk, LocalAsrConfig *config) {
    config->set_reserved1(2);  // 10.2.0 设置为 2
    return wrap(api::_hci_new_local_ft_stream(
        pass<HciSdk>(std::forward<T>(sdk)).get(), config));
  }
  /**
   * 静态方法，创建云端 FreetalkStream 能力会话
   */
  template <typename T, HCI_VALID_IF(is_sdk_<T>::value)>
  static safe_ptr<SELF> NewCloud(T &&sdk, CloudAsrConfig *config) {
    config->set_reserved1(2);  // 10.2.0 设置为 2
    return wrap(api::_hci_new_cloud_ft_stream(
        pass<HciSdk>(std::forward<T>(sdk)).get(), config));
  };

 public:
  /**
   * 启动音频识别
   */
  virtual void Start(FreetalkConfig *config, HciAudioSource *source,
                     FreetalkHandler *handler, bool async = true) = 0;

  /**
   * 停止语音识别，异步操作
   */
  virtual void Stop(bool cancel) = 0;
};
HCI_INTERFACE_(FreetalkStream);

HCI_NS_END(hci, sdk, asr);

#endif  // HCI_FREETALK_H_
