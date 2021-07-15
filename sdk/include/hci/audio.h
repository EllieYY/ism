
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/audio.h
 *
 * 音频源，音频槽接口
 */
#ifndef HCI_AUDIO_H_
#define HCI_AUDIO_H_

#include <hci/callback.h>
#include <hci/table.h>

#include <utility>

namespace hci {

/**
 * 音频错误码
 */
enum struct HciAudioError : int {
  OK = 0,

  INVALID_CALL = -1,     // 非法调用
  INVALID_PARAM = -2,    // 参数错误
  METRICS_UNMATCH = -3,  // 音频设置不匹配
  UNSUPPORTED = -4,      // 无法支持的功能

  END_NORMAL = -5,  // 音频流已结束，应继续处理仍在处理中和未处理的音频
  END_CANCEL = -6,  // 音频流已结束，应放弃处理仍在处理中和未处理的音频

  VOICE_START = -7,  // 语音开始事件 (静音结束)
  VOICE_END = -8,    // 语音结束事件 (静音开始)

  END_SINK = -9,  // 无法写入，用户接下来应调用 HciAudioSink::EndSink
  FAILURE = -10,  // 操作失败
};

HCI_ENUM(HciAudioError, int);

enum struct HciSampleFormat : uint8_t {
  U8,     ///< 无符号 8 位整形 PCM
  S16LE,  ///< 小端有符号 16 位整形 PCM
  S16BE,  ///< 大端有符号 16 位整形 PCM
  F32LE,  ///< 小端 32 位 IEEE 浮点 PCM
  F32BE,  ///< 大端 32 位 IEEE 浮点 PCM
  ALAW,   ///< 8 位 a-Law
  ULAW,   ///< 8 位 mu-Law
  S32LE,  ///< 小端有符号 32 位整形 PCM
  S32BE,  ///< 大端有符号 32 位整形 PCM
  S24LE,  ///< 小端有符号 24 位整形 PCM
  S24BE,  ///< 大端有符号 24 位整形 PCM
};

HCI_ENUM(HciSampleFormat, uint8_t);

/**
 * @class HciAudioMetrics
 *
 * 音频参数设置
 */
// clang-format off
HCI_DECL_TABLE(HciAudioMetrics)
  HCI_PLAIN_FIELD(bool, have_vad, true);  // 音频流中是否包含 VAD 事件
  HCI_PLAIN_FIELD(bool, loop_write);      // 写满后丢弃旧数据保证写操作可以完成
  HCI_PLAIN_FIELD(bool, fine_read);       // 按 sample 读取，而不是按 frame 读取
  HCI_BLANK_FIELD(bool);   // 扩展用
  HCI_PLAIN_FIELD(HciSampleFormat, format, true);
  HCI_PLAIN_FIELD(int32_t, channels, true);
  HCI_PLAIN_FIELD(int32_t, sample_rate, true);
  HCI_PLAIN_FIELD(int32_t, frame_time, true);

  void Validate() const;
  int CalcDataLenByTimeLen(int ms) const;

  int sample_size() const;
  int frame_size() const;
  int min_frame_time() const;

  bool operator==(const HciAudioMetrics &m) const {
    return format() == m.format() &&
      channels() == m.channels() &&
      sample_rate() == m.sample_rate() &&
      frame_time() == m.frame_time();
  }
  template<int N>
  const char *dump(char (&sz)[N]) {
    return hci_dump_audio_metrics(this, sz, N);
  }
HCI_DECL_TABLE_END(HciAudioMetrics);
// clang-format on

/**
 * @class HciAudioCB
 *
 * HciAudioCB 回调接口，用于音频源接口的异步读取和音频槽接口的异步写入，详见
 * HciAudioSource::AsyncRead 和 HciAudioSink::AsyncWrite
 *
 * 原型如下:
 *
 * @code
 *
 * class HCI_LOCAL HciAudioCB : public HciObject {
 *  public:
 *   virtual void Invoke(int retval) = 0;
 *
 *   template<typename F>
 *   static safe_ptr<HciAudioCB> New(F &&func);
 *   template<typename F, typename D>
 *   static safe_ptr<HciAudioCB> New(F &&func, D &&data);
 * };
 *
 * @endcode
 */
HCI_DECL_CALLBACK(HciAudioCB, void, int);

IS_HCI_PTR(is_audio_cb_, HciAudioCB);

#ifndef HCI_EXPLICIT_IMPL
HCI_IMPL_CALLBACK(HciAudioCB, void, int);
#endif

/**
 * @class HciAudioSource audio.h hci/audio.h
 *
 * 音频数据源接口
 *
 * 拉取模式下使用本接口。本接口的方法是不可重入的，任何一个时刻下本接口正在被
 * 调用的函数最多只允许有一个。因此请避免在多个线程中使用本接口，如果必须在多
 * 个线程中使用本接口，请确保对接口的调用是串行且不重叠的。
 *
 *   - 实现者(被驱动方)通过本接口来提供音频数据。
 *   - 使用者(驱动方)通过本接口来获取音频数据。
 */
class HCI_LOCAL HciAudioSource : public HciObject {
  HCI_INTERFACE(HciAudioSource);

 public:
  /**
   * 开启读操作后，获取当前的音频参数设置
   */
  virtual void GetMetrics(HciAudioMetrics *metrics) const = 0;
  /**
   * 获取默认音频参数设置，一般在开启写操作前调用
   */
  virtual void GetDefaultMetrics(HciAudioMetrics *metrics) const = 0;

  /**
   * 开始读操作
   *
   *   @param [in,out] metrics 音频参数设置
   *   @return 音频错误码
   *
   *   - HciAudioError::METRICS_UNMATCH  音频格式不匹配
   *   - HciAudioError::UNSUPPORTED      不支持的音频参数设置
   *   - HciAudioError::INVALID_CALL     非法调用
   *   - HciAudioError::INVALID_PARAM    非法输入
   *
   * 音频参数设置
   *
   *   - have_vad      输出参数
   *   - format        可选参数
   *   - channels      可选参数
   *   - sample_rate   可选参数
   *   - frame_time    可选参数
   */
  virtual HciAudioError StartRead(HciAudioMetrics *metrics) = 0;

  /**
   * 结束读取
   *
   *   @return 音频错误码
   *
   *   - HciAudioError::INVALID_CALL 非法调用，非启动状态下调用返回此错误码
   *   - HciAudioError::OK 操作成功
   *
   * 本操作有如下作用
   *
   *   - 其他线程中正在阻塞的同步读取将立即返回 HciAudioError::END_CANCEL
   *   - 未完成的异步读取将立即以 HciAudioError::END_CANCEL 错误码完成
   *   - 进入未启动状态，后续读取操作均会发生 HciAudioError::INVALID_CALL 错误
   */
  virtual HciAudioError EndRead() = 0;

  /**
   * 音频流当前时间戳
   */
  virtual int Timestamp() const = 0;

  /**
   * 读取音频数据。贪婪式读取，尽可能多的读取数据。
   *
   * fine_read 为 true 时，length 应为 sample_size 整数倍；
   * 否则，应为 frame_size 整数倍。
   *
   *   @param [out] data      音频数据的内存地址
   *   @param [in]  length    音频数据内存的长度
   *   @param [in]  non_block 非阻塞式读取
   *   @return 成功读取数据的长试或音频错误码
   *
   *   - **负值** 表示操作失败，其值为错误码，请参考 HciAudioError
   *   - **零值** 非阻塞操作下为当前无可读数据和可读事件；阻塞模式不会返回此值
   *   - **正值** 成功读取的音频数据长度
   */
  virtual int Read(void *data, size_t length, bool non_block = false) = 0;

  /**
   * 异步读取，贪婪式读取，完成时调用指定的回调函数
   *
   * 异步操作完成前，需要保证 data 指向的内存可用，且不能再调用 Read,
   * AsyncRead 和 EndRead 操作。回调入参与 Read 的返值等价
   */
  virtual void _AsyncRead(void *data, size_t length,
                          pass_ptr<HciAudioCB> callback) = 0;

  template <typename T, HCI_VALID_IF(is_audio_cb_<T>::value)>
  void AsyncRead(T &&callback, void *data, size_t length) {
    _AsyncRead(data, length, pass<HciAudioCB>(std::forward<T>(callback)));
  }
#ifndef HCI_EXPLICIT_IMPL
  template <typename F, HCI_VALID_IF(func_traits<F>::is_func)>
  void AsyncRead(F &&func, void *data, size_t length) {
    auto cb = HciAudioCB::New<F>(std::forward<F>(func));
    _AsyncRead(data, length, cb.pass());
  }
  template <typename F, typename D, HCI_VALID_IF(func_traits<F>::value)>
  void AsyncRead(F &&f, D &&d, void *data, size_t length) {
    auto cb = HciAudioCB::New<F, D>(std::forward<F>(f), std::forward<D>(d));
    _AsyncRead(data, length, cb.pass());
  }
#endif
};

HCI_INTERFACE_(HciAudioSource);
IS_HCI_PTR(is_audio_source_, HciAudioSource);

/**
 * @class HciAudioSink audio.h hci/audio.h
 *
 * 音频数据槽接口
 *
 * 推送模式下使用本接口。本接口的方法是不可重入的，任何一个时刻下本接口正在被
 * 调用的函数最多只允许有一个。因此请避免在多个线程中使用本接口，如果必须在多
 * 个线程中使用本接口，请确保对接口的调用是串行且不重叠的。
 *
 *   - 实现者(被驱动方)通过本接口来接收音频数据。
 *   - 使用者(驱动方)通过本接口来提供音频数据。
 */
class HCI_LOCAL HciAudioSink : public HciObject {
  HCI_INTERFACE(HciAudioSink);

 public:
  /**
   * 开启写操作后，获取当前的音频参数设置
   */
  virtual void GetMetrics(HciAudioMetrics *metrics) const = 0;
  /**
   * 获取默认音频参数设置，一般在开启写操作前调用
   */
  virtual void GetDefaultMetrics(HciAudioMetrics *metrics) const = 0;

  /**
   * 开启写操作
   *
   *   @param [in,out] metrics  音频参数设置
   *   @return 音频错误码
   *
   *   - HciAudioError::METRICS_UNMATCH  音频格式不匹配
   *   - HciAudioError::UNSUPPORTED      不支持的音频参数设置
   *   - HciAudioError::INVALID_CALL     非法调用
   *   - HciAudioError::INVALID_PARAM    非法输入
   *
   * 音频参数设置
   *
   *   - have_vad      可选参数，默认为 false
   *   - loop_write    可选参数，写满时丢弃旧数据保证写操作能马上完成，默认为
   *                   false。与 have_vad 冲突
   *   - format        可选参数，不设置时接口实现者自由选择
   *   - channels      可选参数，不设置时接口实现者自由选择
   *   - sample_rate   可选参数，不设置时接口实现者自由选择
   *   - frame_time    可选参数，不设置时接口实现者自由选择
   */
  virtual HciAudioError StartWrite(HciAudioMetrics *metrics) = 0;

  /**
   * 结束写入
   *
   *   @param [in] cancel 是否取消
   *   @return 音频错误码
   *
   *   - HciAudioError::INVALID_CALL 非法调用，非启动状态下调用返回此错误码
   *   - HciAudioError::OK 操作成功
   *
   * 数据接收者根据 cancel 参数决定是否继续处理在处理中和未处理的音频数据
   *
   * 本操作有如下作用
   *
   *   - 其他线程中正在阻塞的同步写入将立即返回 HciAudioError::END_SINK
   *   - 未完成的异步写入将立即以 HciAudioError::END_SINK 错误码完成
   *   - 进入未启动状态，后续写入操作均会发生 HciAudioError::INVALID_CALL 错误
   */
  virtual HciAudioError EndWrite(bool cancel) = 0;

  /**
   * 写入数据
   *
   *   @param [in] data       数据内存地址
   *   @param [in] length     数据长度 (采样长度的整数倍)
   *   @param [in] non_block  非阻塞式写入
   *   @return 写操作结果或者音频错误码
   *
   * 若支持端点检测，不能将语音和静音数据混合写入；在语音和静音数据的边界处需
   * 要调用* VoiceStart 或 VoiceEnd，且第一次 Write 前必须调用过 VoiceStart
   * 或者 VoiceEnd
   *
   * 写入数据的长度必须是音频参数设置中采样长度的倍数
   *
   * 返值含义:
   *
   *   - **负值** 表示操作失败，其值为 HciAudioError 错误码对应的数值
   *   - **零值** 非阻塞操作，当前不可写，一般是因为内部缓冲已满
   *   - **正值** 写入的音频数据长度。实现者应尽可能多的写入，非强制性要求
   */
  virtual int Write(const void *data, size_t length,
                    bool non_block = false) = 0;

  /**
   * 异步写入，完成后调用指定的回调函数
   *
   * 异步操作完成前，需要保证 data 指向的内存可用，且不能调用 VoiceStart,
   * VoiceEnd, Write, AsyncWrite 和 EndWrite。回调入参与 Write 的返值等价
   */
  virtual void _AsyncWrite(const void *data, size_t length,
                           pass_ptr<HciAudioCB> callback) = 0;

  /**
   * 写入 VOICE_START 事件 (静音结束)
   *
   *   @return 音频错误码
   *
   * 要求：相邻的两个 VAD 事件不能相同，两个 VAD 事件间必须有写入音频数据
   */
  virtual HciAudioError VoiceStart() = 0;
  /**
   * 写入 VOICE_END 事件 (静音结束)
   *
   *   @return 音频错误码
   *
   * 要求：相邻的两个 VAD 事件不能相同，两个 VAD 事件间必须有写入音频数据
   */
  virtual HciAudioError VoiceEnd() = 0;

  template <typename T, HCI_VALID_IF(is_audio_cb_<T>::value)>
  void AsyncWrite(T &&callback, const void *data, size_t length) {
    _AsyncWrite(data, length, pass<HciAudioCB>(std::forward<T>(callback)));
  }
#ifndef HCI_EXPLICIT_IMPL
  template <typename F, HCI_VALID_IF(func_traits<F>::is_func)>
  void AsyncWrite(F &&func, const void *data, size_t length) {
    auto cb = HciAudioCB::New<F>(std::forward<F>(func));
    _AsyncWrite(data, length, cb.pass());
  }
  template <typename F, typename D, HCI_VALID_IF(func_traits<F>::value)>
  void AsyncWrite(F &&f, D &&d, const void *data, size_t length) {
    auto cb = HciAudioCB::New<F, D>(std::forward<F>(f), std::forward<D>(d));
    _AsyncWrite(data, length, cb.pass());
  }
#endif
};

HCI_INTERFACE_(HciAudioSink);
IS_HCI_PTR(is_audio_sink_, HciAudioSink);

/**
 * @class HciAudioBuffer audio.h hci/audio.h
 *
 * 音频数据缓冲区
 *
 * 对外提供音频源接口(被驱动方)和音频槽接口(被驱动方)，建议在不同的线程上使用
 * 源接口和槽接口，否则使用不当时会发生死锁。
 *
 * 源接口的 StartRead 必须等待用户调用槽接口的 StartWrite，槽接口的 StartWrite
 * 无须等待用户调用 StartRead
 */
class HCI_LOCAL HciAudioBuffer : public HciAudioSource {
  HCI_INTERFACE(HciAudioBuffer);

 public:
  /**
   * 获得音频缓冲区的音频槽端接口用于音频数据写入
   *
   * 返回对象不传递额外的引用计数，若无引用计数操作，其生命周期与
   * HciAudioBuffer 相同。
   */
  virtual HciAudioSink *GetAudioSink() const = 0;

  virtual int GetBufferDataLen() const = 0;
  virtual int GetBufferTimeLen() const = 0;

  /**
   * 关闭缓冲区。
   *
   * 关闭后，源接口和槽接口将无法继续正常使用
   */
  virtual void Close() = 0;

  /**
   * 获取写满次数，启用 loop_write 后可用。
   */
  virtual int OverRunCount() const = 0;

  static safe_ptr<HciAudioBuffer> New(const HciAudioMetrics *metrics,
                                      int buffer_time) {
    return wrap(api::_hci_new_audio_buffer(metrics, buffer_time)).safe();
  }
};

HCI_INTERFACE_(HciAudioBuffer);

/**
 * 创建一个基于内存数据的音频源
 *
 * 音频源参数设置中不需要设置 have_vad 字段，其他字段均应给出
 */
static inline safe_ptr<HciAudioSource> NewDataAudioSource(
    const HciAudioMetrics *metrics, const void *data, size_t len, bool copy) {
  auto obj = api::_hci_new_data_audio_source(metrics, data, len, copy);
  return wrap(obj).safe();
}

/**
 * 创建一个基于文件数据的音频源
 *
 *   @param [in]  metrics   音频源参数设置
 *   @param [in]  path      文件路径
 *   @param [in]  off       位置偏移量，从此处位置开始读取数据
 *   @param [in]  len       数据长度，负值表示一直读取至文件末尾
 *
 * 音频源参数设置中不需要设置 have_vad 字段，其他字段均应给出
 *
 * 在 Windows 平台下，文件路径 path 使用的是 ANSI 代码页，当路径中包含 ANSI 代
 * 码页之外的字符时，用户应改用 NewFileAudioSourceUTF8，并给出 UTF-8 文件路径。
 *
 * 非 Windows 平台下，NewFileAudioSource 和 NewFileAudioSourceUTF8 无区别
 */
static inline safe_ptr<HciAudioSource> NewFileAudioSource(
    const HciAudioMetrics *metrics, const char *path, int off = 0,
    int len = -1) {
  auto obj = api::_hci_new_file_audio_source(metrics, path, off, len);
  return wrap(obj).safe();
}

/**
 * 创建一个基于文件数据的音频源
 *
 *   @param [in]  metrics   音频源参数设置
 *   @param [in]  path      文件路径
 *   @param [in]  off       位置偏移量，从此处位置开始读取数据
 *   @param [in]  len       数据长度，负值表示一直读取至文件末尾
 *
 * 音频源参数设置中不需要设置 have_vad 字段，其他字段均应给出
 *
 * 文件路径 path 应使用 UTF-8 编码，非 Windows 平台下，NewFileAudioSource
 * 和 NewFileAudioSourceUTF8 无区别
 */
static inline safe_ptr<HciAudioSource> NewFileAudioSourceUTF8(
    const HciAudioMetrics *metrics, const char *path, int off = 0,
    int len = -1) {
  auto obj = api::_hci_new_file_audio_source_utf8(metrics, path, off, len);
  return wrap(obj).safe();
}

/**
 * 音频源重采样
 *
 *   @param [in]  source 输入音频源
 *   @return 生成的新音频源
 *
 * 一个音频源的声道数量和采样率可能与音频源使用者期望的声道数量和采样率不匹
 * 配，使用本方法可生成一个新的音频源，它将根据使用者的期望对输入音频源进行
 * 混音和重采样以匹配使用者的需求
 *
 *   - 采样格式仅支持 S16LE
 *   - 声道数量仅支持单声道和双声道
 *   - 采样率仅支持 8000, 11025, 16000, 22050, 32000, 48000, 44100, 96000,
 *     192000
 *
 * 注意:
 *
 *   - 重采样操作会引入若干个采样的延时
 *   - 重采样缓冲管理在输入和输出上是完全匹配的，也就是说重采样前后音频数量的
 *     绝对时长是相等的
 *   - 因此，重采样后在音频末尾会丢失若干个采样，具体数量由延时长短决定
 *   - 延时时长非常短，在实际应用场景中可忽略
 */
template <typename T, HCI_VALID_IF(is_audio_source_<T>::value)>
static inline safe_ptr<HciAudioSource> Resample(T &&source) {
  auto obj = api::_hci_resample_audio_source(
      pass<HciAudioSource>(std ::forward<T>(source)).get());
  return wrap(obj).safe();
}

/**
 * 音频槽重采样
 *
 *   @param [in]  sink 输入音频槽
 *   @return 生成的新音频槽
 *
 * 一个音频槽的声道数量和采样率可能与音频槽使用者期望的声道数量和采样率不匹
 * 配，使用本方法可生成一个新的音频槽，它将根据使用者的期望对输入音频槽进行
 * 混音和重采样以匹配使用者的需求
 *
 *   - 采样格式仅支持 S16LE
 *   - 声道数量仅支持单声道和双声道
 *   - 采样率目前仅支持 8000, 11025, 16000, 22050, 32000, 48000, 44100, 96000,
 *     192000
 *
 * 注意:
 *
 *   - 重采样操作会引入若干个采样的延时
 *   - 重采样缓冲管理在输入和输出上是完全匹配的，也就是说重采样前后音频数量的
 *     绝对时长是相等的
 *   - 因此，重采样后在音频末尾会丢失若干个采样，具体数量由延时长短决定
 *   - 延时时长非常短，在实际应用场景中可忽略
 */
template <typename T, HCI_VALID_IF(is_audio_sink_<T>::value)>
static inline safe_ptr<HciAudioSink> Resample(T &&sink) {
  auto obj = api::_hci_resample_audio_sink(
      pass<HciAudioSink>(std ::forward<T>(sink)).get());
  return wrap(obj).safe();
}

/**
 * 音频源采样格式转换
 *
 *   @param [in]  source 输入音频源
 *   @return 生成的新音频源
 *
 * 一个音频源的采样格式可能与音频源使用者期望的采样格式不匹配，使用本方法可
 * 生成一个新的音频源，它将根据使用者的期望对输入音频源进行采样格式转换，以
 * 匹配使用者的需求
 *
 * 仅支持任意采样格式转换为 S16LE。输入音频源为任意采样格式，生成的新音频源
 * 为 S16LE
 */
template <typename T, HCI_VALID_IF(is_audio_source_<T>::value)>
static inline safe_ptr<HciAudioSource> Convert(T &&source) {
  auto obj = api::_hci_convert_audio_source(
      pass<HciAudioSource>(std ::forward<T>(source)).get());
  return wrap(obj).safe();
}

/**
 * 音频槽采样格式转换
 *
 *   @param [in]  sink 输入音频槽
 *   @return 生成的新音频槽
 *
 * 一个音频槽的采样格式可能与音频槽使用者期望的采样格式不匹配，使用本方法可
 * 生成一个新的音频槽，它将根据使用者的期望对输入音频槽进行采样格式转换，以
 * 匹配使用者的需求
 *
 * 仅支持任意采样格式转换为 S16LE。输入音频槽为 S16LE，生成的新音频槽为任意
 * 采样格式
 */
template <typename T, HCI_VALID_IF(is_audio_sink_<T>::value)>
static inline safe_ptr<HciAudioSink> Convert(T &&sink) {
  auto obj = api::_hci_convert_audio_sink(
      pass<HciAudioSink>(std ::forward<T>(sink)).get());
  return wrap(obj).safe();
}

static inline int sample_bytes(HciSampleFormat sample_format) {
  switch (sample_format) {
    case HciSampleFormat::ALAW:
    case HciSampleFormat::ULAW:
    case HciSampleFormat::U8:
      return 1;
    case HciSampleFormat::S16BE:
    case HciSampleFormat::S16LE:
      return 2;
    case HciSampleFormat::F32BE:
    case HciSampleFormat::F32LE:
    case HciSampleFormat::S32BE:
    case HciSampleFormat::S32LE:
      return 4;
    case HciSampleFormat::S24BE:
    case HciSampleFormat::S24LE:
      return 3;
    default:
      return -1;
  }
}

static inline bool is_sample_rate_ok(int sample_rate) {
  switch (sample_rate) {
    case 8000:
    case 11025:
    case 16000:
    case 22050:
    case 32000:
    case 48000:
    case 44100:
    case 96000:
    case 192000:
      return true;
    default:
      return false;
  }
}

static inline int min_frame_time(int sample_rate) {
  switch (sample_rate) {
    case 8000:
    case 16000:
    case 32000:
    case 48000:
    case 44100:
    case 96000:
    case 192000:
      return 10;
    case 11025:
      return 40;
    case 22050:
      return 20;
    default:
      return -1;
  }
}

inline void HciAudioMetrics::Validate() const {
  const char *file = "hci/audio.h";
  if (has_format() && sample_bytes(format()) < 0) {
    api::hci_abort2(file, __LINE__, "bad format = %d", *format());
  }
  if (has_channels() && (channels() <= 0 || channels() > 10)) {
    api::hci_abort2(file, __LINE__, "bad channels = %d", channels());
  }
  if (has_sample_rate() && !is_sample_rate_ok(sample_rate())) {
    api::hci_abort2(file, __LINE__, "bad sample_rate = %d", sample_rate());
  }
  if (has_frame_time()) {
    if (!has_sample_rate()) {
      api::hci_abort(file, __LINE__, "sample_rate not set");
    }
    int min = ::hci::min_frame_time(sample_rate());
    if (frame_time() % min != 0)
      api::hci_abort2(
          file, __LINE__,
          "bad frame_time %d for sample_rate %d, min_frame_time = %d",
          frame_time(), sample_rate(), min);
    if (frame_time() <= 0 || frame_time() > 1000)
      api::hci_abort2(file, __LINE__, "bad frame_time = %d", frame_time());
  }
}

inline int HciAudioMetrics::sample_size() const {
  return sample_bytes(format()) * channels();
}

inline int HciAudioMetrics::frame_size() const {
  return CalcDataLenByTimeLen(frame_time());
}

inline int HciAudioMetrics::min_frame_time() const {
  return ::hci::min_frame_time(sample_rate());
}

inline int HciAudioMetrics::CalcDataLenByTimeLen(int ms) const {
  int64_t size1s = sample_bytes(format()) * channels() * sample_rate();
  return static_cast<int>(size1s * ms / 1000);
}

}  // namespace hci
#endif  // HCI_AUDIO_H_
