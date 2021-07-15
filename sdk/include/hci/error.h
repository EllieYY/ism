
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=120 ff=unix :

/**
 * @file hci/error.h
 *
 * 错误码定义及相关内容
 */
#ifndef HCI_ERROR_H_
#define HCI_ERROR_H_

#include <hci/object.h>

namespace hci {

enum struct HciError : int {
  /**
   * 不是错误，表示操作成功，在成功时返回
   */
  OK = 0,
  /**
   * 操作被取消，一般是调用者引起的
   */
  CANCELLED = 1,
  /**
   * 未知错误
   */
  UNKNOWN = 2,
  /**
   * 非法参数。本错误码与 `FAILED_PRECONDITION`
   * 的区别在于，输入参数错误与系统 状态无关。
   */
  INVALID_ARGUMENT = 3,
  /**
   * 请求完成之前，到达了操作截止时间。对于修改系统状态的操作，即使操作完成了，
   * 也可能会返回此错误，主要是因为响应被延迟导致截止时间超时。
   */
  DEADLINE_EXCEEDED = 4,
  /**
   * 找不到请求的实体
   */
  NOT_FOUND = 5,
  /**
   * 要创建的实体已存在
   */
  ALREADY_EXISTS = 6,
  /**
   * 调用者没有权限执行操作。
   * 本错误码未隐含这些信息：请求合法，或者请求的资源存在，满足前置条件
   */
  PERMISSION_DENIED = 7,
  /**
   * 没有有效的身份认证
   */
  UNAUTHENTICATED = 16,
  /**
   * 资源被耗尽
   */
  RESOURCE_EXHAUSTED = 8,
  /**
   * 因为系统不具备前置条件导致操作被拒绝。
   *
   * 可根据以下原则来选择`FAILED_PRECONDITION`, `ABORTED`, and
   * `UNAVAILABLE`
   *
   *   - 可以让调用者尝试再次调用的情况下，请使用 `UNAVAILABLE`
   *   - 可以让调用者在更高层次尝试调用的情况下，请使用
   * `ABORTED`。(例如：重新 尝试`读取, 修改, 写入` 操作序列)
   *   - 调用者不应重试直到系统状态被修复的情况下，请使用
   * `FAILED_PRECONDITION`
   */
  FAILED_PRECONDITION = 9,
  /**
   * 操作中止。通常是因为并发问题或者事务被中止
   */
  ABORTED = 10,
  /**
   * 操作超出了有效范围
   */
  OUT_OF_RANGE = 11,
  /**
   * 未实现的操作。不支持的功能或者未启动的功能。
   */
  UNIMPLEMENTED = 12,
  /**
   * 内部错误。这个错误意味着系统依赖的固定因素发生了改变，此错误码用于严重错误
   */
  INTERNAL = 13,
  /**
   * 当前处于不可用的状态。大多数情况下是个暂时性的错误，可通过后续的尝试修复
   */
  UNAVAILABLE = 14,
  /**
   * 不可恢复的数据丢失或损坏
   */
  DATA_LOSS = 15,
};

HCI_ENUM(HciError, int);

/**
 * 将 HTTP 状态码转换为 HciError
 *
 * 参考: https://developer.mozilla.org/en-US/docs/Web/HTTP/Status
 */
static inline HciError http_status_to_error(int status) {
  if (status == 200)  // 200 OK
    return HciError::OK;
  if (status == 400)  // 400 Bad Request
    return HciError::INVALID_ARGUMENT;
  if (status == 401)  // 401 Unauthorized
    return HciError::UNAUTHENTICATED;
  if (status == 403)  // 403 Forbidden
    return HciError::PERMISSION_DENIED;
  if (status == 404)  // 404 Not Found
    return HciError::NOT_FOUND;
  if (status == 408)  // 408 Request Timeout
    return HciError::DEADLINE_EXCEEDED;
  if (status == 410)  // 410 Gone
    return HciError::NOT_FOUND;
  if (status == 412)  // 412 Precondition Failed
    return HciError::FAILED_PRECONDITION;
  if (status == 416)  // 416 Range Not Satisfiable
    return HciError::OUT_OF_RANGE;
  if (status == 429)  // 429 Too Many Requests
    return HciError::PERMISSION_DENIED;
  if (status == 500)  // 500 Internal Server Error
    return HciError::INTERNAL;
  if (status == 501)  // 501 Not Implemented
    return HciError::UNIMPLEMENTED;
  if (status == 503)  // 503 Service Unavailable
    return HciError::UNAVAILABLE;
  if (status == 504)  // 504 Gateway Timeout
    return HciError::DEADLINE_EXCEEDED;
  if (status == 505)  // 505 HTTP Version Not Supported
    return HciError::UNIMPLEMENTED;
  return HciError::UNKNOWN;
}

};  // namespace hci

#endif  // HCI_ERROR_H_
