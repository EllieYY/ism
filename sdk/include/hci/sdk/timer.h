
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/sdk/timer.h
 *
 * 灵云 SDK 定时器接口
 */
#ifndef HCI_SDK_TIMER_H_
#define HCI_SDK_TIMER_H_

#include <hci/callback.h>

HCI_NS_BEGIN(hci, sdk);
/**
 * 灵云 SDK 定时器
 */
class HCI_LOCAL HciTimer : public HciObject {
  HCI_INTERFACE(HciTimer);

 public:
  virtual HciError Start(int timeout, int interval) = 0;
  virtual HciError Stop() = 0;
  virtual HciError Close() = 0;
};
HCI_INTERFACE_(HciTimer);

HCI_DECL_CALLBACK(HciTimerCB, void, HciTimer *);

IS_HCI_PTR(is_timer_cb_, HciTimerCB);

#if !defined(HCI_EXPLICIT_IMPL)
HCI_IMPL_CALLBACK(HciTimerCB, void, HciTimer *);
#endif

HCI_NS_END(hci, sdk);

#endif  // HCI_SDK_TIMER_H_
