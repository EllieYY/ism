
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/api.h
 *
 * 绝大部分通用模块接口都在此文件内声明并维护，集中管理的好处是容易跟踪接口
 * 变动，便于进行兼容性评估
 *
 * 声明于 hci::api 命名空间中的接口都不是直接面向用户的，用户应使用经过 C++
 * 封装的内联实现
 *
 * 声明于 hci 命名空间中的接口都是面向用户的，用户可以直接使用
 */
#ifndef HCI_API_H_
#define HCI_API_H_

#ifndef HCI_HCI_H_
#error "Please include hci/hci.h instead of hci/api.h"
#endif

#include <stdio.h>

namespace hci {

class HCI_LOCAL HciAudioMetrics;
class HCI_LOCAL HciAudioSource;
class HCI_LOCAL HciAudioSink;
class HCI_LOCAL HciAudioBuffer;
class HCI_LOCAL HciBuffer;
class HCI_LOCAL HciMutableMetadata;
class HCI_LOCAL HciAbortCB;
class HCI_LOCAL HciCancellationToken;
class HCI_LOCAL HciCancellationTokenSource;
class HCI_LOCAL Console;
class HCI_LOCAL HciMetaObject;
class HCI_LOCAL HciObject;
struct HCI_LOCAL HciVersion;

enum struct HciError : int;

namespace api {
extern "C" {
// clang-format off
HCI_COMMON_API HciAudioSink *_hci_convert_audio_sink(HciAudioSink *);
HCI_COMMON_API HciAudioSink *_hci_resample_audio_sink(HciAudioSink *);

HCI_COMMON_API HciAudioSource *_hci_convert_audio_source(HciAudioSource *);
HCI_COMMON_API HciAudioSource *_hci_resample_audio_source(HciAudioSource *);

HCI_COMMON_API HciAudioSource *_hci_new_file_audio_source(const HciAudioMetrics *, const char *, int, int);
HCI_COMMON_API HciAudioSource *_hci_new_file_audio_source_utf8(const HciAudioMetrics *, const char *, int, int);
HCI_COMMON_API HciAudioSource *_hci_new_data_audio_source(const HciAudioMetrics *, const void *, size_t, bool);

HCI_COMMON_API HciAudioBuffer *_hci_new_audio_buffer(const HciAudioMetrics *, int);

HCI_COMMON_API HciBuffer *_hci_new_buffer(size_t);
HCI_COMMON_API HciBuffer *_hci_new_refbuf(HciBuffer *, size_t, size_t);
HCI_COMMON_API HciMutableMetadata *_hci_new_metadata(bool);

HCI_COMMON_API int _hci_reg_abort_cb(HciAbortCB *);
HCI_COMMON_API void _hci_unreg_abort_cb(int);

HCI_COMMON_API HciCancellationTokenSource *_hci_new_token_source();
HCI_COMMON_API HciCancellationTokenSource *_hci_new_linked_token_source(int, HciCancellationToken **);

HCI_COMMON_API Console *_hci_open_console(int);

HCI_COMMON_API HCI_NORETURN
void hci_abort(const char *, int, const char *);

HCI_COMMON_API HCI_NORETURN __attribute__((format(printf, 3, 4)))
void hci_abort2(const char *file, int line, const char *fmt, ...);

HCI_COMMON_API HciMetaObject *_hci_new_metaobj();

HCI_COMMON_API void _hci_pcm_u8_to_s16le   (const void *, void *, size_t);
HCI_COMMON_API void _hci_pcm_alaw_to_s16le (const void *, void *, size_t);
HCI_COMMON_API void _hci_pcm_ulaw_to_s16le (const void *, void *, size_t);
HCI_COMMON_API void _hci_pcm_s16be_to_s16le(const void *, void *, size_t);
HCI_COMMON_API void _hci_pcm_s24le_to_s16le(const void *, void *, size_t);
HCI_COMMON_API void _hci_pcm_s24be_to_s16le(const void *, void *, size_t);
HCI_COMMON_API void _hci_pcm_s32le_to_s16le(const void *, void *, size_t);
HCI_COMMON_API void _hci_pcm_s32be_to_s16le(const void *, void *, size_t);
HCI_COMMON_API void _hci_pcm_f32le_to_s16le(const void *, void *, size_t);
HCI_COMMON_API void _hci_pcm_f32be_to_s16le(const void *, void *, size_t);

HCI_COMMON_API const char *_hci_dump_audio_metrics(const HciAudioMetrics *, char *, size_t);
// clang-format on
}  // extern "C"
};  // namespace api

extern "C" {
// 返回一个唯一的线程标识，与系统线程标识无关
HCI_COMMON_API int hci_tid();
// 返回当前进程标识
HCI_COMMON_API int hci_pid();
// 返回错误码对应的显示名称
HCI_COMMON_API const char *hci_error_name(HciError code);
// 返回通用模块版本
HCI_COMMON_API const HciVersion *hci_version();
}

};  // namespace hci

#endif  // HCI_API_H_
