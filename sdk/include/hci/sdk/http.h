
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/sdk/http.h
 *
 * 灵云 SDK HTTP 请求
 */
#ifndef HCI_SDK_HTTP_H_
#define HCI_SDK_HTTP_H_

#include <hci/callback.h>
#include <hci/table.h>

HCI_NS_BEGIN(hci, sdk);

/**
 * 认证方法
 */
enum struct HciAuthMethod : int {
  NONE = 0,
  BASIC = 1,  // WWW BASIC 认证
  TOKEN = 2,  // 访问令牌认证
};

HCI_ENUM(HciAuthMethod, int);

/**
 * Http 请求
 */
// clang-format off
HCI_DECL_TABLE(HciHttpReq)
  HCI_XPSTR_FIELD(const char *, method, true);
  HCI_XPSTR_FIELD(const char *, uri, true);
  HCI_BYTES_FIELD(void, content, true);
  HCI_ARRAY_FIELD(const char *, headers, true);
  HCI_PLAIN_FIELD(int32_t, timeout, true);
  HCI_PLAIN_FIELD(HciAuthMethod, auth_method, true);
  HCI_XPSTR_FIELD(const char *, access_token);
HCI_DECL_TABLE_END(HciHttpReq);
// clang-format on

/**
 * Http 响应
 */
// clang-format off
HCI_DECL_TABLE(HciHttpRes)
  HCI_PLAIN_FIELD(int32_t, status_code, true)
  HCI_BLANK_FIELD(int32_t)
  HCI_XPSTR_FIELD(const char *, status_text, true)
  HCI_XPSTR_FIELD(const char *, content_type, true)
  HCI_BYTES_FIELD(void, content, true)
  HCI_ARRAY_FIELD(const char *, headers, true)
HCI_DECL_TABLE_END(HciHttpRes);
// clang-format on

HCI_DECL_CALLBACK(HciHttpReqCB, void, HciError, const HciHttpRes *);

IS_HCI_PTR(is_http_req_cb_, HciHttpReqCB);

#if !defined(HCI_EXPLICIT_IMPL)
HCI_IMPL_CALLBACK(HciHttpReqCB, void, HciError, const HciHttpRes *);
#endif

HCI_NS_END(hci, sdk);

#endif  // HCI_SDK_HTTP_H_
