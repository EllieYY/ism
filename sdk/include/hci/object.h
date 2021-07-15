
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/object.h
 *
 * HciObject 类及相关内容
 *
 * 公开接口
 *
 *   - class HciObject
 *
 *   - template<typename> struct pass_ptr;
 *   - template<typename> class safe_ptr;
 *   - template<typename> class weak_ptr;
 *
 *   - template<typename> struct is_object_;
 *   - template<typename> struct is_object_ptr_;
 *
 *   - template<typename T>
 *     pass_ptr<T> wrap(T *);
 *   - template<typename T>
 *     pass_ptr<T> pass(safe_ptr<T> &&);
 *   - template<typename T>
 *     pass_ptr<T> pass(const safe_ptr<T> &);
 *   - template<typename T>
 *     pass_ptr<T> pass(pass_ptr<T> &&);
 *   - template<typename T>
 *     pass_ptr<T> pass(pass_ptr<T> &);
 *   - template<typename T>
 *     pass_ptr<T> pass(T *);
 *   - pass_ptr<std::nullptr_t> pass(std::nullptr_t);
 */
#ifndef HCI_OBJECT_H_
#define HCI_OBJECT_H_

#include <hci/hci.h>
#include <stdlib.h>

#include <atomic>
#include <memory>
#include <type_traits>
#include <utility>

namespace hci {

template <typename T, typename = void>
struct pass_ptr;
template <typename T, typename = void>
class safe_ptr;
template <typename T, typename = void>
class weak_ptr;

class HCI_LOCAL HciObject;
class HCI_LOCAL HciMetaObject;

// 当一个类的继承关系中因多重继承存在两个 HciObject 时，这个类必须使用
// typedef 将其中一个 HciObject 类型定义成 primary_obj，这样将通过此类
// 型访问 HciObject 的相关方法，避免编译时产生歧义调用。
template <typename T, typename = void>
struct get_primary_obj {
  typedef T type;
};

template <typename T>
struct get_primary_obj<T, HCI_HAS_TYPE(T, primary_obj)> {
  typedef typename T::primary_obj type;
};

// 对象引用计数减少为零时的回调原型
typedef void (*hci_finalizer_t)(void *);

/**
 * 元对象
 *
 * 用于实现引用计数和弱引用。用户不需要直接使用
 */
class HCI_LOCAL HciMetaObject {
  HCI_INTERFACE(HciMetaObject);

  template <typename>
  friend class HciObjectRoot;
  template <typename, typename>
  friend class weak_ptr;
  friend class HciObjectHelper;
  friend class HciObject;

 public:
  virtual int RefCount() = 0;  // 引用计数

 protected:
  virtual void AddRef() = 0;   // 增加引用
  virtual bool Release() = 0;  // 减少引用，减为零释放对象后返 true

 public:
  virtual int MetaRefCount() = 0;  // 弱引用计数

 protected:
  virtual void MetaAddRef() = 0;   // 增加弱引用
  virtual bool MetaRelease() = 0;  // 减少弱引用

  virtual HciObject *Resolve(HciObject *) = 0;  // 解析弱引用

  virtual HciMetaObject *Link(void *obj, hci_finalizer_t finalizer) = 0;
  virtual void Unlink(void *obj) = 0;

  virtual bool SetPointer(HciObject *obj, int index, void *ptr,
                          hci_finalizer_t finalizer) = 0;
  virtual void *GetPointer(const HciObject *obj, int index) = 0;
};

HCI_INTERFACE_(HciMetaObject);

class HCI_LOCAL HciObjectHelper {
 private:
  template <typename, typename>
  friend class safe_ptr;
  template <typename, typename>
  friend class weak_ptr;
  template <typename T>
  friend std::shared_ptr<T> shared_ptr(T *);

  template <typename T>
  static HCI_NO_TSAN HciMetaObject *m(T *p, ...) {
    return p->MetaObject();
  }
  template <typename T>
  static HCI_NO_TSAN HciMetaObject *m(T *, typename T::primary_obj const *p) {
    return p->MetaObject();
  }
  template <typename T>
  static T *AddRef(T *p) {
    if (!!p) m(p, p)->AddRef();
    return p;
  }
  template <typename T>
  static HCI_NO_TSAN void Release(T *p) {
    if (!!p) m(p, p)->Release();
  }
  template <typename T>
  static HCI_NO_TSAN int RefCount(T *p) {
    return p ? m(p, p)->RefCount() : 0;
  }
};

/**
 * 基础对象接口
 *
 * 本接口提供引用计数功能，用于控制对象生命周期。由此接口派生的接口需要由模板
 * 类 HciObjectRoot 实现。
 *
 * 可使用 hci/new_object.h 中的 hci::SafeNew 工具函数创建HciObject 及其派生类
 * 的实例。
 *
 * 出于对用户是否已正确理解对象引用计数并且能够正确使用的担心，这里没有提供类
 * 似 AddRef 和 Release 的功能接口。用户应使用 safe_ptr 和 weak_ptr 来自动管理
 * HciObject 及其派生类对象的生命周期。
 */
class HCI_LOCAL HciObject {
  HCI_INTERFACE(HciObject);

  template <typename>
  friend class HciObjectRoot;
  friend class HciObjectHelper;

 protected:
  void operator delete(void *) {
    api::hci_abort("hci/object.h", __LINE__, "wrong impl");
  }

 private:
  virtual HciMetaObject *MetaObject() const = 0;
  void *operator new(size_t size);

 public:
  /**
   * 获取对象引用计数
   */
  HCI_NO_TSAN int RefCount() const { return MetaObject()->RefCount(); }

  /**
   * 设置用户指针及回调
   *
   * 当对象引用计数变为零，被释放前会调用设置的回调。用户指针只能设置一次，后续
   * 调用均会失败，因此无法更改。
   */
  bool UserSet(void *ptr, hci_finalizer_t finalizer) {
    return MetaObject()->SetPointer(this, 0, ptr, finalizer);
  }
  /**
   * 获取用户指针
   */
  void *UserGet() const { return MetaObject()->GetPointer(this, 0); }

  /**
   * 设置内部指针及回调
   *
   * 当对象引用计数变为零，被释放前会调用设置的回调。内部指针只能设置一次，后续
   * 调用均会失败，因此无法更改。
   *
   * 内部指针主要用于跨语言支持，例如: JNI
   */
  bool InternalSet(void *ptr, hci_finalizer_t finalizer) {
    return MetaObject()->SetPointer(this, 1, ptr, finalizer);
  }
  /**
   * 获取内部指针
   */
  void *InternalGet() const { return MetaObject()->GetPointer(this, 1); }
};

HCI_INTERFACE_(HciObject);

/**
 * template <typename T> struct is_object_;
 *
 * 判断类型 T 是否为 HciObject 或其派生类
 *
 *   - 是，定义常量 value 值为 true
 *   - 否，定义常量 value 值为 false
 */
template <typename T, typename = void>
struct is_object_ {
  static constexpr bool value = false;
};
template <>
struct is_object_<HciObject> {
  static constexpr bool value = true;
};
template <typename T>
struct is_object_<T, HCI_VOID_T(is_base_of_<HciObject, T>::value)> {
  static constexpr bool value = true;
};

/**
 * template<typename T> struct is_object_ptr_;
 *
 * 判断类型 T 是否为 HciObject 相关指针
 */
template <typename T, typename = void>
struct is_object_ptr_ {
  static constexpr bool value = false;
};
template <typename T>
struct is_object_ptr_<T *, HCI_VOID_T(is_object_<T>::value)> {
  static constexpr bool value = true;
};
template <typename T>
struct is_object_ptr_<safe_ptr<T>, HCI_VOID_T(is_object_<T>::value)> {
  static constexpr bool value = true;
};
template <typename T>
struct is_object_ptr_<pass_ptr<T>, HCI_VOID_T(is_object_<T>::value)> {
  static constexpr bool value = true;
};
template <>
struct is_object_ptr_<std::nullptr_t> {
  static constexpr bool value = true;
};
template <typename T>
struct is_object_ptr_<T &> : is_object_ptr_<T> {};
template <typename T>
struct is_object_ptr_<T &&> : is_object_ptr_<T> {};

// pass_ptr
template <>
struct pass_ptr<std::nullptr_t> {
  std::nullptr_t get() { return nullptr; }
  operator bool() const { return false; }
  bool operator!() const { return true; }
  std::nullptr_t detach() { return nullptr; }
  template <typename T>
  operator pass_ptr<T>() const {
    pass_ptr<T> o = {nullptr};
    return o;
  }
};

/**
 * pass_ptr - 接收 pass_ptr 的一方负责将对象的引用计数减一
 *
 * 持有引用对象的一个引用计数，在接口中传递 pass_ptr 时，其持有的引用计数会
 * 传递给接收者，发送方不再负责释放这个引用计数；若未传递 pass_ptr，用户应通
 * 过调用 safe() 方法将其转换为 safe_ptr 并忽略返值自动释放引用
 */
template <typename T>
struct pass_ptr<T, HCI_VOID_T(is_object_<T>::value)> {
  T *__ptr__;

  T *get() const { return __ptr__; }
  safe_ptr<T> safe();
  operator bool() const { return !!__ptr__; }
  bool operator!() const { return !__ptr__; }
  T *detach() {
    T *p = __ptr__;
    __ptr__ = nullptr;
    return p;
  }
};

static_assert(std::is_pod<pass_ptr<HciObject>>::value,
              "pass_ptr<T> is not POD type");

/**
 * safe_ptr
 *
 * 持有引用对象的一个引用计数，当不再引用对象时(析构、置空或者赋值时)，
 * 释放这个引用计数；对于空指针，则不做任何操作
 */
template <typename T>
class safe_ptr<T, HCI_VOID_T(is_object_<T>::value)> : public HciObjectHelper {
  template <typename, typename>
  friend class safe_ptr;
  template <typename U>
  friend pass_ptr<U> pass(safe_ptr<U> &&p);

  std::atomic<T *> ptr_;

  T *detach() { return ptr_.exchange(nullptr); }
  T *exchange(T *desired) { return ptr_.exchange(desired); }

 public:
  safe_ptr() : ptr_(nullptr) {}
  safe_ptr(T *p) : ptr_(AddRef(p)) {}
  safe_ptr(const safe_ptr &r) : ptr_(AddRef(r.get())) {}
  safe_ptr(safe_ptr &&r) : ptr_(r.detach()) {}

  template <typename U>
  safe_ptr(const safe_ptr<U> &r) : ptr_(AddRef(r.get())) {}
  template <typename U>
  safe_ptr(safe_ptr<U> &&r) : ptr_(r.detach()) {}

  template <typename U>
  safe_ptr(pass_ptr<U> &p) : ptr_(p.detach()) {}
  template <typename U>
  safe_ptr(pass_ptr<U> &&p) : ptr_(p.detach()) {}

  safe_ptr(std::nullptr_t) : ptr_(nullptr) {}

  ~safe_ptr() { reset(); }

  safe_ptr &operator=(const safe_ptr &p) {
    if (this != &p) Release(exchange(AddRef(p.ptr_.load())));
    return *this;
  }
  safe_ptr &operator=(safe_ptr &&p) {
    Release(exchange(p.detach()));
    return *this;
  }

  template <typename U>
  safe_ptr &operator=(const safe_ptr<U> &p) {
    if (this != &p) Release(exchange(AddRef(p.ptr_.load())));
    return *this;
  }
  template <typename U>
  safe_ptr &operator=(safe_ptr<U> &&p) {
    Release(exchange(p.detach()));
    return *this;
  }
  template <typename U>
  safe_ptr &operator=(pass_ptr<U> &p) {
    Release(exchange(p.detach()));
    return *this;
  }
  template <typename U>
  safe_ptr &operator=(pass_ptr<U> &&p) {
    Release(exchange(p.detach()));
    return *this;
  }

  int use_count() const { return RefCount(ptr_.load()); }

  void set(T *p) {
    if (p != ptr_) Release(exchange(AddRef(p)));
  }

  void reset() { Release(detach()); }
  HCI_WARN_UNUSED_RESULT pass_ptr<T> pass() {
    pass_ptr<T> o = {detach()};
    return o;
  }

  T *get() const { return ptr_; }
  T *operator->() const { return ptr_; }
  operator bool() const { return !!ptr_; }
  bool operator!() const { return !ptr_; }
  HCI_WARN_UNUSED_RESULT pass_ptr<T> assign(pass_ptr<T> &&v) {
    pass_ptr<T> ret = {exchange(v.detach())};
    return ret;
  }
};

template <typename T>
inline safe_ptr<T> pass_ptr<T, HCI_VOID_T(is_object_<T>::value)>::safe() {
  return safe_ptr<T>(*this);
}

/**
 * weak_ptr
 *
 * 不持有引用对象的引用计数，即使对象引用计数减少为零被释放后，weak_ptr 仍
 * 存有原对象的裸指针。用户不能直接使用 weak_ptr 引用的对象，需要先使用
 * safe() 方法转换为 safe_ptr 才可使用。调用 safe() 时，若引用对象引用计数
 * 为零，返回空的 safe_ptr；否则，返回一个非空的 safe_ptr，其将持有一个新增
 * 的引用计数。
 */
template <typename T>
class weak_ptr<T, HCI_VOID_T(is_object_<T>::value)> : public HciObjectHelper {
  HciMetaObject *m_;
  T *o_;

  void _init(T *p) {
    bool is_null = (o_ = p) == nullptr;
    if (is_null) {
      m_ = nullptr;
    } else {
      (m_ = m(p, p))->MetaAddRef();
    }
  }
  T *resolve() const {
    typedef typename get_primary_obj<T>::type type;
    type *p = o_;
    if (!m_) return nullptr;
    return m_->Resolve(p) ? o_ : nullptr;
  }

 public:
  weak_ptr() : m_(nullptr), o_(nullptr) {}
  weak_ptr(T *p) { _init(p); }
  weak_ptr(const weak_ptr &r) : m_(r.m_), o_(r.o_) {
    if (!!m_) m_->MetaAddRef();
  }
  weak_ptr(weak_ptr &&r) {
    m_ = r.m_, r.m_ = nullptr;
    o_ = r.o_, r.o_ = nullptr;
  }
  template <typename U>
  weak_ptr(const safe_ptr<U> &r) {
    _init(r.get());
  }

  HCI_NO_TSAN ~weak_ptr() {
    if (!!m_) m_->MetaRelease();
  }
  void set(T *p) {
    this->~weak_ptr();
    this->_init(p);
  }
  void reset() {
    this->~weak_ptr();
    m_ = nullptr;
    o_ = nullptr;
  }
  HCI_NO_TSAN int use_count() const { return !!m_ ? m_->RefCount() : 0; }
  size_t hash() const { return 3141592653u ^ reinterpret_cast<size_t>(o_); }

  weak_ptr &operator=(T *p) { return set(p), *this; }
  weak_ptr &operator=(const weak_ptr &r) { return set(r.o_), *this; }
  weak_ptr &operator=(weak_ptr &&r) {
    if (!!m_) m_->MetaRelease();
    m_ = r.m_, r.m_ = nullptr;
    o_ = r.o_, r.o_ = nullptr;
    return *this;
  }
  bool operator==(const weak_ptr &o) const { return o_ == o.o_; }

  safe_ptr<T> safe() const {
    pass_ptr<T> o = {resolve()};
    return safe_ptr<T>(o);
  }
};

struct hash_weak_ptr {
  template <typename T>
  size_t operator()(const weak_ptr<T> &w) const {
    return w.hash();
  }
};

template <typename T>
static inline weak_ptr<T> weak(T *p) {
  return weak_ptr<T>(p);
}

template <typename T>
static inline weak_ptr<T> weak(const safe_ptr<T> &p) {
  return weak_ptr<T>(p);
}

template <typename T>
static inline HCI_WARN_UNUSED_RESULT pass_ptr<T> wrap(T *p) {
  pass_ptr<T> o = {p};
  return o;
}

template <typename T>
static inline HCI_WARN_UNUSED_RESULT pass_ptr<T> pass(safe_ptr<T> &&p) {
  return wrap<T>(p.detach());
}
template <typename T>
static inline HCI_WARN_UNUSED_RESULT pass_ptr<T> pass(const safe_ptr<T> &p) {
  return pass(safe_ptr<T>(p));
}
template <typename T>
static inline HCI_WARN_UNUSED_RESULT pass_ptr<T> pass(T *p) {
  return pass(safe_ptr<T>(p));
}
template <typename T>
static inline HCI_WARN_UNUSED_RESULT pass_ptr<T> pass(std::nullptr_t) {
  pass_ptr<T> o = {nullptr};
  return o;
}
template <typename T>
static inline HCI_WARN_UNUSED_RESULT pass_ptr<T> pass(pass_ptr<T> &&p) {
  return std::move(p);
}
template <typename T>
static inline HCI_WARN_UNUSED_RESULT pass_ptr<T> pass(
    pass_ptr<T> &p) {  // NOLINT
  return std::move(p);
}

static inline pass_ptr<std::nullptr_t> pass(std::nullptr_t) {
  return pass_ptr<std::nullptr_t>();
}

};  // namespace hci

#define IS_HCI_PTR(name, base)                                        \
  template <typename T, typename = void>                              \
  struct name {                                                       \
    static constexpr bool value = false;                              \
  };                                                                  \
  template <typename T>                                               \
  struct name<T &> : name<T> {};                                      \
  template <typename T>                                               \
  struct name<T &&> : name<T> {};                                     \
  template <>                                                         \
  struct name<std::nullptr_t> {                                       \
    static constexpr bool value = true;                               \
  };                                                                  \
  template <>                                                         \
  struct name<base *> {                                               \
    static constexpr bool value = true;                               \
  };                                                                  \
  template <typename T>                                               \
  struct name<T *, HCI_VOID_T(is_base_of_<base, T>::value)> {         \
    static constexpr bool value = true;                               \
  };                                                                  \
  template <>                                                         \
  struct name<safe_ptr<base>> {                                       \
    static constexpr bool value = true;                               \
  };                                                                  \
  template <typename T>                                               \
  struct name<safe_ptr<T>, HCI_VOID_T(is_base_of_<base, T>::value)> { \
    static constexpr bool value = true;                               \
  };

#endif  // HCI_OBJECT_H_
