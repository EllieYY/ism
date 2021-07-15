
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/hci.h
 */
#ifndef HCI_HCI_H_
#define HCI_HCI_H_

#ifndef __cplusplus
#error "NOT A C++ COMPILER!!"
#endif

#if defined(_MSC_VER)
#include <hci/compiler/msvc.h>
#elif defined(__clang_major__)
#include <hci/compiler/clang.h>
#elif defined(__GNUC__)
#include <hci/compiler/gcc.h>
#else
static_assert(false, "unknown compiler!!");
#endif

#include <ctype.h>
#include <cstddef>

#if defined(_MSC_VER)
#if defined(BUILD_HCI_COMMON) || defined(STATIC_HCI_COMMON)
#define HCI_COMMON_API
#else
#define HCI_COMMON_API HCI_IMPORT
#endif
#else
#if defined(BUILD_HCI_COMMON)
#define HCI_COMMON_API HCI_EXPORT
#else
#define HCI_COMMON_API HCI_IMPORT
#endif
#endif

// 内存分配和释放会使用如下方法
//
//   HCI_ALLOCATOR::malloc
//   HCI_ALLOCATOR::free
//   HCI_ALLOCATOR::realloc
//
// 未定义 HCI_ALLOCATOR 的话，会将其定义成空内容，此时内存分配和释放使用
// 的是全局的 ::malloc, ::free, ::realloc
#ifndef HCI_ALLOCATOR
#define HCI_ALLOCATOR
#endif

#ifndef HCI_DEPRECATED0
#define HCI_DEPRECATED0
#endif

#ifndef HCI_DEPRECATED1
#if _MSC_VER
#define HCI_DEPRECATED1 __declspec(deprecated)
#else
#define HCI_DEPRECATED1 __attribute__((deprecated))
#endif
#endif

#include <hci/pp.h>

#define HCI_CAT PP_CAT
#define HCI_NS_BEGIN PP_NS_BEGIN
#define HCI_NS_END PP_NS_END

#define HCI_INTERFACE(name)               \
 public:                                  \
  name() = default;                       \
                                          \
 protected:                               \
  virtual ~name() = 0;                    \
                                          \
 private:                                 \
  name(const name &) = delete;            \
  name(name &&) = delete;                 \
  name &operator=(const name &) = delete; \
  name &operator=(name &&) = delete

#define HCI_INTERFACE_(name) inline name::~name() = default;

#define HCI_ENUM(name, type)                      \
  static inline type operator*(name e) { /**/     \
    return static_cast<type>(e);                  \
  }                                               \
  static inline bool operator==(type a, name e) { \
    return a == static_cast<int>(e);              \
  }                                               \
  static inline bool operator==(name e, type a) { \
    return a == static_cast<type>(e);             \
  }                                               \
  static inline bool operator!=(type a, name e) { \
    return a != static_cast<type>(e);             \
  }                                               \
  static inline bool operator!=(name e, type a) { \
    return a != static_cast<type>(e);             \
  }

#define __HCI_ENUM_NCASE(T, V) \
  case T::V:                   \
    return #V;

#define __HCI_ENUM_VCASE(T, V) \
  if (strcmp(n, #V) == 0) return (void)(*v = T::V);

#define HCI_ENUM_NAME(type, ...)                     \
  inline const char *nameOf(type v) {                \
    switch (v) {                                     \
      PP_KEACH(__HCI_ENUM_NCASE, type, __VA_ARGS__); \
      default:                                       \
        return nullptr;                              \
    }                                                \
  }                                                  \
  inline void valueOf(const char *n, type *v) {      \
    PP_KEACH(__HCI_ENUM_VCASE, type, __VA_ARGS__);   \
  }

namespace hci {

template <typename T, typename... Args>
struct is_one_of {
  static constexpr bool value = false;
};

template <typename T, typename... Args>
struct is_one_of<T, T, Args...> {
  static constexpr bool value = true;
};

template <typename T, typename A, typename... Args>
struct is_one_of<T, A, Args...> {
  static constexpr bool value = false;
};

template <bool>
struct void_if_true;

// 用于 SFINAE
template <>
struct void_if_true<false> {
  typedef bool type;  // 模板类特化 - unmatch case
};

// 用于 SFINAE
template <>
struct void_if_true<true> {
  typedef void type;  // 模板类特化 - match case
};

/**
 * @def HCI_VOID_T(...)
 *
 * 用于模板类特化。主模板声明中的相应模板参数应默认为 void，当 __VA_ARGS__
 * 为真时推导为 void 与主模板匹配从而特化成功。
 */
#define HCI_VOID_T(...) typename ::hci::void_if_true<__VA_ARGS__>::type

#define HCI_HAS_TYPE(T, type) \
  HCI_VOID_T(::hci::always_true<typename T::type>::value)

template <class T, class = int, class = int, class = int>
struct always_true {
  static constexpr bool value = true;
};

template <bool cond, typename T>
struct enable_if_ {};
template <typename T>
struct enable_if_<true, T> {
  typedef T type;
};

/**
 * @def HCI_VALID_IF(cond)
 *
 * 一般用于函数模板的最后一个默认模板参数，使得函数模板仅适用于指定的条件
 */
#define HCI_VALID_IF(...) \
  typename ::hci::enable_if_<__VA_ARGS__, void *>::type = nullptr

template <typename T1, typename T2>
struct is_same_ {
  static constexpr bool value = false;
};

template <typename T>
struct is_same_<T, T> {
  static constexpr bool value = true;
};

template <typename T>
struct remove_ref {
  typedef T type;
  static constexpr bool is_ref = false;
  static constexpr bool is_lvr = false;
  static constexpr bool is_rvr = false;
};

template <typename T>
struct remove_ref<T &> {
  typedef T type;
  static constexpr bool is_ref = true;
  static constexpr bool is_lvr = true;
  static constexpr bool is_rvr = false;
};

template <typename T>
struct remove_ref<T &&> {
  typedef T type;
  static constexpr bool is_ref = true;
  static constexpr bool is_lvr = false;
  static constexpr bool is_rvr = true;
};

// std::is_base_of 要求 Derrived 是一个定义完整的类，而这个版本没有此要求
template <typename Base, typename Derrived>
struct is_base_of_ {
  typedef char (&yes)[4];
  typedef char (&no)[8];
  struct Host {
    operator Base *() const;
    operator Derrived *();
  };
  template <typename T>
  static yes check(Derrived *, T);
  static no check(Base *, int);

  static const bool value = sizeof(check(Host(), int())) == sizeof(yes);
};

template <typename T>
static inline T declval() {
  return T();
}

// 判断类型 T 是否是一个 functor
template <typename T>
struct is_functor_ {
  typedef char (&yes)[4];
  typedef char (&no)[8];

  template <typename U>
  static yes check(decltype(&U::operator()));
  template <typename U>
  static no check(...);

  static const bool value = sizeof(check<T>(nullptr)) == sizeof(yes);
};

#define HCI_ABORT(fmt, ...)                                              \
  PP_NULL(::hci::api::hci_abort, ::hci::api::hci_abort2, __VA_ARGS__, ~) \
  (__FILE__, __LINE__, fmt, ##__VA_ARGS__);

// 不区分大小写的字符串比较 (转换为小写字母进行比较)
static inline int strcasecmp(const char *s1, const char *s2) {
  for (;;) {
    int a = (*s1++) & 0xff, b = (*s2++) & 0xff;
    if (!a || !b) return a - b;
    a |= isalpha(a) ? 0x20 : 0x00;
    b |= isalpha(b) ? 0x20 : 0x00;
    int res = a - b;
    if (res != 0) return res;
  }
}

// 不区分大小写的字符串比较 (转换为小写字母进行比较)
static inline int strncasecmp(const char *s1, const char *s2, size_t n) {
  while (n-- > 0) {
    int a = (*s1++) & 0xff, b = (*s2++) & 0xff;
    if (!a || !b) return a - b;
    a |= isalpha(a) ? 0x20 : 0x00;
    b |= isalpha(b) ? 0x20 : 0x00;
    int res = a - b;
    if (res != 0) return res;
  }
  return 0;
}

}  // namespace hci

#include <hci/api.h>

#endif  // HCI_HCI_H_
