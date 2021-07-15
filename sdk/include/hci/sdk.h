
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/sdk.h
 *
 * 定义灵云 SDK HciSdk 类
 *
 * 公开接口:
 *
 *   - class HciSdk
 *   - class HciSdkConfig
 */
#ifndef HCI_SDK_H_
#define HCI_SDK_H_

#ifndef HCI_SDK_API
#if _MSC_VER
#define HCI_SDK_API
#else
#define HCI_SDK_API HCI_IMPORT
#endif
#endif

#include <hci/sdk/api.h>
#include <hci/sdk/auth.h>
#include <hci/sdk/http.h>
#include <hci/sdk/timer.h>
#include <hci/sdk/log.h>
#include <hci/sdk/event.h>

#include <hci/cancellation_token.h>

#include <utility>

HCI_NS_BEGIN(hci, sdk);

class HCI_LOCAL HciSession;
class HCI_LOCAL WebSocket;
class HCI_LOCAL WebSocketHandler;

/**
 * SDK 配置，用于创建 HciSdk 对象
 */
// clang-format off
HCI_DECL_TABLE(HciSdkConfig)
  HCI_XPSTR_FIELD(const char *, appkey, true);            // [LP]
  HCI_XPSTR_FIELD(const char *, secret, true);            // [LP]
  HCI_XPSTR_FIELD(const char *, udid, true);              // [LP]
  HCI_XPSTR_FIELD(const char *, sys_url, true);           // [LP] 系统服务地址
  HCI_XPSTR_FIELD(const char *, cap_url, true);           // [LP] 能力服务地址
  HCI_XPSTR_FIELD(const char *, local_auth_path, true);   // [LP]
  HCI_XPSTR_FIELD(const char *, data_path, true);         // [LP]
  HCI_XPSTR_FIELD(const char *, http_proxy, true);        // [LP]
  HCI_XPSTR_FIELD(const char *, proxy_username, true);    // [LP]
  HCI_XPSTR_FIELD(const char *, proxy_password, true);    // [LP]
  HCI_XPSTR_FIELD(const char *, ca_bundle, true);         // [LP]
  HCI_PLAIN_FIELD(bool, verify_ssl, true);                // [B1]
  HCI_PLAIN_FIELD(bool, custom_access_token);             // [B1] 10.2.0 增加
HCI_DECL_TABLE_END(HciSdkConfig);
// clang-format on

enum struct SdkState {
  Created = 0,  // 新建状态
  Running = 1,  // 运行中状态
  Closing = 2,  // 关闭中状态
  Closed = 3,   // 已关闭状态
};

HCI_ENUM(SdkState, int);

/**
 * HciSdk 类
 */
class HCI_LOCAL HciSdk : public HciObject {
  HCI_INTERFACE(HciSdk);

 protected:
  LogLevel level_;
  SdkState state_;

 public:
  /**
   * 静态方法，创建 HciSdk 对象
   */
  static safe_ptr<HciSdk> New() { return wrap(api::_hci_new_sdk()).safe(); }

 public:
  SdkState sdk_state() const { return state_; }           // HciSdk 对象状态
  LogLevel log_level() const { return level_; }           // 日志输出级别
  void set_log_level(LogLevel level) { level_ = level; }  // 设置日志输出级别

  /**
   * 获取使用的 HciSdkConfig 配置，与用户调用 Init 的传参不是同一个对象
   */
  virtual const HciSdkConfig *sdk_config() const = 0;

  virtual int session_count() = 0;  // 未关闭的能力会话数量

  /**
   * 判断当前线程是不是事件循环线程
   */
  virtual bool in_loop_thread() const = 0;

  /**
   * 日志输出，一般不直接调用此方法，而是使用 hci/sdk/log.h 中的宏
   */
  virtual __attribute__((format(printf, 5, 6))) void  //
  Log(LogLevel level, const char *file, int line, const char *fmt, ...) = 0;

  /**
   * SDK 初始化
   */
  virtual HciError Init(HciSdkConfig *config) = 0;

  /**
   * 关闭 SDK 对象，异步操作
   */
  virtual void Close() = 0;

  /**
   * 等待 HciSdk 对象被关闭，异步操作
   */
  virtual void _WaitForClosed(pass_ptr<HciVoidCB> callback) = 0;

  /**
   * 创建定时器
   */
  virtual pass_ptr<HciTimer> _NewTimer(pass_ptr<HciTimerCB> callback) = 0;

  /**
   * HTTP 调用，异步操作。
   */
  virtual void _HttpInvoke(pass_ptr<HciHttpReqCB> callback,
                           const HciHttpReq *req,
                           pass_ptr<HciCancellationToken> token) = 0;

  /**
   * 创建 WebSocket 连接
   */
  virtual pass_ptr<WebSocket> _NewWebSocket(  //
      const HciHttpReq *req, WebSocketHandler *handler) = 0;

  /**
   * 获取授权信息
   *
   * 在 Running 状态下本方法总会成功，但授权信息中的 auth_status 字段可能
   * 为 UNKNOWN 状态。
   */
  virtual HciError GetHciAuth(HciAuthInfo *info) = 0;

  /**
   * 等待授权信息就绪
   *
   * 授权状态为 HciAuthStatus::UNKNOWN 时，说明正在从云端服务更新授权。可用
   * 本方法注册一个回调，授权更新后注册的回调将会被调用。
   */
  virtual void _WaitForAuthReady(pass_ptr<HciErrorCB> callback) = 0;

  /**
   * 等待 HciSdk 对象被关闭，异步操作
   */
  template <typename CB, HCI_VALID_IF(is_void_cb_<CB>::value)>
  void WaitForClosed(CB &&callback) {
    _WaitForClosed(pass<HciVoidCB>(std::forward<CB>(callback)));
  }

#ifndef HCI_EXPLICIT_IMPL
  /**
   * 等待 HciSdk 对象被关闭，异步操作
   */
  template <typename F, HCI_VALID_IF(func_traits<F>::is_func)>
  void WaitForClosed(F &&func) {
    auto cb = HciVoidCB::New<F>(std::forward<F>(func));
    _WaitForClosed(cb.pass());
  }

  /**
   * 等待 HciSdk 对象被关闭，异步操作
   */
  template <typename F, typename D, HCI_VALID_IF(func_traits<F>::value)>
  void WaitForClosed(F &&func, D &&data) {
    auto cb = HciVoidCB::New<F, D>(  //
        std::forward<F>(func), std::forward<D>(data));
    _WaitForClosed(cb.pass());
  }

  /**
   * 等待 HciSdk 对象被关闭，同步操作
   */
  void WaitForClosed() {
    if (in_loop_thread()) {
      hci::api::hci_abort("hci/sdk.h", __LINE__, "in loop thead!");
    }
    HciEvent event;
    WaitForClosed([&event]() { event.Set(); });
    event.Wait();
  }
#endif

  template <typename CB, HCI_VALID_IF(is_error_cb_<CB>::value)>
  void WaiForAuthReady(CB &&callback) {
    _WaitForAuthReady(pass<HciErrorCB>(std::forward<CB>(callback)));
  }
#ifndef HCI_EXPLICIT_IMPL
  template <typename F, HCI_VALID_IF(func_traits<F>::is_func)>
  void WaitForAuthReady(F &&func) {
    auto cb = HciErrorCB::New<F>(std::forward<F>(func));
    _WaitForAuthReady(cb.pass());
  }
  template <typename F, typename D>
  void WaitForAuthReady(F &&func, D &&data) {
    auto cb = HciErrorCB::New<F, D>(std::forward<F>(func),  //
                                    std::forward<D>(data));
    _WaitForAuthReady(cb.pass());
  }
  HciError WaitForAuthReady() {
    HciEvent event;
    WaitForAuthReady([&](HciError code) { event.Set(code); });
    return event.Wait();
  }
#endif

  template <typename CB, typename T, HCI_VALID_IF(is_http_req_cb_<CB>::value)>
  void HttpInvoke(CB &&callback, const HciHttpReq *req, T &&token) {
    _HttpInvoke(pass<HciHttpReqCB>(std::forward<CB>(callback)), req,
                pass<HciCancellationToken>(std::forward<T>(token)));
  }
#ifndef HCI_EXPLICIT_IMPL
  template <typename F, typename T, HCI_VALID_IF(func_traits<F>::is_func)>
  void HttpInvoke(F &&callback, const HciHttpReq *req, T &&token) {
    auto cb = HciHttpReqCB::New<F>(std::forward<F>(callback));
    auto tk = pass<HciCancellationToken>(std::forward<T>(token));
    _HttpInvoke(cb.pass(), req, tk);
  }
  template <typename F, typename D, typename T,
            HCI_VALID_IF(func_traits<F>::value)>
  void HttpInvoke(F &&func, D &&data, const HciHttpReq *req, T &&token) {
    auto cb = HciHttpReqCB::New<F, D>(std::forward<F>(func),  //
                                      std::forward<D>(data));
    auto tk = pass<HciCancellationToken>(std::forward<T>(token));
    _HttpInvoke(cb.pass(), req, tk);
  }
#endif

  template <typename CB, HCI_VALID_IF(is_http_req_cb_<CB>::value)>
  void HttpInvoke(CB &&callback, const HciHttpReq *req) {
    _HttpInvoke(pass<HciHttpReqCB>(std::forward<CB>(callback)), req,
                pass(nullptr));
  }
#ifndef HCI_EXPLICIT_IMPL
  template <typename F, HCI_VALID_IF(func_traits<F>::is_func)>
  void HttpInvoke(F &&func, const HciHttpReq *req) {
    auto cb = HciHttpReqCB::New<F>(std::forward<F>(func));
    _HttpInvoke(cb.pass(), req, pass(nullptr));
  }
  template <typename F, typename D, HCI_VALID_IF(func_traits<F>::value)>
  void HttpInvoke(F &&func, D &&data, const HciHttpReq *req) {
    auto cb = HciHttpReqCB::New<F, D>(std::forward<F>(func),  //
                                      std::forward<D>(data));
    _HttpInvoke(cb.pass(), req, pass(nullptr));
  }
#endif

  template <typename CB, HCI_VALID_IF(is_timer_cb_<CB>::value)>
  safe_ptr<HciTimer> NewTimer(CB &&callback) {
    return _NewTimer(pass<HciTimerCB>(std::forward<CB>(callback)));
  }

#ifndef HCI_EXPLICIT_IMPL
  template <typename F, HCI_VALID_IF(func_traits<F>::is_func)>
  safe_ptr<HciTimer> NewTimer(F &&func) {
    auto cb = HciTimerCB::New(std::forward<F>(func));
    return _NewTimer(cb.pass());
  }

  template <typename F, typename D>
  safe_ptr<HciTimer> NewTimer(F &&func, D &&data) {
    auto cb = HciTimerCB::New(std::forward<F>(func), std::forward<D>(data));
    return _NewTimer(cb.pass());
  }
#endif

  /**
   * 创建 WebSocket 连接
   */
  safe_ptr<WebSocket> NewWebSocket(const HciHttpReq *req,
                                   WebSocketHandler *handler);

 private:
  // 预留扩展用
  virtual void reserved1() = 0;
  virtual void reserved2() = 0;
  virtual void reserved3() = 0;
  virtual void reserved4() = 0;
};

HCI_INTERFACE_(HciSdk);

IS_HCI_PTR(is_sdk_, HciSdk);

template <size_t N>
struct cstr {
  char sz[N];
  const char *get() const { return sz; }
  operator const char *() const { return sz; }
  operator char *() { return sz; }
  char &operator[](int i) { return sz[i]; }
  char operator[](int i) const { return sz[i]; }
};

HCI_NS_END(hci, sdk);

#include <hci/sdk/session.h>
#include <hci/sdk/ws.h>

#endif  // HCI_SDK_H_
