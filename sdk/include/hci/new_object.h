
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/new_object.h
 *
 * 定义 HciObjectRoot 类用于实现 HciObject 对象接口
 *
 * 公开接口:
 *
 *   - template<typename T, typename... Args>
 *     safe_ptr<typename HciObjectRoot<T>::obj_type> SafeNew(Args&&.. args);
 *
 * 模板参数 T 请参考 HciObjectBase 模板类的说明
 */
#ifndef HCI_NEW_OBJECT_H_
#define HCI_NEW_OBJECT_H_

#include <hci/object.h>

#include <assert.h>
#include <utility>

namespace hci {

struct HCI_LOCAL empty_objroot {};

template <typename T>
class HCI_LOCAL HciObjectRoot;

template <typename T, typename = void>
struct objroot_traits;

// 特化 objroot_traits, T 继承关系中包含 HciObject
template <typename T>
struct objroot_traits<T, HCI_VOID_T(is_object_<T>::value)> {
  typedef empty_objroot obj_base;
  typedef typename get_primary_obj<T>::type obj_type;
};

// 特化 objroot_traits, T 继承关系中不包含包含 HciObject
template <typename T>
struct objroot_traits<T, HCI_VOID_T(!is_object_<T>::value)> {
  typedef HciObject obj_base;
  typedef HciObjectRoot<T> obj_type;
};

/**
 * HciObjectRoot 模板类
 *
 * @internal
 *
 * 用户应使用 hci::SafeNew，而不是直接使用 HciObjectRoot 模板类。
 *
 * 无论 T 继承关系中是否包含 HciObject, HciObjectRoot 的继承关系中总会
 * 包含 HciObject。HciObjectRoot 会实现 HciObject 接口中的 MetaObject
 *
 * @code {.cpp}
 *
 * // 定义一个接口
 * class IFoo : public HciObject {
 * public:
 *   virtual int DoSomething() = 0;
 * };
 *
 * // 实现 IFoo 接口，不需要实现 HciObject 接口
 * class FooImpl: public IFoo {
 * public:
 *   virtual int DoSomething() override {
 *     // do something...
 *     return 0;
 *   };
 * };
 *
 * // 使用 hci::SafeNew 会间接使用 HciObjectRoot
 * auto imp = hci::SafeNew<FooImpl>(); // imp 类型为 safe_ptr<FooImpl>
 *
 * // 如果是未继承 HciObject 类型，也可使用 hci::SafeNew。
 * struct CONTEXT { int data; };
 *
 * auto imp = hci::SafeNew<CONTEXT>();
 *
 * imp->data = 12345;
 *
 * @endcode
 */
template <typename T>
class HCI_LOCAL HciObjectRoot : public objroot_traits<T>::obj_base, public T {
  friend class HciObjectHelper;

  HCI_NO_TSAN HciMetaObject* MetaObject() const final { return m_; }
  HciMetaObject* m_;

  static void destroy(void* ptr) {
    delete reinterpret_cast<HciObjectRoot*>(ptr);
  }

  void operator delete(void* ptr) { HCI_ALLOCATOR::free(ptr); }
  void* operator new(size_t size) { return HCI_ALLOCATOR::malloc(size); }

 public:
  typedef typename objroot_traits<T>::obj_type obj_type;

 private:
  template <typename... Args>
  HciObjectRoot(Args&&... args) throw()  // NOLINT
      : T(std::forward<Args>(args)...) {
    m_ = api::_hci_new_metaobj()->Link(this, &destroy);
    // 新建的 HciMetaObject，引用计数必定为零
    assert(m_->RefCount() == 0);
  }

 public:
  template <typename... Args>
  static safe_ptr<obj_type> SafeNew(Args&&... args) {
    return new HciObjectRoot(std::forward<Args>(args)...);
  }
};

template <typename T, typename... Args>
static inline safe_ptr<typename HciObjectRoot<T>::obj_type>  //
SafeNew(Args&&... args) {
  return HciObjectRoot<T>::SafeNew(std::forward<Args>(args)...);
}
};  // namespace hci

#endif  // HCI_NEW_OBJECT_H_
