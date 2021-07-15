
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/sdk/log.h
 *
 * 灵云 SDK 日志接口
 */
#ifndef HCI_SDK_LOG_H_
#define HCI_SDK_LOG_H_

HCI_NS_BEGIN(hci, sdk);

class HCI_LOCAL HciSdk;

enum struct LogLevel {
  N = 0,  // Never
  E = 1,  // Error
  W = 2,  // Warn
  I = 3,  // Info
  D = 4,  // Debug
  T = 5,  // Trace
};

HCI_ENUM(LogLevel, int);

/**
 * @def __HciLog(psdk, level, ...)
 *
 * 输出级别为 level 的日志，含文件名和行号
 */
#define __HciLog(psdk, level, ...)                                             \
  if ((psdk)->log_level() >= ::hci::sdk::LogLevel::level) {                    \
    (psdk)->Log(::hci::sdk::LogLevel::level, __FILE__, __LINE__, __VA_ARGS__); \
  }

/**
 * @def __HciLogE(psdk, ...)
 *
 * 输出级别为 Error 的日志，含文件名和行号
 */
#define __HciLogE(psdk, ...)                                                   \
  PP_EVAL0(PP_NULL(HciLogE::MISSING_fmt, __HciLog, __VA_ARGS__, ) /* NOLINT */ \
           (psdk, E, ##__VA_ARGS__))

/**
 * @def __HciLogW(psdk, ...)
 *
 * 输出级别为 Warn 的日志，含文件名和行号
 */
// 输出 Warn 级别日志，包含文件名和行号
#define __HciLogW(psdk, ...)                                                  \
  PP_EVAL0(PP_NULL(HciLogW::MISSING_fmt, __HciLog, __VA_ARGS__, ) /*NOLINT */ \
           (psdk, W, ##__VA_ARGS__))

/**
 * @def __HciLogI(psdk, ...)
 *
 * 输出级别为 Info 的日志，含文件名和行号
 */
#define __HciLogI(psdk, ...)                                         \
  PP_EVAL0(PP_NULL(HciLogI::MISSING_fmt, __HciLog, __VA_ARGS__) /**/ \
           (psdk, I, ##__VA_ARGS__))

/**
 * @def __HciLogD(psdk, ...)
 *
 * 输出级别为 Debug 的日志，含文件名和行号
 */
#define __HciLogD(psdk, ...)                                         \
  PP_EVAL0(PP_NULL(HciLogD::MISSING_fmt, __HciLog, __VA_ARGS__) /**/ \
           (psdk, D, ##__VA_ARGS__))

/**
 * @def __HciLogT(psdk, ...)
 *
 * 输出级别为 Trace 的日志，含文件名和行号。在 Release 模式下不起作用。
 */
#ifdef NDEBUG
#define __HciLogT(psdk, ...)
#else
#define __HciLogT(psdk, ...)                                         \
  PP_EVAL0(PP_NULL(HciLogV::MISSING_fmt, __HciLog, __VA_ARGS__) /**/ \
           (psdk, T, ##__VA_ARGS__))
#endif

/**
 * @def HciLog(psdk, level, ...)
 *
 * 输出级别为 level 的日志，不含文件名和行号
 */
#define HciLog(psdk, level, ...)                                       \
  if ((psdk)->log_level() >= ::hci::sdk::LogLevel::level) {            \
    (psdk)->Log(::hci::sdk::LogLevel::level, nullptr, 0, __VA_ARGS__); \
  }

/**
 * @def HciLogE(psdk, ...)
 *
 * 输出级别为 Error 的日志，不含文件名和行号
 */
#define HciLogE(psdk, ...)                                                 \
  PP_EVAL0(PP_NULL(HciLogE::MISSING_fmt, HciLog, __VA_ARGS__, ) /*NOLINT*/ \
           (psdk, E, ##__VA_ARGS__))

/**
 * @def HciLogW(psdk, ...)
 *
 * 输出级别为 Warn 的日志，不含文件名和行号
 */
#define HciLogW(psdk, ...)                                                 \
  PP_EVAL0(PP_NULL(HciLogW::MISSING_fmt, HciLog, __VA_ARGS__, ) /*NOLINT*/ \
           (psdk, W, ##__VA_ARGS__))

/**
 * @def HciLogI(psdk, ...)
 *
 * 输出级别为 Info 的日志，不含文件名和行号
 */
#define HciLogI(psdk, ...)                                         \
  PP_EVAL0(PP_NULL(HciLogI::MISSING_fmt, HciLog, __VA_ARGS__) /**/ \
           (psdk, I, ##__VA_ARGS__))

/**
 * @def HciLogD(psdk, ...)
 *
 * 输出级别为 Debug 的日志，不含文件名和行号
 */
#define HciLogD(psdk, ...)                                         \
  PP_EVAL0(PP_NULL(HciLogD::MISSING_fmt, HciLog, __VA_ARGS__) /**/ \
           (psdk, D, ##__VA_ARGS__))

/**
 * @def HciLogT(psdk, ...)
 *
 * 输出级别为 Trace 的日志，不含文件名和行号。在 Release 模式下不起作用。
 */
#ifdef NDEBUG
#define HciLogT(psdk, ...)
#else
#define HciLogT(psdk, ...)                                         \
  PP_EVAL0(PP_NULL(HciLogV::MISSING_fmt, HciLog, __VA_ARGS__) /**/ \
           (psdk, T, ##__VA_ARGS__))
#endif

HCI_NS_END(hci, sdk);

#endif  // HCI_SDK_LOG_H_
