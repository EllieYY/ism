
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/table.h
 *
 * 提供数组容器、字典容器和二进制数据三种类型，帮助接口设计者解决接口设
 * 计工作中的版本兼容和扩展性问题。这些工具主要用于定义模块接口中输入和
 * 输出的数据结构，便于定义、维护和使用，且易于理解。
 *
 * 对于数组元素或者字典中的字段，其类型可以是以下任何一种：
 *
 *   - 标量类型 (plain)
 *
 *     - bool
 *     - signed char, unsigned char
 *     - short, unsigned short
 *     - int,unsigned int
 *     - long, unsigned long
 *     - long long, unsigned long long
 *     - float, double
 *     - enum 类型
 *
 *   - 字符串类型 (xpstr)
 *
 *     - const char *
 *     - const wchar_t *
 *
 *   - 二进制数据 (bytes), 使用 class hci::bytes 类
 *   - 数组容器类型 (array), 使用 template<typename> class hci::array 模板类
 *   - 字典容器类型 (table)，使用 class hci::table 的派生类
 *
 * 数组容器类型 array<T> 使用模板参数 T 指定元素类型，应为上述类型之一
 *
 * 若 T 是 bytes, array 和 table 中的三者之一，则 array 的原型为:
 *
 * @code {.cpp}
 *
 * template<typename T>
 * class array {
 *  public:
 *   array();
 *   array(const array&);
 *   ~array();
 *
 *   array &operator=(const array &);  // assign
 *
 *   bool reserve(size_t n);       // 预分配空间 (不改变数组元素个数)
 *   bool assign(const array &);   // 赋值操作，operator =
 *   bool resize(size_t n);        // 改变数组长度，长度增加时填充空指针
 *   void clear();                 // 清空数组
 *
 *   T *mutable_at(size_t index);  // 获取指定索引处元素的可写指针
 *                                 // 索引超出元素个数时用空指针自动扩充
 *
 *   T *add();                     // 在数组末尾处添加元素，返回其可写指针
 *   T *insert(size_t index)       // 在指定索引处添加元素，返回其可写指针
 *                                 // 索引超出元素个数时用空指针自动扩充
 *   void remove(size_t index);    // 删除指定索引处的元素
 *
 *   size_t count() const;
 *   const T *at(size_t index) const;
 *
 *   // bool fn(int, const T*); 返回 false 中止遍历
 *   template<typename FN>
 *   bool for_each(FN &&fn);       // 遍历数组元素
 * };
 *
 * @endcode
 *
 * 若 T 属于 plain, 则其原型为
 *
 * @code {.cpp}
 *
 * template<typename T>
 * class array {
 *  public:
 *   array();
 *   array(const array&);
 *   ~array();
 *
 *   array &operator=(const array &);  // assign
 *
 *   bool reserve(size_t n);       // 预分配空间（不改变数组元素个数）
 *   bool assign(const array &);   // 赋值操作, operator =
 *   bool resize(size_t n);        // 改变数组长度
 *   void clear();                 // 清空数组
 *
 *
 *   bool add(T v);                   // 在数组末尾处添加元素
 *   bool set(size_t index, T v);     // 设置指定索引处的元素
 *                                    // 索引超出元素个数时进行自动扩充
 *   bool insert(size_t index, T v);  // 在指定索引处添加元素
 *                                    // 索引超出元素个数时进行自动扩充
 *   void remove(size_t index);       // 删除指定索引处的元素
 *
 *   size_t count() const;
 *   T at(size_t index) const;
 *
 *   // bool fn(int, T); 返回 false 中止遍历
 *   template<typename FN>
 *   bool for_each(FN &&fn);  // 遍历数组元素
 * };
 *
 * @endcode
 *
 * 若 T 属于 xpstr 类型, 则其原型为
 *
 * @code {.cpp}
 *
 * template<typename T>
 * class array<const T *> {
 *  public:
 *   array();
 *   array(const array&);
 *   ~array();
 *
 *   array &operator=(const array &);  // assign
 *
 *   bool reserve(size_t n);       // 预分配空间（不改变数组元素个数）
 *   bool assign(const array &);   // 赋值操作, operator =
 *   bool resize(size_t n);        // 改变数组长度
 *   void clear();                 // 清空数组
 *
 *   // 在数组末尾处添加元素
 *   bool add(const T *sz);            // 整个字符串
 *   bool add(const T *sz, size_t n);  // 字符串中的 n 个字符
 *
 *   // 设置指定索引处的元素，索引超出元素个数时进行自动扩充
 *   bool set(size_t i, const T *sz);            // 整个字符串
 *   bool set(size_t i, const T *sz, size_t n);  // 字符串中的 n 个字符
 *
 *   // 在指定索引处添加元素, 索引超出元素个数时进行自动扩充
 *   bool insert(size_t i, const T *sz);            // 整个字符串
 *   bool insert(size_t i, const T *sz, size_t n);  // 字符串中的 n 个字符
 *
 *   void remove(size_t i);       // 删除指定索引处的元素
 *
 *   size_t count() const;
 *   const char *at(size_t i) const;
 *
 *   // bool fn(int, const T *); 返回 false 中止遍历
 *   template<typename FN>
 *   bool for_each(FN &&fn);  // 遍历数组元素
 * };
 *
 * @endcode
 *
 * 字典容器类型均派生自 class table, 用户应使用以下宏自行定义
 *
 *   - HCI_DECL_TABLE(table_name)      // 定义字典类
 *   - HCI_DECL_TABLE_END(table_name)  // 结束字典类的定义
 *
 *   - HCI_EMPTY_FIELD()      // 定义空字段
 *   - HCI_BLANK_FIELD(type)  // 定义空白字段
 *
 *   - HCI_PLAIN_FIELD(type, name, mandatory, deprecated)  // 定义标量字段
 *   - HCI_XPSTR_FIELD(type, name, mandatory, deprecated)  // 定义字符串字段
 *   - HCI_BYTES_FIELD(type, name, mandatory, deprecated)  // 定义二进制数据字段
 *   - HCI_TABLE_FIELD(type, name, mandatory, deprecated)  // 定义字典类型字段
 *   - HCI_ARRAY_FIELD(type, name, mandatory, deprecated)  // 定义数组类型字段
 *
 * 使用 HCI_BYTES_FIELD 时，type 参数无意义，仅仅是为了形式的一致性
 *
 * 字典容器中所有字段都有一个唯一索引，使用的是定义字段时的先后顺序，计
 * 数从零开始。空字段和空白字段为业务无关字段，无名称，用户不可访问; 其
 * 他字段为业务相关字段，有名称，用户可以访问。最多设置 60 个字段。
 *
 * mandatory 设置为非零值时，表示该字段在所有版本中都会存在。deprecated
 * 设置为非零值时，表示该字段已被弃用，用户应避免使用此字段。这两个参数
 * 可省略，省略时取默认值 0。
 *
 * 维护字典容器类时，若需要保证兼容，则应确保：
 *
 *   - 已有的业务相关字段的类型和索引不能改变
 *   - 已有的业务相关字段的内存布局位置不能发生变化
 *   - 已有的业务相关字段不能被删除
 *   - 已有的业务相关字段的 mandatory 不能发生改变
 *
 * 要满足以上要求需要合理使用空字段和空白字段。空字段占用索引，不占用内
 * 存布局空间；空白字段占用索引，占用内存布局空间。
 *
 * 字段在内存布局中占用的空间大小取决于其类型，标量类型占用的空间大小与
 * sizeof 运算符得到的结果相同，其他类型占用的空间大小与 sizeof(void*)
 * 结果相同。
 *
 * 内存布局还需要考虑对齐问题，编译器在处理对齐时可能会产生用于补齐的空
 * 洞。建议使用 HCI_BLANK_FIELD 对补齐区域定义预留字段，用于以后扩展。
 *
 * 公开接口:
 *
 *   - class bytes;
 *   - template <typename T>
 *     class array;
 *
 *   - HCI_DECL_TABLE(table_name)
 *   - HCI_DECL_TABLE(table_name)
 *
 *   - HCI_EMPTY_FIELD()
 *   - HCI_BLANK_FIELD(type)
 *
 *   - HCI_PLAIN_FIELD(type, name, mandatory, deprecated)
 *   - HCI_XPSTR_FIELD(type, name, mandatory, deprecated)
 *   - HCI_BYTES_FIELD(type, name, mandatory, deprecated)
 *   - HCI_TABLE_FIELD(type, name, mandatory, deprecated)
 *   - HCI_ARRAY_FIELD(type, name, mandatory, deprecated)
 *
 * 二进制数据 bytes 原型
 *
 * @code {.cpp}
 *
 * class bytes {
 *  public:
 *   size_t size() const;
 *   const void *data() const;
 *
 *   void *data();
 *
 *   bool resize(size_t size);
 * };
 *
 * @endcode
 *
 * 业务相关字段均有以下访问方法:
 *
 *   - bool has_name() const;  // 是否设置了该字段
 *   - bool HAS_name() const;  // 是否支持本字段
 *   - void clear_name();      // 清除字段，清除后 has_name() 返回 false
 *
 * 标量字段和字符串字段访问方法：
 *
 *   - type name() const;             // 获取字段的值
 *   - void set_name(type v) const;   // 设置字段
 *
 * 二进制数据字段访问方法:
 *
 *   - bytes *mutable_name();      // 获取字段中保存的对象，可以修改，如果
 *                                 // 没有则先创建它
 *   - const bytes *name() const;  // 获取字段中保存的对象，不可修改，如果
 *                                 // 没有返回 NULL
 * 数组类型字段访问方法:
 *
 *   - array<type> *mutable_name();      // 获取字段中保存的对象，可以修改，
 *                                       // 如果没有则先创建它
 *   - const array<type> *name() const;  // 获取字段中保存的对象，不可修改，
 *                                       // 如果没有返回 NULL
 *
 * 字典类型字段访问方法:
 *
 *   - type *mutable_name();      // 获取字段中保存的对象，可以修改，如果
 *                                // 没有则先创建它
 *   - const type *name() const;  // 获取字段中保存的对象，不可修改，如果
 *                                // 没有返回 NULL
 *
 * @code {.cpp}
 *
 * 字典容器类型还提供以下访问方法:
 *
 * class table_name : public ::hci::detail::table {
 *  public:
 *   table_name();
 *   table_name(const table_name&);
 *   ~table_name();
 *
 *   enum {
 *     ALL_FIELDS,       // table_name 在当前模块中所有支持字段的掩码位
 *     MANDATORY_FIELDS, // table_name 在当前模块中所有 mandatory 字段的掩码位
 *   };
 *
 *   uint64_t field_masks();      // 已设置字段的掩码位
 *   uint64_t all_field_masks();  // 所有支持字段的掩码位
 *
 *   table_name& operator=(const table_name &);
 *   table_name& assign(const table_name &);
 *
 *   void clear();  // 清除所有字段
 *
 *   // 字典有多个类型的字段时，FN 必须是仿函数并针对每个类型重载 operator()
 *   // bool fn(int index, const char *name, T val); 返回 false 将中止遍历
 *   template<typename FN>
 *   bool for_each(FN &&fn);  // 对已设置的字段进行遍历
 * };
 *
 * @endcode
 *
 * 所有类型均不允许用户使用 new 和 delete，但允许进行拷贝构造或赋值。对
 * 于数组和字典，允许嵌套，例如: array<array<const char *>>
 *
 * 当对数据进行操作涉及内存分配时，内存分配均发生创建目标对象的所属模块
 * 内，例如：你要修改一个字典容器，它有一个数组字段但还没有设置，你通过
 * mutable_name() 方法创建该数组，则数组的创建和内存分配均发生在创建字
 * 典容器的那个模块内，而不是发生在调用 mutable_name() 的那个模块内。
 *
 *
 * 关于兼容性
 *
 * 只读操作的兼容性由内存布局所决定，基于此，我们在设计复合类型时使用了
 * 聚合 (aggregation) 的方法而不是组合 (composition) 的方法，这保证了复
 * 合类型是可扩展的，可在新版本中为其增加字段，同时也满足了兼容性的要求。
 * 所有只读操作都是内联方法，保证性能，同时也降低编译和使用的依赖。
 *
 * 修改操作的兼容性除受内存布局影响外，还与版本相关，举个例子：在一个 v2
 * 对象上设置一个内嵌的容器字段时，你肯定希望新创建的对象也是 v2 版本的，
 * 即使你是在一个 v1 版本模块内调用修改操作。基于此，涉及对象创建和内存
 * 分配和释放的操作都直接或间接的使用了虚函数。以确保在修改对象时一旦涉
 * 及对象创建、内存分配或释放时，均在该创建该对象的模块内进行，而不是在
 * 调用修改操作的模块内进行。
 */
#ifndef HCI_TABLE_H_
#define HCI_TABLE_H_

#include <hci/hci.h>

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <algorithm>
#include <utility>

// MSVC 编译器处理 __VA_ARGS__ 机制不同，需要做些特殊处理
// 请参考本链接: https://stackoverflow.com/questions/5134523
#define HCI_PLAIN_FIELD(T, F, ...) \
  PP_EXPAND(__HCI_PLAIN_FIELD, (T, F, ##__VA_ARGS__, 0, 0))
#define HCI_XPSTR_FIELD(T, F, ...) \
  PP_EXPAND(__HCI_XPSTR_FIELD, (T, F, ##__VA_ARGS__, 0, 0))
#define HCI_BYTES_FIELD(T, F, ...) \
  PP_EXPAND(__HCI_BYTES_FIELD, (T, F, ##__VA_ARGS__, 0, 0))
#define HCI_TABLE_FIELD(T, F, ...) \
  PP_EXPAND(__HCI_TABLE_FIELD, (T, F, ##__VA_ARGS__, 0, 0))
#define HCI_ARRAY_FIELD(T, F, ...) \
  PP_EXPAND(__HCI_ARRAY_FIELD, (T, F, ##__VA_ARGS__, 0, 0))
#define HCI_FNPTR_FIELD(T, F, ...) \
  PP_EXPAND(__HCI_FNPTR_FIELD, (T, F, ##__VA_ARGS__, 0, 0))
#define HCI_FNCTX_FIELD(T, F, ...) \
  PP_EXPAND(__HCI_FNCTX_FIELD, (T, F, ##__VA_ARGS__, 0, 0))

namespace hci {

namespace detail {
class HCI_LOCAL np_obj_t {
 protected:
  virtual ~np_obj_t() = 0;

  static void *malloc(size_t size) {  //
    return HCI_ALLOCATOR::malloc(size);
  }
  static void *realloc(void *ptr, size_t size) {
    return HCI_ALLOCATOR::realloc(ptr, size);
  }
  static void free(void *ptr) {  //
    HCI_ALLOCATOR::free(ptr);
  }

  void *operator new(size_t size) { return malloc(size); }
  void operator delete(void *ptr) { free(ptr); }

  void *operator new(size_t size, size_t e) { return malloc(size + e); }
  void operator delete(void *ptr, size_t) { free(ptr); }  // make cl.exe happy

  // placement new
  void *operator new(size_t, void *ptr) { return ptr; }

  template <typename, typename = void>
  struct field;

 public:
  template <typename T>
  T *cast() {
    return static_cast<T *>(this);
  }
};

inline np_obj_t::~np_obj_t() {}

template <typename T>
class HCI_LOCAL xpstr;
template <typename T, typename = void>
class HCI_LOCAL array;

class bytes;
class table;

template <typename, typename = void>
struct is_plain {
  static const bool value = false;
};

template <typename>
struct is_xpstr {
  static const bool value = false;
};

template <typename>
struct is_bytes {
  static const bool value = false;
};

template <typename>
struct is_array {
  static const bool value = false;
};

template <typename, typename = void>
struct is_table {
  static const bool value = false;
};

template <typename>
struct is_fnptr {
  static const bool value = false;
};

template <typename>
struct is_fnctx {
  static const bool value = false;
};

#define HCI_FIELD_TYPE(name, type)  \
  template <>                       \
  struct name<type> {               \
    static const bool value = true; \
  }

HCI_FIELD_TYPE(is_plain, bool);
HCI_FIELD_TYPE(is_plain, signed char);
HCI_FIELD_TYPE(is_plain, unsigned char);
HCI_FIELD_TYPE(is_plain, signed short);    // NOLINT
HCI_FIELD_TYPE(is_plain, unsigned short);  // NOLINT
HCI_FIELD_TYPE(is_plain, signed int);
HCI_FIELD_TYPE(is_plain, unsigned int);
HCI_FIELD_TYPE(is_plain, signed long);         // NOLINT
HCI_FIELD_TYPE(is_plain, unsigned long);       // NOLINT
HCI_FIELD_TYPE(is_plain, signed long long);    // NOLINT
HCI_FIELD_TYPE(is_plain, unsigned long long);  // NOLINT
HCI_FIELD_TYPE(is_plain, float);
HCI_FIELD_TYPE(is_plain, double);

template <typename T>
struct is_plain<T, HCI_VOID_T(std::is_enum<T>::value)> {
  static const bool value = true;
};

template <>
struct is_xpstr<const char *> {
  static const bool value = true;
  typedef char char_type;
};

template <>
struct is_xpstr<const wchar_t *> {
  static const bool value = true;
  typedef wchar_t char_type;
};

template <typename R, typename... Args>
struct is_fnptr<R (*)(Args...)> {
  static const bool value = true;
};

template <>
struct is_fnctx<void *> {
  static const bool value = true;
};

template <>
struct is_bytes<bytes> {
  static const bool value = true;
};

template <typename T>
struct is_array<array<T, void> > {
  static const bool value = true;
};

template <typename T>
struct is_table<T, HCI_VOID_T(is_base_of_<table, T>::value)> {
  static const bool value = true;
};

// plain field
template <typename T>
struct HCI_LOCAL np_obj_t::field<T, HCI_VOID_T(is_plain<T>::value)> {
  T value;

  field() : value() {}
  field(T &&v) : value(std::move(v)) {}
  field(const T &v) : value(v) {}
  field(const field &o) : value(o.value) {}
  template <typename U = T, HCI_VALID_IF(!std::is_trivial<U>::value)>
  void set(U &&v) {
    value = std::forward<U>(v);
  }
  void set(const T &v) { value = v; }
  void set(const field &o) { value = o.value; }
  void reset() { value = T(); }
  void operator=(const field &o) { value = o.value; }
  const T &get() const { return value; }
};

// xpstr field
template <typename T>
struct HCI_LOCAL np_obj_t::field<T, HCI_VOID_T(is_xpstr<T>::value)> {
  xpstr<T> *obj;
  field() : obj(nullptr) {}
  field(T v) { obj = v ? xpstr<T>::create(v) : nullptr; }
  field(T v, size_t len) { obj = v ? xpstr<T>::create(v, len) : nullptr; }
  field(field &&o) : obj(o.obj) { o.obj = nullptr; }
  field(const field &o) {
    if (o.obj == nullptr)
      obj = nullptr;
    else
      obj = xpstr<T>::create(o.obj->str());
  }
  ~field() { reset(); }
  void set(T v) {
    if (obj) delete obj;
    obj = xpstr<T>::create(v);
  }
  void set(T v, size_t len) {
    if (obj) delete obj;
    obj = xpstr<T>::create(v, len);
  }

  void reset() {
    delete obj;
    obj = nullptr;
  }
  void operator=(field &&o) {
    delete obj;
    obj = o.obj;
    o.obj = nullptr;
  }
  void operator=(const field &o) {
    delete obj;
    obj = o.obj ? xpstr<T>::create(o.obj->str()) : nullptr;
  }

  T get() const { return obj ? obj->str() : NULL; }
};

// fnptr field
template <typename T>
struct HCI_LOCAL np_obj_t::field<T, HCI_VOID_T(is_fnptr<T>::value)> {
  T fnptr;

  field() : fnptr() {}
  field(const field &o) : fnptr(o.fnptr) {}
  void set(T v) { fnptr = v; }
  const T get() const { return fnptr; }
  void reset() { fnptr = T(); }
  field &operator=(const field &o) = default;
};

// fnctx field
template <typename T>
struct HCI_LOCAL np_obj_t::field<T, HCI_VOID_T(is_fnctx<T>::value)> {
  T fnctx;

  field() : fnctx() {}
  field(const field &o) : fnctx(o.fnctx) {}
  const T get() const { return fnctx; }
  void set(T v) { fnctx = v; }
  void reset() { fnctx = T(); }
  field &operator=(const field &o) = default;
};

template <typename T>
struct is_mutable {
  static const bool value =
      is_bytes<T>::value || is_array<T>::value || is_table<T>::value;
};

// bytes field or array field or table field
template <typename T>
struct HCI_LOCAL np_obj_t::field<T, HCI_VOID_T(is_mutable<T>::value)> {
  T *obj;

  field() : obj(nullptr) {}
  field(field &&o) : obj(o.obj) { o.obj = nullptr; }
  field(const field &o) {
    if (o.obj == nullptr)
      obj = nullptr;
    else
      obj = new T(*o.obj);
  }
  ~field() { reset(); }
  void reset() {
    delete obj;
    obj = nullptr;
  }
  T *get() { return obj; }
  const T *get() const { return obj; }
  np_obj_t *mutable_get() { return obj = new T(); }
  void operator=(field &&o) {
    delete obj;
    obj = o.obj;
    o.obj = nullptr;
  }
  void operator=(const field &o) {
    delete obj;
    obj = o.obj ? new T(*o.obj) : nullptr;
  }
};

// 字符串类型，不可修改
template <typename T>
class HCI_LOCAL xpstr : public np_obj_t {
  template <typename, typename>
  friend struct np_obj_t::field;

  typedef typename is_xpstr<T>::char_type char_type;

  static size_t length(T str) {
    size_t i = 0;
    while (str[i] != char_type()) ++i;
    return i;
  }
  static xpstr *create(T str) {
    size_t len = length(str);
    size_t size = sizeof(char_type) * (len + 1);
    return new (size) xpstr(len, size, str);
  }
  static xpstr *create(T str, size_t len) {
    size_t size = sizeof(char_type) * (len + 1);
    return new (size) xpstr(len, str);
  }

  xpstr(size_t len, size_t size, T str) {
    void *d = this + 1;
    len_ = len;
    memcpy(d, str, size);
  }
  xpstr(size_t len, T str) {
    void *d = this + 1;
    len_ = len;
    memcpy(d, str, sizeof(char_type) * len);
    char_type *p = reinterpret_cast<char_type *>(this + 1);
    p[len] = 0;
  }

  size_t len_;

 public:
  size_t len() const { return len_; }
  T str() const { return reinterpret_cast<T>(this + 1); }
};

class HCI_LOCAL bytes : public np_obj_t {
  template <typename, typename>
  friend struct np_obj_t::field;

  bytes() : size_(0), ptr_(NULL), user_buffer_(false) {}
  ~bytes() {
    if (!user_buffer_) free(ptr_);
  }

  size_t size_;
  void *ptr_;
  bool user_buffer_;

 public:
  bytes(const bytes &o) {
    if (o.size_ > 0) {
      ptr_ = malloc(size_ = o.size_);
      memcpy(ptr_, o.ptr_, size_);
    } else {
      size_ = 0, ptr_ = NULL;
    }
    user_buffer_ = false;
  }
  bytes &operator=(const bytes &o) {
    set(o.data(), o.size());
    resize(o.size());
    return *this;
  }
  size_t size() const { return size_; }
  const void *data() const { return ptr_; }

  void *data() { return ptr_; }

  virtual bool resize(size_t size) {
    void *p;
    if (user_buffer_) {
      p = malloc(size);
      if (p == NULL) return false;
      user_buffer_ = false;
      memcpy(p, ptr_, size < size_ ? size : size_);
    } else {
      p = realloc(ptr_, size);
      if (p == NULL) return false;
    }
    ptr_ = p;
    size_ = size;
    return true;
  }
  virtual void set(const void *user, size_t size) {
    if (!user_buffer_) free(ptr_);
    ptr_ = const_cast<void *>(user);
    size_ = size;
    user_buffer_ = true;
  }
};

template <typename T>
class HCI_LOCAL array_base : public np_obj_t {
 protected:
  typedef field<T> item_t;
  item_t *ptr_;
  size_t cnt_;
  size_t max_;

  array_base() : ptr_(NULL), cnt_(0), max_(0) {}

  array_base(const array_base &s) : ptr_(NULL), cnt_(0), max_(0) { assign(s); }
  virtual ~array_base() { clear(), free(ptr_); }

  void zero_fill(size_t f, size_t t) {
    for (; f < t; ++f) new (ptr_ + f) item_t();
  }

 public:
  // 必须是虚函数，以确保其行为与调用者所在模块的数据结构版本无关
  virtual bool reserve(size_t n) {
    n = (n + 3u) & ~3u;
    if (n > max_) {
      size_t size = sizeof(*ptr_) * n;
      void *p = realloc(ptr_, size);
      if (p == NULL) return false;
      ptr_ = reinterpret_cast<item_t *>(p);
      max_ = n;
    }
    return true;
  }
  // 必须是虚函数，以确保其行为与调用者所在模块的数据结构版本无关
  virtual bool assign(const array_base &s) {
    if (!array_base::reserve(s.count())) return false;
    size_t i;
    for (i = 0; i < cnt_; ++i) ptr_[i].~item_t();
    cnt_ = s.cnt_;
    for (i = 0; i < cnt_; ++i) new (ptr_ + i) item_t(s.ptr_[i]);
    return true;
  }
  void clear() {
    for (size_t i = 0; i < cnt_; ++i) ptr_[i].~item_t();
    cnt_ = 0;
  }

  bool resize(size_t n) {
    if (n < cnt_) {
      for (size_t i = n; i < cnt_; ++i)  //
        ptr_[i].~item_t();
      cnt_ = n;
    } else if (n > cnt_) {
      if (!reserve(n)) return false;
      zero_fill(cnt_, n);
      cnt_ = n;
    }
    return true;
  }
  size_t count() const { return cnt_; }
  void remove(size_t i) {
    if (i >= cnt_) return;
    std::move(ptr_ + i + 1, ptr_ + cnt_--, ptr_ + i);
    ptr_[cnt_].~item_t();
  }
  template <typename FN>
  bool for_each(FN &&fn) const {
    const item_t *p = ptr_;
    for (size_t i = 0; i < cnt_; ++i) {
      if (!fn(i, p[i].get())) return false;
    }
    return true;
  }
};

// array of plain type
template <typename T>
class HCI_LOCAL array<T, HCI_VOID_T(is_plain<T>::value)>
    : public array_base<T> {
  template <typename, typename>
  friend struct np_obj_t::field;

  typedef array_base<T> base;

  using base::cnt_;
  using base::max_;
  using base::ptr_;
  using base::zero_fill;
  using typename base::item_t;

  bool _insert(size_t i) {
    if (i <= cnt_) {
      if (!reserve(cnt_ + 1u)) return false;
      new (ptr_ + cnt_) item_t();
      std::move_backward(ptr_ + i, ptr_ + cnt_, ptr_ + cnt_ + 1);
      ptr_[i].~item_t();
      ++cnt_;
    } else {
      if (!reserve(i + 1u)) return false;
      zero_fill(cnt_, i);
      cnt_ = i + 1u;
    }
    return true;
  }

 public:
  using base::reserve;

  array() : base() {}
  array(const array &s) : base() { this->assign(s); }
  ~array() {}

  array &operator=(const array &o) { return this->assign(o), *this; }

  bool add(const T v) {
    if (!reserve(cnt_ + 1)) return false;
    return new (ptr_ + cnt_++) item_t(v), true;
  }
  template <typename U = T, HCI_VALID_IF(!std::is_trivial<U>::value)>
  bool add(U &&v) {
    if (!reserve(cnt_ + 1)) return false;
    return new (ptr_ + cnt_++) item_t(std::move(v)), true;
  }
  bool insert(size_t i, const T v) {
    if (!_insert(i)) return false;
    return new (ptr_ + i) item_t(v), true;
  }
  template <typename U = T, HCI_VALID_IF(!std::is_trivial<U>::value)>
  bool insert(size_t i, U &&v) {
    if (!_insert(i)) return false;
    return new (ptr_ + i) item_t(std::move(v)), true;
  }
  bool set(size_t i, const T v) {
    if (i < cnt_) {
      return ptr_[i].set(v), true;
    } else {
      if (!reserve(i + 1)) return false;
      zero_fill(cnt_, i);
      cnt_ = i + 1;
      return new (ptr_ + i) item_t(v), true;
    }
  }
  template <typename U, HCI_VALID_IF(!std::is_trivial<U>::value)>
  bool set(size_t i, U &&v) {
    if (i < cnt_) {
      return ptr_[i].set(std::move(v)), true;
    } else {
      if (!reserve(i + 1)) return false;
      zero_fill(cnt_, i);
      cnt_ = i + 1;
      return new (ptr_ + i) item_t(std::move(v)), true;
    }
  }
  T at(size_t i) const { return i < cnt_ ? ptr_[i].get() : T(); }

  template <typename FN>
  void init(FN &&fn) {
    size_t i, size = fn.length();
    const T *p = nullptr;
    this->resize(size);
    if (size > 0) this->resize(size);
    for (i = 0; i < size; ++i) {
      auto ret = fn(i, p);
      if (ret.first) set(i, ret.second);
    }
  }
};

// array of xpstr
template <typename T>
class HCI_LOCAL array<T, HCI_VOID_T(is_xpstr<T>::value)>
    : public array_base<T> {
  template <typename, typename>
  friend struct np_obj_t::field;

  typedef array_base<T> base;

  using base::cnt_;
  using base::max_;
  using base::ptr_;
  using base::zero_fill;
  using typename base::item_t;

  bool _insert(size_t i) {
    if (i <= cnt_) {
      if (!reserve(cnt_ + 1u)) return false;
      new (ptr_ + cnt_) item_t();
      std::move_backward(ptr_ + i, ptr_ + cnt_, ptr_ + cnt_ + 1);
      ptr_[i].~item_t();
      ++cnt_;
    } else {
      if (!reserve(i + 1u)) return false;
      zero_fill(cnt_, i);
      cnt_ = i + 1u;
    }
    return true;
  }

 public:
  using base::reserve;

  array() : base() {}
  array(const array &s) : base() { this->assign(s); }
  ~array() {}

  array &operator=(const array &o) { return this->assign(o), *this; }

  bool add(const T v) {
    if (!reserve(cnt_ + 1)) return false;
    return new (ptr_ + cnt_++) item_t(v), true;
  }
  bool add(const T v, size_t len) {
    if (!reserve(cnt_ + 1)) return false;
    return new (ptr_ + cnt_++) item_t(v, len), true;
  }
  bool insert(size_t i, const T v) {
    if (!_insert(i)) return false;
    return new (ptr_ + i) item_t(v), true;
  }
  bool insert(size_t i, const T v, size_t len) {
    if (!_insert(i)) return false;
    return new (ptr_ + i) item_t(v, len), true;
  }

  bool set(size_t i, const T v) {
    if (i < cnt_) {
      return ptr_[i].set(v), true;
    } else {
      if (!reserve(i + 1)) return false;
      zero_fill(cnt_, i);
      cnt_ = i + 1;
      return new (ptr_ + i) item_t(v), true;
    }
  }
  bool set(size_t i, const T v, size_t len) {
    if (i < cnt_) {
      return ptr_[i].set(v, len), true;
    } else {
      if (!reserve(i + 1)) return false;
      zero_fill(cnt_, i);
      cnt_ = i + 1;
      return new (ptr_ + i) item_t(v, len), true;
    }
  }
  T at(size_t i) const { return i < cnt_ ? ptr_[i].get() : T(); }

  template <typename FN>
  void init(FN &&fn) {
    size_t i, size = fn.length();
    const T p = nullptr;
    this->resize(0);
    if (size > 0) this->resize(size);
    for (i = 0; i < size; ++i) {
      auto ret = fn(i, p);
      if (ret.first) set(i, ret.second);
    }
  }
};

template <typename T>
class HCI_LOCAL array<T, HCI_VOID_T(is_base_of_<np_obj_t, T>::value)>
    : public array_base<T> {
  template <typename, typename>
  friend struct np_obj_t::field;

  typedef array_base<T> base;

  using base::cnt_;
  using base::max_;
  using base::ptr_;
  using base::zero_fill;
  using typename base::item_t;

  virtual T *_mutable_get(typename base::item_t *item) {
    return item->mutable_get()->template cast<T>();
  }

 public:
  using base::reserve;

  array() : base() {}
  array(const array &s) : base() { this->assign(s); }
  ~array() {}

  array &operator=(const array &o) { return assign(o), *this; }

  const T *at(size_t i) const { return i < cnt_ ? ptr_[i].get() : NULL; }
  T *mutable_at(size_t i) {
    if (i < cnt_) {
      T *p = ptr_[i].get();
      if (p != NULL) return p;
    } else {
      reserve(i + 1u);
      zero_fill(cnt_, i + 1u);
      cnt_ = i + 1u;
    }
    return _mutable_get(&ptr_[i]);
  }
  T *add() {
    if (!reserve(cnt_ + 1)) return NULL;
    new (ptr_ + cnt_) item_t();
    return _mutable_get(&ptr_[cnt_++]);
  }
  T *insert(size_t i) {
    if (i <= cnt_) {
      if (!reserve(cnt_ + 1u)) return NULL;
      memmove(reinterpret_cast<void *>(ptr_ + i + 1), ptr_ + i,
              sizeof(*ptr_) * (cnt_ - i));
      new (ptr_ + i) item_t();
      ++cnt_;
    } else {
      if (!reserve(i + 1u)) return NULL;
      zero_fill(cnt_, i + 1u);
      cnt_ = i + 1u;
    }
    return _mutable_get(&ptr_[i]);
  }

  template <typename FN>
  void init(FN &&fn) {
    size_t i, size = fn.length();
    this->resize(0);
    if (size > 0) this->resize(size);
    for (i = 0; i < size; ++i) {
      if (!fn(i, mutable_at(i))) {
        ptr_[i].reset();
      }
    }
  }
};

class HCI_LOCAL table : public np_obj_t {
 protected:
  template <typename, typename>
  friend struct field;

  static constexpr uint64_t mask(size_t i) { return uint64_t(1) << i; }

  virtual ~table() = 0;

  uint64_t _masks;  // 记录设置的字段
  uint64_t _MASKS;  // 记录支持的字段

  explicit table(uint64_t m) : _masks(0), _MASKS(m) {}

  bool has(size_t i) const { return (_masks & mask(i)) != 0; }
  bool HAS(size_t i) const { return (_MASKS & mask(i)) != 0; }
  template <typename T, typename FT>
  T get(size_t i, FT *p, T v = declval<T>()) const {
    return (_masks & mask(i)) ? p->get() : v;
  }
  template <typename T, typename FT>
  void set(size_t i, FT *p, T &&v) {
    if (!(_MASKS & mask(i))) return;
    _masks |= mask(i);
    p->set(std::forward<T>(v));
  }
  template <typename T, typename FT>
  void set(size_t i, FT *p, T v, size_t len) {
    if (!(_MASKS & mask(i))) return;
    _masks |= mask(i);
    p->set(v, len);
  }
  template <typename T>
  void clear(size_t i, T *d) {
    if (_masks & mask(i)) {
      d->reset();
      _masks &= ~mask(i);
    }
  }

 public:
  uint64_t all_field_masks() const { return _MASKS; }
  uint64_t field_masks() const { return _masks; }
};

inline table::~table() {}
}  // namespace detail

using detail::array;
using detail::bytes;

}  // namespace hci

#define HCI_DECL_TABLE(name)                            \
  class HCI_LOCAL name : private ::hci::detail::table { \
   private:                                             \
    friend class ::hci::detail::np_obj_t;               \
    template <typename, typename>                       \
    friend struct np_obj_t::field;                      \
                                                        \
    using table::clear;                                 \
                                                        \
    enum { A = __COUNTER__ + 1 };                       \
                                                        \
    template <int n = 0>                                \
    struct Boolean {                                    \
      enum { V = n != 0 ? 1 : 0 };                      \
    };                                                  \
                                                        \
    /* feild helper */                                  \
    template <typename X, int>                          \
    struct FH {                                         \
      enum { M = 0, B = 0, I = -1 };                    \
      static void free(X *) {}                          \
      static void copy(X *, const X *) {}               \
      static void assign(X *, const X *) {}             \
      template <typename FN>                            \
      static bool for_each(const X *, FN &&) {          \
        return true;                                    \
      }                                                 \
    };                                                  \
    /* prevent defining 61' field */                    \
    template <typename X>                               \
    struct FH<X, 60> {};                                \
                                                        \
    template <typename X, int>                          \
    struct IFH {                                        \
      template <typename FN>                            \
      static void init(X *, FN &&) {}                   \
    };                                                  \
    template <typename X>                               \
    struct IFH<X, 60> {};                               \
                                                        \
    /* container field helper */                        \
    template <typename X, int I>                        \
    struct CFH {                                        \
      static HCI_FORCEINLINE np_obj_t * /**/            \
      mutable_get(X *d, size_t i) {                     \
        return CFH<X, I - 1>::mutable_get(d, i);        \
      }                                                 \
    };                                                  \
    /* stop at this line */                             \
    template <typename X>                               \
    struct CFH<X, -1> {                                 \
      static HCI_FORCEINLINE np_obj_t * /**/            \
      mutable_get(X *, size_t) {                        \
        return NULL;                                    \
      }                                                 \
    };

#define __HCI_CFH(F)                       \
  template <typename X>                    \
  struct CFH<X, F##I> {                    \
    typedef CFH<X, F##I - 1> prev;         \
                                           \
    static HCI_FORCEINLINE np_obj_t * /**/ \
    mutable_get(X *d, size_t i) {          \
      if (i != F##I)                       \
        return prev::mutable_get(d, i);    \
      else                                 \
        return d->F##_.mutable_get();      \
    }                                      \
  };

#define HCI_EMPTY_FIELD()                                                      \
 private: /* NOLINT */                                                         \
  enum { HCI_CAT(__, __LINE__) = __COUNTER__ - A };                            \
                                                                               \
  template <typename X>                                                        \
  struct FH<X, HCI_CAT(__, __LINE__)> : FH<X, HCI_CAT(__, __LINE__) - 1> {};   \
  template <typename X>                                                        \
  struct IFH<X, HCI_CAT(__, __LINE__)> : IFH<X, HCI_CAT(__, __LINE__) - 1> {}; \
  template <typename X>                                                        \
  struct CFH<X, HCI_CAT(__, __LINE__)> : CFH<X, HCI_CAT(__, __LINE__) - 1> {};

#define HCI_BLANK_FIELD(T) \
  HCI_EMPTY_FIELD();       \
                           \
 private: /* NOLINT */     \
  field<T> HCI_CAT(__f, __LINE__);

#define __HCI_FIELD(T, F, mandatory, deprecated, ...)             \
 private: /* NOLINT */                                            \
  enum { F##I = __COUNTER__ - A };                                \
  field<T> F##_;                                                  \
                                                                  \
  template <typename X>                                           \
  struct FH<X, F##I> {                                            \
    typedef FH<X, F##I - 1> prev;                                 \
    enum : uint64_t {                                             \
      I = F##I,                                                   \
      V = (1ull << F##I),                                         \
      M = V | prev::M,                                            \
      B = (V * Boolean<mandatory>::V) | prev::B                   \
    };                                                            \
    static void free(X *p) {                                      \
      prev::free(p);                                              \
      if (p->has_##F()) p->F##_.reset();                          \
    }                                                             \
    static HCI_FORCEINLINE void /**/                              \
    copy(X *d, const X *s) {                                      \
      prev::copy(d, s);                                           \
      auto mask = table::mask(F##I);                              \
      if (s->_masks & mask) {                                     \
        d->_masks |= mask;                                        \
        d->F##_ = s->F##_;                                        \
      }                                                           \
    }                                                             \
    template <typename FN>                                        \
    static HCI_FORCEINLINE bool /**/                              \
    for_each(const X *p, FN &&fn) {                               \
      if (!prev::for_each(p, std::forward<FN>(fn))) return false; \
      if (!p->has_##F()) return true;                             \
      return fn(F##I, #F, p->F##_.get());                         \
    }                                                             \
  };                                                              \
                                                                  \
 public: /* NOLINT */                                             \
  HCI_CAT(HCI_DEPRECATED, deprecated)                             \
  bool has_##F() const { return has(F##I); }                      \
                                                                  \
  HCI_CAT(HCI_DEPRECATED, deprecated)                             \
  bool HAS_##F() const { return mandatory || HAS(F##I); }         \
                                                                  \
  HCI_CAT(HCI_DEPRECATED, deprecated)                             \
  void clear_##F() {                                              \
    if (HAS_##F()) clear(F##I, &F##_);                            \
  }

#define __HCI_MUTABLE_FIELD(T, F, mandatory, deprecated) \
  HCI_CAT(HCI_DEPRECATED, deprecated)                    \
  const T *F() const { /**/                              \
    return has(F##I) ? F##_.get() : NULL;                \
  }                                                      \
                                                         \
  HCI_CAT(HCI_DEPRECATED, deprecated)                    \
  T *mutable_##F() {                                     \
    if (!HAS_##F()) return NULL;                         \
    if (!has(F##I)) {                                    \
      return mutable_get(F##I)->cast<T>();               \
    } else {                                             \
      return F##_.get();                                 \
    }                                                    \
  }

#define __HCI_PLAIN_FIELD(T, F, mandatory, deprecated, ...)           \
  static_assert(::hci::detail::is_plain<T>::value, "not plain type"); \
  __HCI_FIELD(T, F, mandatory, deprecated);                           \
                                                                      \
 private: /* NOLINT */                                                \
  template <typename X>                                               \
  struct IFH<X, F##I> {                                               \
    typedef IFH<X, F##I - 1> prev;                                    \
    template <typename FN>                                            \
    static HCI_FORCEINLINE void /**/                                  \
    init(X *p, FN &&fn) {                                             \
      prev::init(p, std::forward<FN>(fn));                            \
      if (!p->HAS_##F()) return;                                      \
      const T *value = nullptr;                                       \
      auto ret = fn(F##I, #F, value);                                 \
      if (ret.first) p->set(F##I, &p->F##_, std::move(ret.second));   \
    }                                                                 \
  };                                                                  \
                                                                      \
 public: /* NOLINT */                                                 \
  template <typename U, HCI_VALID_IF(!std::is_trivial<U>::value)>     \
  HCI_CAT(HCI_DEPRECATED, deprecated)                                 \
  void set_##F(U &&v) {                                               \
    set(F##I, &F##_, std::forward<U>(v));                             \
  }                                                                   \
  template <typename U = T, HCI_VALID_IF(std::is_trivial<U>::value)>  \
  HCI_CAT(HCI_DEPRECATED, deprecated)                                 \
  void set_##F(const T v) {                                           \
    set(F##I, &F##_, v);                                              \
  }                                                                   \
                                                                      \
  HCI_CAT(HCI_DEPRECATED, deprecated)                                 \
  T F() const { return get<T>(F##I, &F##_); }                         \
                                                                      \
  HCI_CAT(HCI_DEPRECATED, deprecated)                                 \
  T F(T v) const { return get<T>(F##I, &F##_, v); }

#define __HCI_XPSTR_FIELD(T, F, mandatory, deprecated, ...)   \
  __HCI_FIELD(T, F, mandatory, deprecated);                   \
                                                              \
 private: /* NOLINT */                                        \
  template <typename X>                                       \
  struct IFH<X, F##I> {                                       \
    typedef IFH<X, F##I - 1> prev;                            \
    template <typename FN>                                    \
    static HCI_FORCEINLINE void /**/                          \
    init(X *p, FN &&fn) {                                     \
      prev::init(p, std::forward<FN>(fn));                    \
      if (!p->HAS_##F()) return;                              \
      T value = nullptr;                                      \
      auto ret = fn(F##I, #F, value);                         \
      if (ret.first) p->set(F##I, &p->F##_, ret.second);      \
    }                                                         \
  };                                                          \
                                                              \
 public: /* NOLINT */                                         \
  HCI_CAT(HCI_DEPRECATED, deprecated)                         \
  void set_##F(T v) { set(F##I, &F##_, v); }                  \
                                                              \
  HCI_CAT(HCI_DEPRECATED, deprecated)                         \
  void set_##F(T v, size_t len) { set(F##I, &F##_, v, len); } \
                                                              \
  HCI_CAT(HCI_DEPRECATED, deprecated)                         \
  T F() const { return get<T>(F##I, &F##_); }                 \
                                                              \
  HCI_CAT(HCI_DEPRECATED, deprecated)                         \
  T F(T v) const { return get<T>(F##I, &F##_, v); }

#define __HCI_FNPTR_FIELD(T, F, mandatory, deprecated, ...) \
  static_assert(::hci::detail::is_fnptr<T>::value, /**/     \
                "not fnptr type");                          \
  __HCI_FIELD(T, F, mandatory, deprecated);                 \
                                                            \
 private: /* NOLINT */                                      \
  template <typename X>                                     \
  struct IFH<X, F##I> {                                     \
    typedef IFH<X, F##I - 1> prev;                          \
    template <typename FN>                                  \
    static HCI_FORCEINLINE void /**/                        \
    init(X *p, FN &&fn) {                                   \
      prev::init(p, std::forward<FN>(fn));                  \
      if (!p->HAS_##F()) return;                            \
      const T *value = nullptr;                             \
      auto ret = fn(F##I, #F, value);                       \
      if (ret.first) p->set(F##I, &p->F##_, ret.second);    \
    }                                                       \
  };                                                        \
                                                            \
 public: /* NOLINT */                                       \
  HCI_CAT(HCI_DEPRECATED, deprecated)                       \
  void set_##F(T v) { set(F##I, &F##_, v); }                \
                                                            \
  HCI_CAT(HCI_DEPRECATED, deprecated)                       \
  T F() const { return get<T>(F##I, &F##_); }

#define __HCI_FNCTX_FIELD(T, F, mandatory, deprecated, ...) \
  static_assert(::hci::detail::is_fnctx<T>::value, /**/     \
                "not fnptr type");                          \
  __HCI_FIELD(T, F, mandatory, deprecated);                 \
                                                            \
 private: /* NOLINT */                                      \
  template <typename X>                                     \
  struct IFH<X, F##I> {                                     \
    typedef IFH<X, F##I - 1> prev;                          \
    template <typename FN>                                  \
    static HCI_FORCEINLINE void /**/                        \
    init(X *p, FN &&fn) {                                   \
      prev::init(p, std::forward<FN>(fn));                  \
      if (!p->HAS_##F()) return;                            \
      const T *value = nullptr;                             \
      auto ret = fn(F##I, #F, value);                       \
      if (ret.first) p->set(F##I, &p->F##_, ret.second);    \
    }                                                       \
  };                                                        \
                                                            \
 public: /* NOLINT */                                       \
  HCI_CAT(HCI_DEPRECATED, deprecated)                       \
  void set_##F(T v) { set(F##I, &F##_, v); }                \
                                                            \
  HCI_CAT(HCI_DEPRECATED, deprecated)                       \
  T F() const { return get<T>(F##I, &F##_); }

#define __HCI_BYTES_FIELD(T, F, mandatory, deprecated, ...) \
 private: /* NOLINT */                                      \
  typedef ::hci::bytes F##_type__;                          \
  __HCI_FIELD(F##_type__, F, mandatory, deprecated);        \
                                                            \
 private: /* NOLINT */                                      \
  __HCI_CFH(F);                                             \
                                                            \
  template <typename X>                                     \
  struct IFH<X, F##I> {                                     \
    typedef IFH<X, F##I - 1> prev;                          \
    template <typename FN>                                  \
    static HCI_FORCEINLINE void /**/                        \
    init(X *p, FN &&fn) {                                   \
      prev::init(p, std::forward<FN>(fn));                  \
      if (!p->HAS_##F()) return;                            \
      auto ret = fn(F##I, #F, p->F());                      \
      if (ret.first) fn(ret.second, p->mutable_##F());      \
    }                                                       \
  };                                                        \
                                                            \
 public: /* NOLINT */                                       \
  __HCI_MUTABLE_FIELD(F##_type__, F, mandatory, deprecated);

#define __HCI_TABLE_FIELD(T, F, mandatory, deprecated, ...) \
 private: /* NOLINT */                                      \
  __HCI_FIELD(T, F, mandatory, deprecated);                 \
                                                            \
 private: /* NOLINT */                                      \
  __HCI_CFH(F);                                             \
                                                            \
  template <typename X>                                     \
  struct IFH<X, F##I> {                                     \
    typedef IFH<X, F##I - 1> prev;                          \
    template <typename FN>                                  \
    static HCI_FORCEINLINE void /**/                        \
    init(X *p, FN &&fn) {                                   \
      prev::init(p, std::forward<FN>(fn));                  \
      if (!p->HAS_##F()) return;                            \
      auto ret = fn(F##I, #F, p->F());                      \
      if (ret.first) fn(ret.second, p->mutable_##F());      \
    }                                                       \
  };                                                        \
                                                            \
 public: /* NOLINT */                                       \
  __HCI_MUTABLE_FIELD(T, F, mandatory, deprecated);

#define __HCI_ARRAY_FIELD(T, F, mandatory, deprecated, ...) \
 private: /* NOLINT */                                      \
  typedef ::hci::array<T> F##_type;                         \
  __HCI_FIELD(F##_type, F, mandatory, deprecated);          \
                                                            \
 private: /* NOLINT */                                      \
  __HCI_CFH(F);                                             \
                                                            \
  template <typename X>                                     \
  struct IFH<X, F##I> {                                     \
    typedef IFH<X, F##I - 1> prev;                          \
    template <typename FN>                                  \
    static HCI_FORCEINLINE void /**/                        \
    init(X *p, FN &&fn) {                                   \
      prev::init(p, std::forward<FN>(fn));                  \
      if (!p->HAS_##F()) return;                            \
      auto ret = fn(F##I, #F, p->F());                      \
      if (ret.first) fn(ret.second, p->mutable_##F());      \
    }                                                       \
  };                                                        \
                                                            \
 public: /* NOLINT */                                       \
  __HCI_MUTABLE_FIELD(F##_type, F, mandatory, deprecated);

// clang-format off
#define HCI_DECL_TABLE_END(name)                         \
   private: /* NOLINT */                                 \
    typedef FH<name, __COUNTER__ - A - 1> last;          \
    typedef CFH<name, last::I> last_cfh;                 \
                                                         \
    virtual np_obj_t *mutable_get(size_t i) {            \
      np_obj_t *p = last_cfh::mutable_get(this, i);      \
      if (p) _masks |= mask(i);                          \
      return p;                                          \
    }                                                    \
                                                         \
   public: /* NOLINT */                                  \
    enum : uint64_t {                                    \
      ALL_FIELDS = last::M,                              \
      MANDATORY_FIELDS = last::B,                        \
    };                                                   \
    using table::all_field_masks;                        \
    using table::field_masks;                            \
                                                         \
    name() : table(static_cast<uint64_t>(last::M)) {}    \
    name(const name &s)                                  \
        : table(static_cast<uint64_t>(last::M)) {        \
      /* just copy */                                    \
      last::copy(this, &s);                              \
    }                                                    \
    name &operator=(const name &s) {                     \
      /* call virtual method */                          \
      return this->assign(s);                            \
    }                                                    \
    ~name() { last::free(this); }                        \
    virtual void clear() {                               \
      last::free(this);                                  \
      _masks = 0;                                        \
    }                                                    \
    virtual name &assign(const name &s) {                \
      last::free(this);                                  \
      _masks = 0;                                        \
      last::copy(this, &s);                              \
      return *this;                                      \
    }                                                    \
    template <typename FN>                               \
    bool for_each(FN &&fn) const {                       \
      return last::for_each(this, std::forward<FN>(fn)); \
    }                                                    \
    template <typename FN>                               \
    void init(FN &&fn) {                                 \
      IFH<name, last::I>::init(                          \
          this, std::forward<FN>(fn));                   \
    }                                                    \
  };
// clang-format on

#endif  // HCI_TABLE_H_
