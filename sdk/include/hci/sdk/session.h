
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/sdk/session.h
 *
 * 灵云 SDK 能力会话接口
 */
#ifndef HCI_SDK_SESSION_H_
#define HCI_SDK_SESSION_H_

#include <hci/object.h>
#include <hci/version.h>

#include <assert.h>

#include <utility>

HCI_NS_BEGIN(hci, sdk);

/**
 * 能力描述
 */
struct HCI_LOCAL HciCapInfo {
  HciVersion version;       // 能力版本
  const char *capkey;       // capkey
  const char *description;  // 描述信息
};

/**
 * 能力会话状态
 */
enum struct SessionState : uint8_t {
  Idle = 0,      // 空闲状态
  Running = 1,   // 运行状态
  Stopping = 2,  // 停止中状态
  Closing = 3,   // 关闭中状态
  Closed = 4,    // 已关闭状态
};

HCI_ENUM(SessionState, uint8_t);

inline const char *state_name(SessionState s) {
  uint8_t i = *s;
  if (i < 0 || i > 4) i = 5;
  static const char *const names[] = {
      "Idle", "Running", "Stopping", "Closing", "Closed", "UNKNOWN",
  };
  return names[i];
}

/**
 * 能力会话基类
 */
class HCI_LOCAL HciSession : public HciObject {
  HCI_INTERFACE(HciSession);

 public:
  virtual HciSdk *sdk() const = 0;  // 获取会话依属于的 HciSdk 对象
  virtual SessionState state() const = 0;          // 获取会话状态
  virtual const HciCapInfo *cap_info() const = 0;  // 获取能力描述信息

  /**
   * 关闭会话，异步操作
   *
   * 用于关闭会话，本方法会尽快返回，用户可调用 WaitForClosed 等待对象完成
   * 关闭操作
   *
   *   - 在 Idle/Running/Stopping 状态下调用，会使对象进入 Closing 状态，
   *     直到完成关闭，进入 Closed 状态
   *   - 在 Closing/Closed 状态下调用，无任何操作，不会改变对象状态
   */
  virtual void Close() = 0;

  /**
   * 等待会话对象被关闭，异步操作
   *
   *   @param [in]  callback  异步回调，对象关闭后被调用
   *
   * 当对象转换至 Closed 状态后，异步回调被调用。若调用 WaitForClosed 时对象
   * 已处于 Closed 状态，则会尽快调用异步回调。
   *
   * 本方法允许调用多次，所有传入的异步回调会以 First-In-Last-Out 的顺序被调用
   *
   * 内部方法，用户应使用模板方法 WaitForClosed
   */
  virtual void _WaitForClosed(pass_ptr<HciVoidCB> callback) = 0;

  /**
   * 等待会话对象被关闭，异步操作
   *
   *   @param [in]  callback  异步回调，对象关闭后被调用
   *
   *   @tparam CB 异步回调类型。可以是 HciVoidCB *，safe_ptr<HciVoidCB>
   *              及其引用类型。内联实现会将其转换为 pass_ptr<HciVoidCB>
   *              并调用 _WaitForClosed
   *
   * 当对象转换至 Closed 状态后，异步回调被调用。若调用 WaitForClosed 时对象
   * 已处于 Closed 状态，则会尽快调用异步回调。
   *
   * 本方法允许调用多次，所有传入的异步回调会以 First-In-Last-Out 的顺序被调用
   */
  template <typename CB, HCI_VALID_IF(is_void_cb_<CB>::value)>
  void WaitForClosed(CB &&callback) {
    _WaitForClosed(pass<HciVoidCB>(std::forward<CB>(callback)));
  }

#ifndef HCI_EXPLICIT_IMPL
  /**
   * 等待会话对象被关闭，异步操作
   *
   *   @param [in]  func  异步回调，对象关闭后被调用
   *
   *   @tparam F 异步回调类型。可以是函数指针或仿函数类型，内联实现会调用
   *             HciVoidCB::New 创建异步回调接口并调用 _WaitForClosed
   *
   * 当对象转换至 Closed 状态后，异步回调被调用。若调用 WaitForClosed 时对象
   * 已处于 Closed 状态，则会尽快调用异步回调。
   *
   * 本方法允许调用多次，所有传入的异步回调会以 First-In-Last-Out 的顺序被调用
   */
  template <typename F, HCI_VALID_IF(func_traits<F>::is_func)>
  void WaitForClosed(F &&func) {
    auto cb = HciVoidCB::New<F>(std::forward<F>(func));
    _WaitForClosed(cb.pass());
  }

  /**
   * 等待会话对象被关闭，异步操作
   *
   *   @param [in]  func  异步回调，对象关闭后被调用
   *   @param [in]  data  异步回调上下文参数
   *
   *   @tparam F 异步回调类型，可以是函数指针，仿函数或者成员函数指针类型，
   *             内联实现会调用 HciVoidCB::New 创建异步回调接口并调用
   *             _WaitForClosed
   *
   *   @tparam D 上下文参数类型。当 F 为成员函数指针类型时，D 应为成员函数所属
   *             对象指针的类型，会以 (data->*func)() 的方式调用异步回调；当 F
   *             为函数指针或仿函数类型时，会以 func(data) 的方式调用异步回调
   *
   * 当对象转换至 Closed 状态后，异步回调被调用。若调用 WaitForClosed 时对象
   * 已处于 Closed 状态，则会尽快调用异步回调。
   *
   * 本方法允许调用多次，所有传入的异步回调会以 First-In-Last-Out 的顺序被调用
   */
  template <typename F, typename D, HCI_VALID_IF(func_traits<F>::value)>
  void WaitForClosed(F &&func, D &&data) {
    auto cb = HciVoidCB::New<F, D>(  //
        std::forward<F>(func), std::forward<D>(data));
    _WaitForClosed(cb.pass());
  }

  /**
   * 等待会话对象被关闭，同步操作
   *
   * 不能在事件循环线程中调用本方法，会发生死锁
   */
  void WaitForClosed() {
    if (sdk()->in_loop_thread()) {
      hci::api::hci_abort("hci/sdk/session.h", __LINE__, "in loop thread");
    }
    HciEvent event;
    WaitForClosed([&event]() { event.Set(); });
    event.Wait();
  }
#endif

 private:
  // 预留扩展用
  virtual void reserved1() = 0;
  virtual void reserved2() = 0;
  virtual void reserved3() = 0;
  virtual void reserved4() = 0;
};

HCI_INTERFACE_(HciSession);

HCI_NS_END(hci, sdk);

#endif  // HCI_SDK_SESSION_H_
