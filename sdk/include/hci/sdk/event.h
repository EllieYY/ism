
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/sdk/event.h
 *
 * HciEvent 类，多在内联实现中实现同步接口时使用
 */
#ifndef HCI_SDK_EVENT_H_
#define HCI_SDK_EVENT_H_

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <hci/error.h>

HCI_NS_BEGIN(hci, sdk);

#if defined(_WIN32)
class HCI_LOCAL HciEvent {
 public:
  HciEvent(bool state = false) {
    event_ = ::CreateEventW(NULL, TRUE, state, NULL);
  }
  ~HciEvent() {  //
    CloseHandle(event_);
  }
  void Set(HciError code = HciError::OK) {
    code_ = code;
    SetEvent(event_);
  }
  void Reset() {  //
    ResetEvent(event_);
  }
  HciError Wait() {
    WaitForSingleObject(event_, INFINITE);
    return code_;
  }
  HciError Wait(int ms) {
    if (ms > 0 && WAIT_OBJECT_0 == WaitForSingleObject(event_, ms))
      return code_;
    else
      return HciError::DEADLINE_EXCEEDED;
  }

 private:
  HciError code_;
  HANDLE event_;
};
#else
class HCI_LOCAL HciEvent {
 public:
  HciEvent(bool state = false) : signal_(state) {
    pthread_mutex_init(&mutex_, nullptr);
    pthread_cond_init(&cond_, nullptr);
  }
  ~HciEvent() {
    pthread_mutex_destroy(&mutex_);
    pthread_cond_destroy(&cond_);
  }
  void Reset() {
    pthread_mutex_lock(&mutex_);
    signal_ = false;
    pthread_mutex_unlock(&mutex_);
  }
  void Set(HciError code = HciError::OK) {
    pthread_mutex_lock(&mutex_);
    signal_ = true;
    code_ = code;
    pthread_cond_signal(&cond_);
    pthread_mutex_unlock(&mutex_);
  }
  HciError Wait() {
    HciError code;
    pthread_mutex_lock(&mutex_);
    while (!signal_) {
      pthread_cond_wait(&cond_, &mutex_);
    }
    code = code_;
    pthread_mutex_unlock(&mutex_);
    return code;
  }
#if !__APPLE__
  HciError Wait(int ms) {
    HciError code = HciError::DEADLINE_EXCEEDED;
    struct timespec ts;
    if (ms <= 0) return code;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += ms / 1000;
    ts.tv_nsec += (ms % 1000) * 1000000;
    if (ts.tv_nsec >= 1000000000)  //
      ts.tv_sec++, ts.tv_nsec -= 1000000000;
    pthread_mutex_lock(&mutex_);
    while (!signal_) {
      if (0 != pthread_cond_timedwait(&cond_, &mutex_, &ts)) goto done;
    }
    code = code_;
  done:
    pthread_mutex_unlock(&mutex_);
    return code;
  }
#endif

 private:
  bool signal_;
  HciError code_;
  pthread_mutex_t mutex_;
  pthread_cond_t cond_;
};
#endif

HCI_NS_END(hci, sdk);

#endif  // HCI_SDK_EVENT_H_
