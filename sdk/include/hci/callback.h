
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/callback.h
 *
 * 提供用于声明回调接口的宏，并声明 HciVoidCB 和 HciErrorCB 回调接口
 *
 * 公开接口:
 *
 *   - HCI_DECL_CALLBACK(name, result, ...)
 *
 *   - class HciVoidCB
 *   - class HciErrorCB
 *   - class HciAbortCB
 *
 *   - hci_reg_abort_cb
 *   - hci_unreg_abort_cb
 */
#ifndef HCI_CALLBACK_H_
#define HCI_CALLBACK_H_

#include <hci/error.h>
#include <hci/object.h>

/**
 * @def HCI_DECL_CALLBACK(name, result, ...)
 *
 * 声明一个名称为 @a name 的回调接口类
 *
 * 可通过 Invoke 方法调用回调接口，返值类型为 @a result，参数类型由宏中的
 * 变长参数 ... 指定。
 *
 * 类 @a name 中提供静态方法 New 用于创建回调接口对象实现。使用 New 方法
 * 需要先使用 HCI_IMPL_CALLBACK(name,result, ...) 宏。
 */
#define HCI_DECL_CALLBACK(name, result, ...)              \
  class HCI_LOCAL name : public ::hci::HciObject {        \
    HCI_INTERFACE(name);                                  \
                                                          \
   public:                                                \
    virtual result Invoke(__VA_ARGS__) = 0;               \
                                                          \
    template <typename F>                                 \
    static ::hci::safe_ptr<name> New(F &&func);           \
    template <typename F, typename D>                     \
    static ::hci::safe_ptr<name> New(F &&func, D &&data); \
  };                                                      \
                                                          \
  HCI_INTERFACE_(name);

namespace hci {
/**
 * HciVoidCB 回调接口
 *
 * 原型如下:
 *
 * @code
 *
 * class HCI_LOCAL HciVoidCB : public HciObject {
 *  public:
 *   virtual void Invoke() = 0;
 *
 *   template<typename F>
 *   static safe_ptr<HciVoidCB> New(F &&func);
 *   template<typename F, typename D>
 *   static safe_ptr<HcivoidCB> New(F &&func, D &&data);
 * };
 *
 * @endcode
 */
HCI_DECL_CALLBACK(HciVoidCB, void);

IS_HCI_PTR(is_void_cb_, HciVoidCB);

/**
 * HciErrorCB 回调接口
 *
 * 原型如下:
 *
 * @code {.cpp}
 *
 * class HCI_LOCAL HciErrorCB : public HciObject {
 *  public:
 *   virtual void Invoke(HciError) = 0;
 *
 *   template<typename F>
 *   static safe_ptr<HciErrorCB> New(F &&func);
 *   template<typename F, typename D>
 *   static safe_ptr<HciErrorCB> New(F &&func, D &&data);
 * };
 *
 * @endcode
 */
HCI_DECL_CALLBACK(HciErrorCB, void, HciError);

IS_HCI_PTR(is_error_cb_, HciErrorCB);

/**
 * HciAbortCB 回调接口
 *
 * 当调用 hci_abort, hci_abort2 时触发此回调，用户需要先向系统注册回调接口。
 * 使用 hci_reg_abort_cb, hci_unreg_abort_cb 进行 HciAbortCB 的注册和反注册
 *
 * 原型如下:
 *
 * @code {.cpp}
 *
 * class HCI_LOCAL HciAbortCB : public HciObject {
 *  public:
 *   virtual void Invoke(const char *file, int line, const char *message) = 0;
 *
 *   template<typename F>
 *   static safe_ptr<HciAbortCB> New(F &&func);
 *   template<typename F, typename D>
 *   static safe_ptr<HciAbortCB> New(F &&func, D &&data);
 * };
 *
 * @endcode
 */
HCI_DECL_CALLBACK(HciAbortCB, void, const char *, int, const char *);

IS_HCI_PTR(is_abort_cb_, HciAbortCB);

template <typename T, HCI_VALID_IF(is_abort_cb_<T>::value)>
static inline int hci_reg_abort_cb(T &&callback) {
  return api::_hci_reg_abort_cb(
      pass<HciAbortCB>(std::forward<T>(callback)).get());
}

static inline void hci_unreg_abort_cb(int cookie) {
  api::_hci_unreg_abort_cb(cookie);
}

}  // namespace hci

#if !defined(HCI_EXPLICIT_IMPL)
/**
 * 本条件编译区块中的内容有如下特点:
 *
 *   - 涉及大量的复杂模板，会大大地降低编译速度
 *   - 属于非必需内容，仅在需使用仿函数对象/函数指针/成员函数指针创建回调接
 *     口对象时，才需要使用。
 *
 * 通过 HCI_EXPLICT_IMPL 宏来控制是否启用本块内容
 *
 *   - 不定义此宏时启用，代价是降低编译速度。适合最终开发者使用的情况。
 *   - 定义此宏时禁用。适合内部项目中使用，熟悉本机制同时对编译速度也比较敏
 *     感的用户也可使用。
 *
 * 公开接口:
 *
 *   - HCI_IMPL_CALLBACK(name, result, ...)
 *
 *   - template<typename F>
 *     safe_ptr<HciVoidCB> HciVoidCB::New(F &&func);
 *   - template<typename F, typename D>
 *     safe_ptr<HciVoidCB> HciVoidCB::New(F &&func, D &&data);
 *   - template<typename F>
 *     safe_ptr<HciErrorCB> HciErrorCB::New(F &&func);
 *   - template<typename F, typename D>
 *     safe_ptr<HciErrorCB> HciErrorCB::New(F &&func, D &&data);
 */
#include <hci/new_object.h>
#include <hci/func_traits.h>

#include <utility>

// 返回值的 return 语句, x 为 void 展开为空，否则展开为 return
#define HCI_CALLBACK_RETURN(x) \
  HCI_CALLBACK_RETURN_((_, HCI_CAT(HCI_CALLBACK_return_, x), , return ))
#define HCI_CALLBACK_RETURN_(x) PP_A03 x
#define HCI_CALLBACK_return_void 0, 0

/**
 * @def HCI_IMPL_CALLBACK(name, result, ...)
 *
 * 为回调接口 @a name 提供一个实现，可使用仿函数对象/函数指针/成员函数指针
 * 创建 @a name 的对象实例。
 *
 * 使用时应确保与对应的 HCI_DECL_CALLBACK 使用相同的参数
 */
#define HCI_IMPL_CALLBACK(name, result, ...)                       \
  namespace internal {                                             \
                                                                   \
  template <typename FT>                                           \
  class HCI_LOCAL name##X : public name {                          \
    HCI_INTERFACE(name##X);                                        \
                                                                   \
   public:                                                         \
    typedef typename FT::func_type func_type;                      \
    template <typename F>                                          \
    name##X(F &&func) : func_(::std::forward<F>(func)) {}          \
                                                                   \
   protected:                                                      \
    func_type func_;                                               \
                                                                   \
    result Invoke(PP_EVAL0(PP_FP_ARGS_SN(__VA_ARGS__))) {          \
      HCI_CALLBACK_RETURN(result)                                  \
      func_(PP_EVAL0(PP_FC_ARGS_SN(__VA_ARGS__)));                 \
    }                                                              \
  };                                                               \
  template <typename FT>                                           \
  inline name##X<FT>::~name##X() {}                                \
                                                                   \
  template <typename FT>                                           \
  class HCI_LOCAL name##Y : public name {                          \
    HCI_INTERFACE(name##Y);                                        \
                                                                   \
   public:                                                         \
    typedef typename FT::func_type func_type;                      \
    typedef typename FT::class_type *data_type;                    \
    enum { arity = PP_EXPAND(PP_COUNT, (__VA_ARGS__)) };           \
                                                                   \
    static_assert(FT::arity == arity, /**/                         \
                  "invalid argument count");                       \
    template <typename F, typename D>                              \
    name##Y(F &&func, D &&data)                                    \
        : func_(::std::forward<F>(func)), /**/                     \
          data_(::std::forward<D>(data)) {}                        \
                                                                   \
   protected:                                                      \
    func_type func_;                                               \
    data_type data_;                                               \
                                                                   \
    result Invoke(PP_EVAL0(PP_FP_ARGS_SN(__VA_ARGS__))) {          \
      HCI_CALLBACK_RETURN(result)                                  \
      (data_->*func_)(PP_EVAL0(PP_FC_ARGS_SN(__VA_ARGS__)));       \
    }                                                              \
  };                                                               \
  template <typename FT>                                           \
  inline name##Y<FT>::~name##Y() {}                                \
                                                                   \
  template <typename FT, typename D>                               \
  class HCI_LOCAL name##Z : public name {                          \
    HCI_INTERFACE(name##Z);                                        \
                                                                   \
   public:                                                         \
    typedef typename FT::func_type func_type;                      \
    typedef typename ::hci::remove_ref<D>::type data_type;         \
    enum { arity = PP_EXPAND(PP_COUNT, (__VA_ARGS__)) };           \
                                                                   \
    static_assert(FT::arity == arity + 1, /**/                     \
                  "invalid argument count");                       \
    typedef typename FT::arg1_type arg1_type;                      \
    static_assert(!::hci::remove_ref<arg1_type>::is_rvr, /**/      \
                  "the first argument shall not be rv-ref");       \
    template <typename F>                                          \
    name##Z(F &&func, D &&data)                                    \
        : func_(::std::forward<F>(func)), /**/                     \
          data_(::std::forward<D>(data)) {}                        \
                                                                   \
   protected:                                                      \
    func_type func_;                                               \
    data_type data_;                                               \
                                                                   \
    result Invoke(PP_EVAL0(PP_FP_ARGS_SN(__VA_ARGS__))) {          \
      HCI_CALLBACK_RETURN(result)                                  \
      func_(data_ PP_EVAL0(PP_FC_ARGS_NN(__VA_ARGS__)));           \
    }                                                              \
  };                                                               \
  template <typename FT, typename D>                               \
  inline name##Z<FT, D>::~name##Z() {}                             \
                                                                   \
  template <typename FT, typename F, HCI_VALID_IF(FT::is_func)>    \
  static ::hci::safe_ptr<name> New##name(F &&func) {               \
    return ::hci::SafeNew<name##X<FT>>(std::forward<F>(func));     \
  }                                                                \
                                                                   \
  template <typename FT, typename F, typename D,                   \
            /* memfunc */ HCI_VALID_IF(FT::is_memfunc)>            \
  static ::hci::safe_ptr<name> New##name(F &&func, D &&data) {     \
    return /* use name##Y */ ::hci::SafeNew<name##Y<FT>>(          \
        std::forward<F>(func), std::forward<D>(data));             \
  }                                                                \
  template <typename FT, typename F, typename D,                   \
            /* not memfunc */ HCI_VALID_IF(!FT::is_memfunc)>       \
  static ::hci::safe_ptr<name> New##name(F &&func, D &&data) {     \
    return /* use name##Z */ ::hci::SafeNew<name##Z<FT, D>>(       \
        std::forward<F>(func), std::forward<D>(data));             \
  }                                                                \
  } /* namespace internal */                                       \
                                                                   \
  template <typename F>                                            \
  inline ::hci::safe_ptr<name> name::New(F &&func) {               \
    typedef ::hci::func_traits<F> ft;                              \
    static_assert(ft::is_functor || ft::is_funcptr,                \
                  "not a functor or function pointer");            \
    static_assert(ft::arity == PP_EXPAND(PP_COUNT, (__VA_ARGS__)), \
                  "inconsistent argument count");                  \
    return internal::New##name<ft, F>(std::forward<F>(func));      \
  }                                                                \
                                                                   \
  template <typename F, typename D>                                \
  inline ::hci::safe_ptr<name> name::New(F &&func, D &&data) {     \
    typedef ::hci::func_traits<F> ft;                              \
    static_assert(ft::value, "not a function type");               \
    return /*********************/ internal::New##name<ft, F, D>(  \
        std::forward<F>(func), ::std::forward<D>(data));           \
  }

namespace hci {
// HciVoidCB 回调接口实现
HCI_IMPL_CALLBACK(HciVoidCB, void);
// HciErrorCB 回调接口实现
HCI_IMPL_CALLBACK(HciErrorCB, void, HciError);
// HciAbortCB 回调接口实现
HCI_IMPL_CALLBACK(HciAbortCB, void, const char *, int, const char *);

template <typename F, HCI_VALID_IF(func_traits<F>::is_func)>
static inline int hci_reg_abort_cb(F &&func) {
  auto cb = HciAbortCB::New<F>(std::forward<F>(func));
  return api::_hci_reg_abort_cb(cb.pass().get());
}

template <typename F, typename D, HCI_VALID_IF(func_traits<F>::value)>
static inline int hci_reg_abort_cb(F &&func, D &&data) {
  auto cb = HciAbortCB::New<F, D>(std::forward<F>(func), std::forward<D>(data));
  return api::_hci_reg_abort_cb(cb.pass().get());
}

}  // namespace hci

#endif  // !defined(HCI_EXPLICIT_IMPL)

#endif  // HCI_CALLBACK_H_
