
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/sdk/auth.h
 *
 * 灵云 SDK 授权
 */
#ifndef HCI_SDK_AUTH_H_
#define HCI_SDK_AUTH_H_

#include <hci/table.h>

HCI_NS_BEGIN(hci, sdk);

/**
 * 能力授权信息
 */
// clang-format off
HCI_DECL_TABLE(HciCapAuthInfo)
  HCI_XPSTR_FIELD(const char *, capkey, true)
  HCI_XPSTR_FIELD(const char *, property, true)
  HCI_PLAIN_FIELD(int32_t, life_time, true)
HCI_DECL_TABLE_END(HciCapAuthInfo);
// clang-format on

enum struct HciAuthStatus : int {
  UNKNOWN = -1,          // 未知状态，授权更新中
  NORMAL = 0,            // 正常状态
  EXPIRED = 1,           // 授权过期状态
  EXCEEDED_DEVICES = 2,  // 终端数量超限
  APPKEY_REVOKED = 3,    // appkey 被注销
};

HCI_ENUM(HciAuthStatus, int);

/**
 * 授权信息，通过合并 HCI_LOCAL_AUTH, HCI_AUTH 和 HCI_EXTRA_AUTH
 * 及本地信息而得到
 */
// clang-format off
HCI_DECL_TABLE(HciAuthInfo)
  HCI_PLAIN_FIELD(int32_t, serial, true)
  HCI_PLAIN_FIELD(HciAuthStatus, auth_status, true)
  HCI_PLAIN_FIELD(int32_t, device_limit, true)
  HCI_PLAIN_FIELD(int32_t, session_limit, true)
  HCI_PLAIN_FIELD(int64_t, valid_start_time, true)
  HCI_PLAIN_FIELD(int64_t, valid_end_time, true)
  HCI_PLAIN_FIELD(int64_t, activation_time, true)
  HCI_PLAIN_FIELD(int64_t, next_check_auth_time, true)
  HCI_ARRAY_FIELD(const char *, udids, true)
  HCI_ARRAY_FIELD(HciCapAuthInfo, caps, true)
  HCI_XPSTR_FIELD(const char *, description, true)
HCI_DECL_TABLE_END(HciAuthInfo);
// clang-format on

HCI_NS_END(hci, sdk);

#endif  // HCI_SDK_AUTH_H_
