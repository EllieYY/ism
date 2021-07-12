
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/sdk/ws.h
 *
 * 灵云 SDK WebSocket 对象
 */
#ifndef HCI_SDK_WS_H_
#define HCI_SDK_WS_H_

#include <hci/sdk/http.h>
#include <hci/sdk/event.h>

HCI_NS_BEGIN(hci, sdk);

class HCI_LOCAL WebSocketHandler;

namespace ws {

/**
 * WebSocket 状态
 */
enum struct State : uint8_t {
  CONNECTING = 0,  // 连接中
  OPEN = 1,        // 已连接
  CLOSING = 2,     // 关闭中
  CLOSED = 3,      // 已关闭
};

HCI_ENUM(State, uint8_t);

/**
 * WebSocket 关闭状态码
 *
 * 详见: https://tools.ietf.org/html/rfc6455#section-7.4.1
 */
enum struct StatusCode : uint16_t {
  /**
   * indicates a normal closure, meaning that the purpose for
   * which the connection was established has been fulfilled.
   */
  NORMAL_CLOSURE = 1000,
  /**
   * indicates that an endpoint is "going away", such as a server
   * going down or a browser having navigated away from a page.
   */
  GOING_AWAY = 1001,
  /**
   * indicates that an endpoint is terminating the connection due
   * to a protocol error.
   */
  PROTOCOL_ERROR = 1002,
  /**
   * indicates that an endpoint is terminating the connection
   * because it has received a type of data it cannot accept (e.g., an
   * endpoint that understands only text data MAY send this if it
   * receives a binary message)
   */
  UNSUPPORTED_DATA = 1003,
  /**
   * 1005 is a reserved value and MUST NOT be set as a status code in a
   * Close control frame by an endpoint.  It is designated for use in
   * applications expecting a status code to indicate that no status
   * code was actually present.
   */
  NO_STATUS_RECEIVED = 1005,
  /**
   * 1006 is a reserved value and MUST NOT be set as a status code in a
   * Close control frame by an endpoint.  It is designated for use in
   * applications expecting a status code to indicate that the
   * connection was closed abnormally, e.g., without sending or
   * receiving a Close control frame.
   */
  ABNORMAL_CLOSURE = 1006,
  /**
   * 1007 indicates that an endpoint is terminating the connection
   * because it has received data within a message that was not
   * consistent with the type of the message (e.g., non-UTF-8 [RFC3629]
   * data within a text message).
   */
  INVALID_PAYLOAD = 1007,
  /**
   * 1008 indicates that an endpoint is terminating the connection
   * because it has received a message that violates its policy.  This
   * is a generic status code that can be returned when there is no
   * other more suitable status code (e.g., 1003 or 1009) or if there
   * is a need to hide specific details about the policy.
   */
  POLICY_VIOLATION = 1008,
  /**
   * 1009 indicates that an endpoint is terminating the connection
   * because it has received a message that is too big for it to
   * process.
   */
  MESSAGE_TOO_BIG = 1009,
  /**
   * 1010 indicates that an endpoint (client) is terminating the
   * connection because it has expected the server to negotiate one or
   * more extension, but the server didn't return them in the response
   * message of the WebSocket handshake.  The list of extensions that
   * are needed SHOULD appear in the /reason/ part of the Close frame.
   * Note that this status code is not used by the server, because it
   * can fail the WebSocket handshake instead.
   */
  MANDATORY_EXT = 1010,
  /**
   * 1011 indicates that a server is terminating the connection because
   * it encountered an unexpected condition that prevented it from
   * fulfilling the request.
   */
  INTERNAL_SERVER_ERROR = 1011,
  /**
   * 1015 is a reserved value and MUST NOT be set as a status code in a
   * Close control frame by an endpoint.  It is designated for use in
   * applications expecting a status code to indicate that the
   * connection was closed due to a failure to perform a TLS handshake
   * (e.g., the server certificate can't be verified).
   */
  TLS_HANDSHAKE = 1015,
};

HCI_ENUM(StatusCode, uint16_t);

};  // namespace ws

/**
 * WebSocket 连接
 */
class HCI_LOCAL WebSocket : public HciObject {
  HCI_INTERFACE(WebSocket);

 public:
  virtual ws::State state() const = 0;  // WebSocket 当前状态
  virtual const char *uri() const = 0;  // WebSocket 连接的 uri
  virtual HciSdk *sdk() const = 0;      // HciSdk 对象指针
  virtual bool paused() const = 0;      // 数据接收是否处于暂停状态
  virtual void set_paused(bool paused) = 0;  // 设置数据接收的暂停状态

  virtual void Close() = 0;  // 关闭连接

  /**
   * 发送文本消息
   *
   *   @param [in]  text  要发送的文本消息
   *   @return 错误码
   *
   *     - HciError::FAILED_PRECONDITION  在非 OPEN 状态下调用
   *     - HciError::OK                   数据已放入发送队列
   *     - HciError::UNAVAILABLE          因发送队列满失败
   */
  virtual HciError SendText(const char *text) = 0;

  /**
   * 发送数据消息
   *
   *   @param [in]  data 待发送数据的内存地址
   *   @param [in]  size  待发送数据的长度
   *   @return 错误码
   *
   *     - HciError::FAILED_PRECONDITION  在非 OPEN 状态下调用
   *     - HciError::OK                   数据已放入发送队列
   *   -   HciError::UNAVAILABLE          因发送队列满失败
   */
  virtual HciError SendData(const void *data, size_t size) = 0;

  /**
   * 等待 WebSocket 可写 (内部方法，请使用对应的模板方法 WaitForWritable)
   *
   *   异步回调会在三种情况下发生：
   *
   *     - 非 OPEN 状态下调用本方法，回调错误码为 HciError::FAILED_PRECONDITION
   *     - WebSocket 变为可写时，回调错误码为 HciError::OK
   *     - WebSocket 进入 CLOSED 状态时，回调错误码为 HciError::OK。本异步回调会
   *       发生在 WebSocketHandler::OnClose 之前
   */
  virtual void _WaitForWritable(pass_ptr<HciErrorCB> callback) = 0;

  template <typename T, HCI_VALID_IF(is_error_cb_<T>::value)>
  void WaitForWritable(T &&callback) {
    WaitForWritable(pass<HciErrorCB>(std::forward<T>(callback)));
  }
#ifndef HCI_EXPLICIT_IMPL
  template <typename F, HCI_VALID_IF(func_traits<F>::is_func)>
  void WaitForWritable(F &&func) {
    auto cb = HciErrorCB::New<F>(std::forward<F>(func));
    _WaitForWritable(cb.pass());
  }
  template <typename F, typename D, HCI_VALID_IF(func_traits<F>::value)>
  void WaitForWritable(F &&func, D &&data) {
    auto cb = HciErrorCB::New<F, D>(  //
        std::forward<F>(func), std::forward<D>(data));
    _WaitForWritable(cb.pass());
  }
  /**
   * 等待 WebSocket 可写，同步方法，不可在事件循环线程调用，否则会发生死锁
   */
  HciError WaitForWritable() {
    if (sdk()->in_loop_thread()) {
      hci::api::hci_abort("hci/sdk/ws.h", __LINE__, "in loop thread");
    }
    HciEvent event;
    WaitForWritable([&](HciError code) { event.Set(code); });
    return event.Wait();
  }
#endif
};

HCI_INTERFACE_(WebSocket);

/**
 * WebSocket 回调接口
 *
 *   - 连接无法建立时，进入 CLOSED 状态并调用 OnOpen
 *   - 连接建立成功后，进入 OPEN 状态并调用 OnOpen
 *   - CONNECTING 状态下调用 Close，进入 CLOSED 状态并调用 OnOpen
 *   - OPEN 状态下收到对端消息时会调用 OnText 或 OnData
 *   - OPEN 状态下主动关闭连接，进入 CLOSING 状态并调用 OnClosing。此时仍可
 *     接收对端消息并发生 OnText 和 OnData 调用；在收到对端的关闭响应后，进
 *     入 CLOSED 状态并调用 OnClose
 *   - OPEN 状态下收到对端关闭请求时，进入 CLOSING 状态并调用 OnClosing，不
 *     再接收对端数据，发送关闭响应后进入 CLOSED 状态并调用 OnClose
 *   - OPEN 状态下底层连接意外断开时，不再发送和接收数据，马上进入 CLOSING
 *     状态并调用 OnClosing，然后马上进入 CLOSED 状态并调用 OnClose
 *
 * 所有回调中都不能执行需要长时间阻塞的任务，均应尽快返回
 */
class HCI_LOCAL WebSocketHandler {
 public:
  virtual ~WebSocketHandler() = 0;

  virtual void OnOpen(WebSocket *ws, HciError code) = 0;
  virtual void OnClosing(WebSocket *ws, ws::StatusCode code) = 0;
  virtual void OnClose(WebSocket *ws) = 0;
  virtual void OnText(WebSocket *ws, const char *text) = 0;
  virtual void OnData(WebSocket *ws, const void *data, size_t size) = 0;
};

inline WebSocketHandler::~WebSocketHandler() = default;

inline safe_ptr<WebSocket> HciSdk::NewWebSocket(const HciHttpReq *req,
                                                WebSocketHandler *handler) {
  return _NewWebSocket(req, handler).safe();
}

HCI_NS_END(hci, sdk);

#endif  // HCI_SDK_WS_H_
