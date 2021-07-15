
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/asr/common.h
 *
 * ASR 能力公共内容
 */
#ifndef HCI_ASR_COMMON_H_
#define HCI_ASR_COMMON_H_

#include <hci/table.h>

HCI_NS_BEGIN(hci, sdk, asr);

/**
 * 本地能力设置，用于创建本地 ASR 能力会话对象
 */
// clang-format off
HCI_DECL_TABLE(LocalAsrConfig);
  HCI_XPSTR_FIELD(const char *, model_path, true);  // [LP] 模型路径
  HCI_BLANK_FIELD(const char *);                    // [LP] 10.1.0 预留
  HCI_PLAIN_FIELD(int32_t, reserved1);              // [I4] 10.1.0 增加，内部用，检查版本兼容性
HCI_DECL_TABLE_END(LocalAsrConfig);
// clang-format on

/**
 * 云端能力设置，用于创建云端 ASR 能力会话对象
 */
// clang-format off
HCI_DECL_TABLE(CloudAsrConfig);
  HCI_PLAIN_FIELD(int32_t, reserved1);              // [I4] 10.1.0 增加，内部用，检查版本兼容性
HCI_DECL_TABLE_END(CloudAsrConfig);
// clang-format on

/**
 * 识别输出中的音频事件类型
 */
enum struct AudioEventType : uint8_t {
  NONE = 0,
  VOICE_START = 1,           // 语音开始
  VOICE_END = 2,             // 语音结束
  EXCEEDED_SILENCE = 3,      // 开头静音超长
  EXCEEDED_END_SILENCE = 4,  // 结束静音超长
  EXCEEDED_AUDIO = 5,        // 音频超长
  NOISE = 6,                 // 噪声
};

HCI_ENUM_NAME(AudioEventType, NONE, VOICE_START, VOICE_END, EXCEEDED_SILENCE,
              EXCEEDED_END_SILENCE, EXCEEDED_AUDIO, NOISE);

/**
 * 识别设置中的分词类型，用于控制分词输出
 */
enum struct HciWordType : uint8_t {
  DISABLED = 0,  // 禁用分词
  WORD = 1,      // 按词组分词
  CHAR = 2,      // 按单字分词
};

HCI_ENUM_NAME(HciWordType, DISABLED, WORD, CHAR);

/**
 * 识别输出中的分词类别，标志位类型，判断分词类别时需按位处理
 */
enum struct HciWord : uint8_t {
  N = 1,    // 正常分词
  P = 2,    // 标点
  S = 4,    // 顺滑词
  M = 8,    // 屏蔽词
  D = 16,   // 数字归一化词
  U = 128,  // 用户热词
};

template <typename TA, typename TB,
          HCI_VALID_IF(is_one_of<HciWord, TA, TB>::value)>
static inline uint8_t operator&(const TA a, const TB b) {
  return static_cast<uint8_t>(a) & static_cast<uint8_t>(b);
}

template <typename TA, typename TB,
          HCI_VALID_IF(is_one_of<HciWord, TA, TB>::value)>
static inline HciWord operator|(const TA a, const TB b) {
  auto v = static_cast<uint8_t>(a) | static_cast<uint8_t>(b);
  return static_cast<HciWord>(v);
}

static inline cstr<8> nameOf(HciWord w) {
  cstr<8> sz;
  int i = 0;

  if (w & HciWord::U) sz[i++] = 'U';
  if (w & HciWord::N) sz[i++] = 'N';
  if (w & HciWord::P) sz[i++] = 'P';
  if (w & HciWord::S) sz[i++] = 'S';
  if (w & HciWord::M) sz[i++] = 'M';
  if (w & HciWord::D) sz[i++] = 'D';
  sz[i++] = 0;
  return sz;
}

static inline void valueOf(const char *n, HciWord *w) {
  *w = static_cast<HciWord>(0);
  for (; *n; n++) {
    if (*n == 'N') *w = *w | HciWord::N;
    if (*n == 'P') *w = *w | HciWord::P;
    if (*n == 'S') *w = *w | HciWord::S;
    if (*n == 'M') *w = *w | HciWord::M;
    if (*n == 'D') *w = *w | HciWord::D;
    if (*n == 'U') *w = *w | HciWord::U;
  }
}

/**
 * 情绪类别
 */
enum struct HciEmotion : int8_t {
  HAPPY = 0,
  ANGRY = 1,
  SAD = 2,
  DISGUSTED = 3,
};

HCI_ENUM_NAME(HciEmotion, HAPPY, ANGRY, SAD, DISGUSTED);

/**
 * 性别
 */
enum struct HciGender : int8_t {
  FEMALE = 0,
  MALE = 1,
};

HCI_ENUM_NAME(HciGender, FEMALE, MALE);

/**
 * 识别结果中的分词信息
 *
 *   - 10.2.0 中进行了不兼容的字段调整
 */
// clang-format off
HCI_DECL_TABLE(HciWordInfo);
  HCI_XPSTR_FIELD(const char *, word, true);    // [LP] 分词文本
  HCI_XPSTR_FIELD(const char *, pinyin, true);  // [LP] 分词拼音
  HCI_PLAIN_FIELD(int32_t, start_time, true);   // [I4] 开始时间，单位: ms
  HCI_PLAIN_FIELD(int32_t, end_time, true);     // [I4] 结束时间，单位: ms
  HCI_PLAIN_FIELD(float, confidence, true);     // [F4] 置信度, 0 ~ 1.0
  HCI_PLAIN_FIELD(HciWord, type, true);         // [B1] 分词类别
HCI_DECL_TABLE_END(HciWordInfo);
// clang-format on

/**
 * 情绪信息
 */
// clang-format off
HCI_DECL_TABLE(HciEmotionInfo);
  HCI_PLAIN_FIELD(int32_t, start_time, true);  // [I4] 开始时间，单位: ms
  HCI_PLAIN_FIELD(int32_t, end_time, true);    // [I4] 结束时间，单位: ms
  HCI_PLAIN_FIELD(float, confidence, true);    // [F4] 置信度, 0 ~ 1.0
  HCI_PLAIN_FIELD(HciEmotion, emotion, true);  // [B1] 情绪类别
HCI_DECL_TABLE_END(HciEmotionInfo);
// clang-format on

/**
 * 性别信息
 *
 *   - 10.3.0 版本可用
 */
// clang-format off
HCI_DECL_TABLE(HciGenderInfo);
  HCI_PLAIN_FIELD(float, confidence, true);  // [F4]
  HCI_PLAIN_FIELD(HciGender, gender, true);  // [I1]
HCI_DECL_TABLE_END(HciGenderInfo);
// clang-format on

HCI_NS_END(hci, sdk, asr);

#endif  // HCI_ASR_COMMON_H_
