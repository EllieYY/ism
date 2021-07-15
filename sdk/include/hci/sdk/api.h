
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/sdk/api.h
 *
 * 灵云 SDK 底层接口都在此文件内声明并维护，集中管理的好处是容易跟踪接口
 * 变动，便于进行兼容性评估
 *
 * 声明于 hci::sdk::api 命名空间中的接口都不是直接面向用户的，用户应使用
 * 经过 C++ 封装的内联实现
 *
 * 声明于 hci::sdk 命名空间内的接口都是面向用户的，用户可以直接使用
 */
#ifndef HCI_SDK_API_H_
#define HCI_SDK_API_H_

#include <hci/hci.h>
#include <hci/version.h>

HCI_NS_BEGIN(hci, sdk);

extern "C" HCI_SDK_API const HciVersion *sys_version();

class HCI_LOCAL HciSdk;

namespace asr {
extern "C" HCI_SDK_API const HciVersion *asr_version();
class HCI_LOCAL LocalAsrConfig;
class HCI_LOCAL CloudAsrConfig;

class HCI_LOCAL FreetalkShortAudio;
class HCI_LOCAL FreetalkStream;
}  // namespace asr

namespace tts {
extern "C" HCI_SDK_API const HciVersion *tts_version();
class HCI_LOCAL LocalTtsConfig;
class HCI_LOCAL CloudTtsConfig;
class HCI_LOCAL SynthShortText;
class HCI_LOCAL SynthStream;
}  // namespace tts

namespace api {
extern "C" {
HCI_SDK_API HciSdk *_hci_new_sdk() throw();
HCI_SDK_API asr::FreetalkShortAudio *_hci_new_local_ft_shortaudio(
    HciSdk *, const asr::LocalAsrConfig *) throw();
HCI_SDK_API asr::FreetalkShortAudio *_hci_new_cloud_ft_shortaudio(
    HciSdk *, const asr::CloudAsrConfig *) throw();
HCI_SDK_API asr::FreetalkStream *_hci_new_local_ft_stream(  //
    HciSdk *, const asr::LocalAsrConfig *) throw();
HCI_SDK_API asr::FreetalkStream *_hci_new_cloud_ft_stream(  //
    HciSdk *, const asr::CloudAsrConfig *) throw();
HCI_SDK_API tts::SynthShortText *_hci_new_local_synth_shorttext(  //
    HciSdk *, const tts::LocalTtsConfig *) throw();
HCI_SDK_API tts::SynthShortText *_hci_new_cloud_synth_shorttext(  //
    HciSdk *, const tts::CloudTtsConfig *) throw();
HCI_SDK_API tts::SynthStream *_hci_new_local_synth_stream(  //
    HciSdk *, const tts::LocalTtsConfig *) throw();
HCI_SDK_API tts::SynthStream *_hci_new_cloud_synth_stream(  //
    HciSdk *, const tts::CloudTtsConfig *) throw();
}  // extern "C"
}  // namespace api

HCI_NS_END(hci, sdk);

#endif  // HCI_SDK_API_H_
