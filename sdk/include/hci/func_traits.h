
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/func_traits.h
 *
 * 定义 C++ Function Traits (hci::func_traits) 用于获取仿函数或函数指针的
 * 返值类型，参数个数和参数类型
 */
#ifndef HCI_FUNC_TRAITS_H_
#define HCI_FUNC_TRAITS_H_

#include <hci/hci.h>

namespace hci {

template <typename>
struct func_traits_helper {
  static constexpr int arity = -1;
  static constexpr bool is_functor = false;  // 是否为仿函数
  static constexpr bool is_funcptr = false;  // 是否为函数指针
  static constexpr bool is_memfunc = false;  // 是否为成员函数指针

  // is_functor || is_funcptr
  static constexpr bool is_func = false;

  // is_functor || is_funcptr || is_memfunc
  static constexpr bool value = false;
};

#define HCI_FTH_AT(i, x) typedef x PP_CAT(arg, i, _type);

#define HCI_FTH_E()                             \
  template <class R>                            \
  struct func_traits_helper<R()> {              \
    typedef R result_type;                      \
    typedef R (*func_type)();                   \
    static constexpr int arity = 0;             \
    static constexpr bool is_functor = false;   \
    static constexpr bool is_funcptr = true;    \
    static constexpr bool is_memfunc = false;   \
    static constexpr bool is_func = true;       \
    static constexpr bool value = true;         \
  };                                            \
  template <class R, class C>                   \
  struct func_traits_helper<R (C::*)()> {       \
    typedef R result_type;                      \
    typedef C class_type;                       \
    typedef R (C::*func_type)();                \
    static constexpr int arity = 0;             \
    static constexpr bool is_functor = false;   \
    static constexpr bool is_funcptr = false;   \
    static constexpr bool is_memfunc = true;    \
    static constexpr bool is_func = true;       \
    static constexpr bool is_const = false;     \
    static constexpr bool value = true;         \
  };                                            \
  template <class R, class C>                   \
  struct func_traits_helper<R (C::*)() const> { \
    typedef R result_type;                      \
    typedef C class_type;                       \
    typedef R (C::*func_type)() const;          \
    static constexpr int arity = 0;             \
    static constexpr bool is_functor = false;   \
    static constexpr bool is_funcptr = false;   \
    static constexpr bool is_memfunc = true;    \
    static constexpr bool is_func = true;       \
    static constexpr bool is_const = true;      \
    static constexpr bool value = true;         \
  }

#define HCI_FTH_NE(...)                                    \
  template <class R PP_TA_ARGS_NN(__VA_ARGS__)>            \
  struct func_traits_helper<R(__VA_ARGS__)> {              \
    typedef R result_type;                                 \
    typedef R (*func_type)(__VA_ARGS__);                   \
    PP_IEACH_(HCI_FTH_AT, __VA_ARGS__, ~) /**/             \
    static constexpr int arity = PP_COUNT(__VA_ARGS__);    \
    static constexpr bool is_functor = false;              \
    static constexpr bool is_funcptr = true;               \
    static constexpr bool is_memfunc = false;              \
    static constexpr bool is_func = true;                  \
    static constexpr bool value = true;                    \
  };                                                       \
  template <class R, class C PP_TA_ARGS_NN(__VA_ARGS__)>   \
  struct func_traits_helper<R (C::*)(__VA_ARGS__)> {       \
    typedef R result_type;                                 \
    typedef C class_type;                                  \
    typedef R (C::*func_type)(__VA_ARGS__);                \
    PP_IEACH_(HCI_FTH_AT, __VA_ARGS__, ~) /**/             \
    static constexpr int arity = PP_COUNT(__VA_ARGS__);    \
    static constexpr bool is_functor = false;              \
    static constexpr bool is_funcptr = false;              \
    static constexpr bool is_memfunc = true;               \
    static constexpr bool is_func = false;                 \
    static constexpr bool is_const = false;                \
    static constexpr bool value = true;                    \
  };                                                       \
  template <class R, class C PP_TA_ARGS_NN(__VA_ARGS__)>   \
  struct func_traits_helper<R (C::*)(__VA_ARGS__) const> { \
    typedef R result_type;                                 \
    typedef C class_type;                                  \
    typedef R (C::*func_type)(__VA_ARGS__) const;          \
    PP_IEACH_(HCI_FTH_AT, __VA_ARGS__, ~) /**/             \
    static constexpr int arity = PP_COUNT(__VA_ARGS__);    \
    static constexpr bool is_functor = false;              \
    static constexpr bool is_funcptr = false;              \
    static constexpr bool is_memfunc = true;               \
    static constexpr bool is_func = false;                 \
    static constexpr bool is_const = true;                 \
    static constexpr bool value = true;                    \
  }

#define HCI_FTH(...) PP_NULL(HCI_FTH_E, HCI_FTH_NE, __VA_ARGS__, ~)(__VA_ARGS__)

PP_VARIADIC_TEMPLS(10, HCI_FTH);

#undef HCI_FTH_AT
#undef HCI_FTH

/**
 * func_traits
 */
template <typename T, typename = void>
struct func_traits : func_traits_helper<T> {};

template <typename T>
struct func_traits<T &> : func_traits<T> {};
template <typename T>
struct func_traits<T &&> : func_traits<T> {};
template <typename T>
struct func_traits<T *> : func_traits<T> {};

template <typename T>
struct func_traits<T, HCI_VOID_T(is_functor_<T>::value)>
    : func_traits_helper<decltype(&T::operator())> {
  typedef T func_type;
  static constexpr bool is_functor = true;
  static constexpr bool is_memfunc = false;
  static constexpr bool is_func = true;
  static constexpr bool value = true;
};
}  // namespace hci
#endif  // HCI_FUNC_TRAITS_H_
