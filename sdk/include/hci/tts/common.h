
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/tts/common.h
 *
 * TTS 能力公共内容
 */
#ifndef HCI_TTS_COMMON_H_
#define HCI_TTS_COMMON_H_

#include <hci/table.h>

HCI_NS_BEGIN(hci, sdk, tts);

/**
 * 本地能力设置，用于创建本地 TTS 能力会话对象
 */
// clang-format off
HCI_DECL_TABLE(LocalTtsConfig);
  HCI_PLAIN_FIELD(int32_t, reserved1, true);  // [I4]
HCI_DECL_TABLE_END(LocalTtsConfig);
// clang-format on

/**
 * 云端能力设置，用于创建云端 TTS 能力会话对象
 */
// clang-format off
HCI_DECL_TABLE(CloudTtsConfig);
  HCI_PLAIN_FIELD(int32_t, reserved1, true);  // [I4]
HCI_DECL_TABLE_END(CloudTtsConfig);
// clang-format on

HCI_NS_END(hci, sdk, tts);

#endif  // HCI_TTS_COMMON_H_
