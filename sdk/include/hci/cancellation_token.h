
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/cancellation_token.h
 *
 * 定义取消令牌
 *
 * 在取消操作场景中，会涉及两类参与用户：取消操作发起者和取消操作执行者。
 * 至少存在一个取消操作执行者，某些场景中也可能会存更多的取消操作执行者。
 *
 * 用户应按如下方法使用:
 *
 *   - 取消操作发起者创建 *`HciCancellationTokenSource`*，并调用
 *     *`HciCancellationTokenSource::Token`* 方法获取 *`HciCancellationToken`*
 *     对象，将其传递给所有取消操作执行者
 *   - 取消操作执行者会调用 *`HciCancellationToken::Register`* 注册取消回调，
 *     用于接收取消操作通知，接收到通知后执行取消操作（一般为异步操作）
 *   - 取消操作执行者若在取消操作发生之前完成任务或有其他原因，应调用
 *     *`HciCancellationToken::Unegister`* 注销取消回调
 *   - 取消操作发起者应保持 *`HciCancellationTokenSource`* 的一个引用计数，
 *     其引用计数变为零时，若还未取消则会自动发起取消操作
 *   - 取消操作执行者应保持 *`HciCancellationToken`* 的一个引用计数，
 *     其引用计数变为零时，会自动注销已注册的所有取消回调
 *
 * 使用 *`HciCancellationTokenSource`* 和 *`HciCancellationToken`* 的意义在于，
 * 将取消操作的发起和执行进行解耦。
 *
 * 公开接口：
 *
 *   - class HciCancellationToken;
 *   - calss HciCancellationTokenSource;
 */
#ifndef HCI_CANCELLATION_TOKEN_H_
#define HCI_CANCELLATION_TOKEN_H_

#include <hci/callback.h>

#include <assert.h>
#include <stdarg.h>

#include <utility>
#include <vector>

namespace hci {

/**
 * 用于注册/注销取消回调
 */
class HCI_LOCAL HciCancellationToken : public HciObject {
  HCI_INTERFACE(HciCancellationToken);

 public:
  /**
   * 取消操作是否已发起
   */
  virtual bool IsCancellationRequested() const = 0;

  /**
   * 注册取消回调
   *
   *   @param [in]  callback 取消回调
   *   @return 返回用于注销的 cookie
   *
   * 可注册多个回调，调用顺序为 LIFO，先注册的后被执行。若取消操作已发起，
   * 取消回调会马上被执行，然后返回零值。否则返回非零值。
   *
   * 内部方法，用户应调用模板方法 Register
   */
  virtual int _Register(pass_ptr<HciVoidCB> callback) = 0;

  /**
   * 注销取消回调
   *
   *   @param [in]  cookie  要注销的取消回调的 cookie
   */
  virtual void Unregister(int cookie) = 0;

  /**
   * 注册取消回调
   *
   *   @param [in]  callback  取消回调
   *   @return 返回用于注销的 cookie
   *
   *   @tparam T 取消回调类型。可以是 HciVoidCB *, safe_ptr<HciVoidCB>
   *             及其引用类型，内联实现会将其转换为 pass_ptr<HciVoidCB>
   *             并调用 _Register
   *
   * 可注册多个回调，调用顺序为 LIFO，先注册的后被执行。若取消操作已发起，
   * 取消回调会马上被执行，然后返回零值。否则返回非零值。
   */
  template <typename T, HCI_VALID_IF(is_void_cb_<T>::value)>
  int Register(T &&callback) {
    return _Register(pass<HciVoidCB>(std::forward<T>(callback)));
  }

#ifndef HCI_EXPLICIT_IMPL
  /**
   * 注册取消回调
   *
   *   @param [in]  func  取消回调
   *   @return 返回用于注销的 cookie
   *
   *   @tparam F 取消回调类型，可以是函数指针或仿函数，内联实现会调用
   *             HciVoidCB::New 方法创建回调接口并调用 _Register
   *
   * 可注册多个回调，调用顺序为 LIFO，先注册的后被执行。若取消操作已发起，
   * 取消回调会马上被执行，然后返回零值。否则返回非零值。
   */
  template <typename F, HCI_VALID_IF(func_traits<F>::is_func)>
  int Register(F &&func) {
    auto cb = HciVoidCB::New<F>(std::forward<F>(func));
    return _Register(cb.pass());
  }

  /**
   * 注册取消回调
   *
   *   @param [in]  func  取消回调
   *   @param [in]  data  取消回调上下文
   *   @return 返回用于注销的 cookie
   *
   *   @tparam F 取消回调类型，可以是函数指针，仿函数或者成员函数指针类型，
   *             内联实现会调用 HciVoidCB::New 方法创建回调接口并调用 _Register
   *   @tparam D 取消回调上下文参数类型。当 F 为成员函数指针类型时，D 应为
   *             成员函数所属对象的类型，会以 (data->*func)() 的方式调用异步回
   *             调；否则，D 可以是除右值引用之外的任意类型，会以 func(data)
   *             的方式调用异步回调
   *
   * 可注册多个回调，调用顺序为 LIFO，先注册的后被执行。若取消操作已发起，
   * 取消回调会马上被执行，然后返回零值。否则返回非零值。
   */
  template <typename F, typename D, HCI_VALID_IF(func_traits<F>::value)>
  int Register(F &&func, D &&data) {
    auto cb = HciVoidCB::New<F, D>(std::forward<F>(func),  //
                                   std::forward<D>(data));
    return Register(cb.pass());
  }
#endif
};

HCI_INTERFACE_(HciCancellationToken);

/**
 * 用于发起取消操作
 */
class HCI_LOCAL HciCancellationTokenSource : public HciObject {
  HCI_INTERFACE(HciCancellationTokenSource);

 public:
  /**
   * 静态方法，创建 HciCancellationTokenSource 对象
   */
  static safe_ptr<HciCancellationTokenSource> New() {
    return wrap(api::_hci_new_token_source()).safe();
  }
  /**
   * 静态方法，创建与 n 个 Token 实例相关联的 HciCancellationTokenSource 对象
   *
   * 任何一个关联的 Token 接收到取消操作通知时，均会在本对象上发起取消操作，
   * 也可以通过调用自身的 Cancel 发起取消操作。
   *
   * @param [in] n     关联 Token 数量
   * @param [in] token 关联 Token 实例, 后续应继续传入 n-1 个 Token 实例
   */
  static safe_ptr<HciCancellationTokenSource> New(  //
      int n, HciCancellationToken *token, ...) {
    assert(n > 0);
    if (n <= 0) return New();
    std::vector<HciCancellationToken *> tokens(n);
    va_list va;
    va_start(va, token);
    tokens[0] = token;
    for (int i = 1; i < n; ++i) {
      tokens[i] = va_arg(va, HciCancellationToken *);
    }
    return wrap(api::_hci_new_linked_token_source(n, &tokens[0])).safe();
  }

 public:
  /**
   * 发起取消操作
   */
  virtual void Cancel() = 0;

  /**
   * 取消操作是否已发起
   */
  virtual bool IsCancellationRequested() const = 0;

 private:
  /**
   * 获取 HciCancellationToken 对象
   */
  virtual HciCancellationToken *_Token() = 0;

 public:
  /**
   * 获取 HciCancellationToken 对象
   */
  safe_ptr<HciCancellationToken> Token() { return wrap(_Token()).safe(); }
};

HCI_INTERFACE_(HciCancellationTokenSource);

IS_HCI_PTR(is_token_, HciCancellationToken);

}  // namespace hci

#endif  // HCI_CANCELLATION_TOKEN_H_
