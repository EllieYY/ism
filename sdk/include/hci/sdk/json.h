
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/sdk/json.h
 *
 * 提供一个简单的 JSON 工具，基于 cJSON 实现
 *
 *   - cJSON 接口由 hci-sys 导出
 *   - 本头文件中所有内容均为内联实现
 */
#ifndef HCI_SDK_JSON_H_
#define HCI_SDK_JSON_H_

#include <hci/table.h>
#include <errno.h>
#include "cJSON.h"

#include <limits>

#ifdef _MSC_VER
#define _hci_strdup _strdup
#else
#define _hci_strdup strdup
#endif

HCI_NS_BEGIN(hci, sdk);

class HCI_LOCAL JsonException {
 public:
  JsonException(const char *message) { message_ = _hci_strdup(message); }
  JsonException(JsonException &&r) : message_(r.message_) {
    r.message_ = nullptr;
  }
  JsonException(const JsonException &r) { message_ = _hci_strdup(r.message_); }
  ~JsonException() { free(message_); }
  const char *message() const { return message_; }

 private:
  char *message_;
};

// 使用方法请参考灵云 SDK C++ 开发手册
class HCI_LOCAL JsonValue {
 public:
  JsonValue() : __raw__(nullptr) {}
  JsonValue(cJSON *json, bool ref) {
    auto raw = reinterpret_cast<intptr_t>(json);
    this->__raw__ = reinterpret_cast<cJSON *>(raw | (ref ? 1 : 0));
  }
  JsonValue(JsonValue &&r) { __raw__ = r.__raw__, r.__raw__ = nullptr; }
  JsonValue(const JsonValue &r) {
    auto raw = reinterpret_cast<intptr_t>(r.__raw());
    __raw__ = reinterpret_cast<cJSON *>(raw ? (raw | 1) : 0);
  }
  ~JsonValue() { reset(); }
  void reset() {
    auto raw = reinterpret_cast<intptr_t>(__raw__);
    if ((raw & 1) == 0) cJSON_Delete(__raw__);
    __raw__ = nullptr;
  }
  cJSON *release() {
    auto p = __raw();
    __raw__ = nullptr;
    return p;
  }

  JsonValue copy() const {
    auto *p = __raw();
    return JsonValue(p ? cJSON_Duplicate(p, true) : nullptr, false);
  }

  JsonValue &operator=(JsonValue &&r) {
    reset();
    __raw__ = r.__raw__, r.__raw__ = nullptr;
    return *this;
  }
  JsonValue &operator=(const JsonValue &r) {
    reset();
    auto raw = reinterpret_cast<intptr_t>(r.__raw());
    __raw__ = reinterpret_cast<cJSON *>(raw ? (raw | 1) : 0);
    return *this;
  }

  operator bool() const { return __raw() != nullptr; };
  operator cJSON *() const { return __raw(); }
  cJSON *operator->() const { return __raw(); }

  //{{{ 类型判断
  bool is_null() const { return cJSON_IsNull(*this); }
  bool is_true() const { return cJSON_IsTrue(*this); }
  bool is_false() const { return cJSON_IsTrue(*this); }
  bool is_bool() const { return cJSON_IsBool(*this); }
  bool is_number() const { return cJSON_IsNumber(*this); }
  bool is_string() const { return cJSON_IsString(*this); }
  bool is_array() const { return cJSON_IsArray(*this); }
  bool is_object() const { return cJSON_IsObject(*this); }
  //}}} 类型判断

  //{{{ 对象操作 (Object)
  void add(const char *key, JsonValue &&value, bool const_key = false) {
    auto raw = reinterpret_cast<intptr_t>(value.__raw__);
    if (raw & 1) throw JsonException("ref = 1");
    if (const_key)
      cJSON_AddItemToObjectCS(*this, key, value);
    else
      cJSON_AddItemToObject(*this, key, value);
    value.__raw__ = nullptr;
  }
  void add(const char *key, JsonValue &value, bool const_key = false) {
    auto raw = reinterpret_cast<intptr_t>(value.__raw__);
    if (raw & 1) throw JsonException("ref = 1");
    if (const_key)
      cJSON_AddItemToObjectCS(*this, key, value);
    else
      cJSON_AddItemToObject(*this, key, value);
    value.__raw__ = reinterpret_cast<cJSON *>(raw | 1);
  }
  void add(const char *key, bool value, bool const_key = false) {
    add(key, NewBool(value), const_key);
  }
  void add(const char *key, double value, bool const_key = false) {
    add(key, NewNumber(value), const_key);
  }
  template <typename T, HCI_VALID_IF(std::is_integral<T>::value)>
  void add(const char *key, T value, bool const_key = false) {
    add(key, NewNumber(value), const_key);
  }
  void add(const char *key, std::nullptr_t, bool const_key = false) {
    add(key, NewNull(), const_key);
  }
  void add(const char *key, const char *value, bool const_key = false) {
    add(key, NewString(value), const_key);
  }
  void add_refstr(const char *key, const char *value, bool const_key = false) {
    add(key, NewRefString(value), const_key);
  }
  //}}} 对象操作 (Object)

  //{{{ 数组操作 (Array)
  void add(JsonValue &&value) {
    auto raw = reinterpret_cast<intptr_t>(value.__raw__);
    if (raw & 1) throw JsonException("ref = 1");
    cJSON_AddItemToArray(*this, value);
    value.__raw__ = nullptr;
  }
  void add(JsonValue &value) {
    auto raw = reinterpret_cast<intptr_t>(value.__raw__);
    if (raw & 1) throw JsonException("ref = 1");
    cJSON_AddItemToArray(*this, value);
    value.__raw__ = reinterpret_cast<cJSON *>(raw | 1);
  }
  void add(bool value) { add(NewBool(value)); }
  void add(double value) { add(NewNumber(value)); }
  template <typename T, HCI_VALID_IF(std::is_integral<T>::value)>
  void add(T value) {
    add(NewNumber(value));
  }
  void add(std::nullptr_t) { add(NewNull()); }
  void add(const char *value) { add(NewString(value)); }
  void add_refstr(const char *value) { add(NewRefString(value)); }
  //}}} 数组操作 (Array)

  const char *stringValue() const {
    auto node = __raw();
    if (!node || cJSON_IsNull(node)) return nullptr;
    if (cJSON_IsBool(node)) return cJSON_IsTrue(node) ? "true" : "false";
    if (cJSON_IsString(node)) {
      return cJSON_GetStringValue(node);
    }
    throw JsonException("not string value");
  }
  const char *stringValue(const char *default_value) const {
    auto node = __raw();
    if (!node || cJSON_IsNull(node)) return nullptr;
    if (cJSON_IsBool(node)) return cJSON_IsTrue(node) ? "true" : "false";
    if (cJSON_IsString(node)) {
      return cJSON_GetStringValue(node);
    }
    return default_value;
  }

  bool boolValue() const {
    auto node = __raw();
    if (!node) throw JsonException("not exist");
    if (cJSON_IsNull(node)) return false;
    if (cJSON_IsBool(node)) return cJSON_IsTrue(node);
    if (cJSON_IsString(node)) {
      auto sz = cJSON_GetStringValue(node);
      if (!strcasecmp(sz, "true")) return true;
      if (!strcasecmp(sz, "false")) return false;
    }
    throw JsonException("not boolean value");
  }
  bool boolValue(bool default_value) const {
    auto node = __raw();
    if (!node) return default_value;
    if (cJSON_IsNull(node)) return false;
    if (cJSON_IsBool(node)) return cJSON_IsTrue(node);
    if (cJSON_IsString(node)) {
      auto sz = cJSON_GetStringValue(node);
      if (!strcasecmp(sz, "true")) return true;
      if (!strcasecmp(sz, "false")) return false;
    }
    return default_value;
  }

#define NUMBER_VALUE(name, type, sztype)                        \
  template <typename T = type>                                  \
  T name() const {                                              \
    auto node = __raw();                                        \
    if (!node) throw JsonException("key not exist");            \
    if (cJSON_IsNull(node)) return static_cast<T>(0);           \
    if (cJSON_IsBool(node)) /**/                                \
      return static_cast<T>(cJSON_IsTrue(node) ? 1 : 0);        \
    if (cJSON_IsNumber(node)) {                                 \
      if (node->valuedouble < (std::numeric_limits<T>::min)())  \
        throw JsonException(sztype " underflow");               \
      if (node->valuedouble > (std::numeric_limits<T>::max)())  \
        throw JsonException(sztype " overflow");                \
      return static_cast<T>(node->valuedouble);                 \
    }                                                           \
    if (cJSON_IsString(node)) {                                 \
      return parseNumber<T>(cJSON_GetStringValue(node), false); \
    }                                                           \
    throw JsonException("not " sztype " value");                \
  }                                                             \
  template <typename T = type>                                  \
  T name(T default_value) const {                               \
    auto node = __raw();                                        \
    if (!node) return default_value;                            \
    if (cJSON_IsNull(node)) return static_cast<T>(0);           \
    if (cJSON_IsBool(node)) /**/                                \
      return static_cast<T>(cJSON_IsTrue(node) ? 1 : 0);        \
    if (cJSON_IsNumber(node)) {                                 \
      if (node->valuedouble < (std::numeric_limits<T>::min)())  \
        return (std::numeric_limits<T>::min)();                 \
      if (node->valuedouble > (std::numeric_limits<T>::max)())  \
        return (std::numeric_limits<T>::max)();                 \
      return static_cast<T>(node->valuedouble);                 \
    }                                                           \
    if (cJSON_IsString(node)) {                                 \
      return parseNumber<T>(cJSON_GetStringValue(node), true);  \
    }                                                           \
    return default_value;                                       \
  }

  NUMBER_VALUE(intValue, int, "integer");

  NUMBER_VALUE(uintValue, unsigned int, "unsigned integer");

  NUMBER_VALUE(realValue, double, "floating-point");

#undef NUMBER_VALUE

  JsonValue get(size_t i) const {
    return JsonValue(cJSON_GetArrayItem(__raw(), static_cast<int>(i)), true);
  }
  JsonValue get(const char *key) const {
    size_t i, len = strlen(key);
    char *cc = reinterpret_cast<char *>(alloca(len + 1)), *p = cc;
    bool next_upper = false;
    for (i = 0; i < len; ++i) {
      if (key[i] == '_')
        next_upper = true;
      else if (next_upper)
        *p++ = static_cast<char>(toupper(key[i])), next_upper = false;
      else
        *p++ = key[i];
    }
    *p++ = 0;
    auto raw = __raw();
    auto child = cJSON_GetObjectItemCaseSensitive(raw, key);
    if (child) return JsonValue(child, true);
    if (strcmp(cc, key) != 0) child = cJSON_GetObjectItemCaseSensitive(raw, cc);
    return JsonValue(child, true);
  }

  bool getBool(size_t i) const { return get(i).boolValue(); }
  bool getBool(const char *key) const { return get(key).boolValue(); }
  bool optBool(size_t i, bool default_value = false) const {
    return get(i).boolValue(default_value);
  }
  bool optBool(const char *key, bool default_value = false) const {
    return get(key).boolValue(default_value);
  }

  const char *getString(size_t i) const { return get(i).stringValue(); }
  const char *getString(const char *key) const {
    return get(key).stringValue();
  }
  const char *optString(size_t i, const char *default_value = nullptr) const {
    return get(i).stringValue(default_value);
  }
  const char *optString(const char *key,
                        const char *default_value = nullptr) const {
    return get(key).stringValue(default_value);
  }

  template <typename T = int>
  T getInt(size_t i) const {
    return get(i).intValue<T>();
  }
  template <typename T = int>
  T getInt(const char *key) const {
    return get(key).intValue<T>();
  }
  template <typename T = int>
  T optInt(size_t i, T default_value = 0) const {
    return get(i).intValue<T>(default_value);
  }
  template <typename T = int>
  T optInt(const char *key, T default_value = 0) const {
    return get(key).intValue<T>(default_value);
  }

  template <typename T = unsigned int>
  T getUInt(size_t i) const {
    return get(i).uintValue<T>();
  }
  template <typename T = unsigned int>
  T getUInt(const char *key) const {
    return get(key).uintValue<T>();
  }
  template <typename T = unsigned int>
  T optUInt(size_t i, T default_value = 0) const {
    return get(i).uintValue<T>(default_value);
  }
  template <typename T = unsigned int>
  T optUInt(const char *key, T default_value = 0) const {
    return get(key).uintValue<T>(default_value);
  }

  template <typename T = double>
  T getReal(size_t i) const {
    return get(i).realValue<T>();
  }
  template <typename T = double>
  T getReal(const char *key) const {
    return get(key).realValue<T>();
  }
  template <typename T = double>
  T optReal(size_t i, T default_value = 0) const {
    return get(i).realValue<T>(default_value);
  }
  template <typename T = double>
  T optReal(const char *key, T default_value = 0) const {
    return get(key).realValue<T>(default_value);
  }

  JsonValue getArray(size_t i) const {
    auto child = get(i);
    if (!child || cJSON_IsNull(child)) return JsonValue(nullptr, false);
    if (!cJSON_IsArray(child)) throw JsonException("not array");
    return child;
  }
  JsonValue getArray(const char *key) const {
    auto child = get(key);
    if (!child || cJSON_IsNull(child)) return JsonValue(nullptr, false);
    if (!cJSON_IsArray(child)) throw JsonException("not array");
    return child;
  }
  JsonValue getObject(size_t i) const {
    auto child = get(i);
    if (!child || cJSON_IsNull(child)) return JsonValue(nullptr, false);
    if (!cJSON_IsObject(child)) throw JsonException("not object");
    return child;
  }
  JsonValue getObject(const char *key) const {
    auto child = get(key);
    if (!child || cJSON_IsNull(child)) return JsonValue(nullptr, false);
    if (!cJSON_IsObject(child)) throw JsonException("not object");
    return child;
  }

  static JsonValue NewNull() {  //
    return JsonValue(cJSON_CreateNull(), false);
  }
  static JsonValue NewBool(bool v) {
    return JsonValue(cJSON_CreateBool(v), false);
  }
  template <typename T, HCI_VALID_IF(std::is_floating_point<T>::value ||
                                     std::is_integral<T>::value)>
  static JsonValue NewNumber(T v) {
    return JsonValue(cJSON_CreateNumber(static_cast<T>(v)), false);
  }
  static JsonValue NewString(const char *v) {
    return JsonValue(cJSON_CreateString(v), false);
  }
  static JsonValue NewRefString(const char *v) {
    return JsonValue(cJSON_CreateStringReference(v), false);
  }
  static JsonValue NewObject() {
    return JsonValue(cJSON_CreateObject(), false);
  }
  static JsonValue NewArray() {  //
    return JsonValue(cJSON_CreateArray(), false);
  }
  static JsonValue Parse(const char *text, int length) {
    const char *end;
    cJSON *json = cJSON_ParseWithOpts(text, length, &end, false);
    if (json == nullptr) return JsonValue(nullptr, false);
    if (length > 0) {
      while (end < text + length) {
        if (!isspace(*end++)) {
          cJSON_Delete(json), json = nullptr;
          break;
        }
      }
    } else {
      while (*end) {
        if (!isspace(*end++)) {
          cJSON_Delete(json), json = nullptr;
          break;
        }
      }
    }
    return JsonValue(json, false);
  }

  template <typename T, HCI_VALID_IF(std::is_integral<T>::value &&
                                     !!std::is_signed<T>::value)>
  static T parseNumber(const char *sz, bool saturated) {
    static_assert(
        std::numeric_limits<T>::is_signed && std::numeric_limits<T>::is_integer,
        "T isn't signed integer");
    char *end;
    errno = 0;
    auto value = strtoll(sz, &end, 0);
    if (*end != 0 || errno != 0) throw JsonException("bad integer format");
    if (value < (std::numeric_limits<T>::min)()) {
      if (saturated)
        return (std::numeric_limits<T>::min)();
      else
        throw JsonException("integer underflow");
    }
    if (value > (std::numeric_limits<T>::max)()) {
      if (saturated)
        return (std::numeric_limits<T>::max)();
      else
        throw JsonException("integer overflow");
    }
    return static_cast<T>(value);
  }
  template <typename T, HCI_VALID_IF(std::is_integral<T>::value &&
                                     !!std::is_unsigned<T>::value)>
  static T parseNumber(const char *sz, bool saturated) {
    static_assert(!std::numeric_limits<T>::is_signed &&
                      std::numeric_limits<T>::is_integer,
                  "T isn't unsigned integer");
    char *end;
    errno = 0;
    auto value = strtoull(sz, &end, 0);
    if (*end != 0 || errno != 0)
      throw JsonException("bad unsigned integer format");
    if (value < (std::numeric_limits<T>::min)()) {
      if (saturated)
        return (std::numeric_limits<T>::min)();
      else
        throw JsonException("unsigned integer underflow");
    }
    if (value > (std::numeric_limits<T>::max)()) {
      if (saturated)
        return (std::numeric_limits<T>::max)();
      else
        throw JsonException("unsigned integer overflow");
    }
    return static_cast<T>(value);
  }
  template <typename T, HCI_VALID_IF(std::is_floating_point<T>::value)>
  static T parseNumber(const char *sz, bool saturated) {
    static_assert(std::numeric_limits<T>::is_signed &&
                      !std::numeric_limits<T>::is_integer,
                  "T isn't floating-point number");
    char *end;
    errno = 0;
    auto value = strtod(sz, &end);
    if (*end != 0 || errno != 0)
      throw JsonException("bad floating-point format");
    if (value < (std::numeric_limits<T>::min)()) {
      if (saturated)
        return (std::numeric_limits<T>::min)();
      else
        throw JsonException("floating-point underflow");
    }
    if (value > (std::numeric_limits<T>::max)()) {
      if (saturated)
        return (std::numeric_limits<T>::max)();
      else
        throw JsonException("floating-point overflow");
    }
    return static_cast<T>(value);
  }

 private:
  cJSON *__raw() const {
    auto raw = reinterpret_cast<intptr_t>(__raw__);
    return reinterpret_cast<cJSON *>(raw & ~intptr_t(1));
  }
  cJSON *__raw__;
};

template <typename T>
struct HCI_LOCAL json_table {
  static void init(const JsonValue &json, T *p);
};

template <typename T>
struct HCI_LOCAL json_table<T &> : json_table<T> {};

template <typename T>
struct HCI_LOCAL json_array {
  static void init(const JsonValue &json, array<T> *p);
};

template <typename T>
struct HCI_LOCAL json_array<T &> : json_array<T> {};

template <typename T>
struct HCI_LOCAL json_enum {
  static std::pair<bool, T> value(const JsonValue &child) {
    if (cJSON_IsNumber(child)) {
      auto v = static_cast<int>(child->valuedouble);
      return std::pair<bool, T>(true, static_cast<T>(v));
    }
    if (!child || !cJSON_IsString(child)) return std::pair<bool, T>();
    T v = T();
    valueOf(cJSON_GetStringValue(child), &v);
    return std::pair<bool, T>(true, v);
  }
};

struct HCI_LOCAL json_table_visitor {
  const JsonValue &root;

  json_table_visitor(const JsonValue &json) : root(json) {}

  // bool field
  std::pair<bool, bool> operator()(unsigned, const char *key, const bool *) {
    auto child = root.get(key);
    try {
      if (child) return std::pair<bool, bool>(true, child.boolValue());
    } catch (JsonException &) {
    }
    return std::pair<bool, bool>();
  }

  // signed integer field
  template <typename T, HCI_VALID_IF(std::is_signed<T>::value &&
                                     !!std::is_integral<T>::value)>
  std::pair<bool, T> operator()(unsigned, const char *key, const T *) {
    auto child = root.get(key);
    try {
      if (child) return std::pair<bool, T>(true, child.intValue<T>());
    } catch (JsonException &) {
    }
    return std::pair<bool, T>();
  }

  // unsigned integer field
  template <typename T, HCI_VALID_IF(std::is_unsigned<T>::value &&
                                     !!std::is_integral<T>::value)>
  std::pair<bool, T> operator()(unsigned, const char *key, const T *) {
    auto child = root.get(key);
    try {
      if (child) return std::pair<bool, T>(true, child.uintValue<T>());
    } catch (JsonException &) {
    }
    return std::pair<bool, T>();
  }

  // floating-point field
  template <typename T, HCI_VALID_IF(std::is_floating_point<T>::value)>
  std::pair<bool, T> operator()(unsigned, const char *key, const T *) {
    auto child = root.get(key);
    try {
      if (child) return std::pair<bool, T>(true, child.realValue<T>());
    } catch (JsonException &) {
    }
    return std::pair<bool, T>();
  }

  // string field
  std::pair<bool, const char *> operator()(unsigned, const char *key,
                                           const char *) {
    auto child = root.get(key);
    try {
      if (child)
        return std::pair<bool, const char *>(true, child.stringValue());
    } catch (JsonException &) {
    }
    return std::pair<bool, const char *>();
  }

  // enum field
  template <typename T, HCI_VALID_IF(std::is_enum<T>::value)>
  std::pair<bool, T> operator()(unsigned, const char *key, const T *) {
    return json_enum<T>::value(root.get(key));
  }

  // json field
  std::pair<bool, JsonValue> operator()(unsigned, const char *key,
                                        const JsonValue *) {
    auto child = root.get(key);
    if (!child) return std::pair<bool, JsonValue>(false, JsonValue());
    return std::pair<bool, JsonValue>(true, child.copy());
  }

  // table field
  template <typename T, HCI_VALID_IF(::hci::detail::is_table<T>::value)>
  std::pair<bool, JsonValue> operator()(unsigned, const char *key, const T *) {
    auto child = root.get(key);
    return std::pair<bool, JsonValue>(cJSON_IsObject(child), child);
  }

  // init table field
  template <typename T, HCI_VALID_IF(::hci::detail::is_table<T>::value)>
  void operator()(const JsonValue &obj, T *p) {
    json_table<T>::init(obj, p);
  }

  // array field
  template <typename T, HCI_VALID_IF(::hci::detail::is_table<T>::value)>
  std::pair<bool, JsonValue> operator()(unsigned, const char *key,
                                        const array<T> *) {
    auto child = root.get(key);
    return std::pair<bool, JsonValue>(cJSON_IsArray(child), child);
  }

  // init array field
  template <typename T, HCI_VALID_IF(detail::is_table<T>::value)>
  void operator()(const JsonValue &obj, array<T> *p) {
    json_array<T>::init(obj, p);
  }
};

struct HCI_LOCAL json_array_visitor {
  const JsonValue &root;
  json_array_visitor(const JsonValue &root) : root(root) {}

  size_t length() { return cJSON_GetArraySize(root); }

  // bool element
  std::pair<bool, bool> operator()(size_t i, const bool *) {
    auto child = root.get(i);
    try {
      if (child) return std::pair<bool, bool>(true, child.boolValue());
    } catch (JsonException &) {
    }
    return std::pair<bool, bool>();
  }

  // signed integer element
  template <typename T, HCI_VALID_IF(std::is_signed<T>::value &&
                                     !!std::is_integral<T>::value)>
  std::pair<bool, T> operator()(size_t i, const T *) {
    auto child = root.get(i);
    try {
      if (child) return std::pair<bool, T>(true, child.intValue<T>());
    } catch (JsonException &) {
    }
    return std::pair<bool, T>();
  }

  // unsigned integer element
  template <typename T, HCI_VALID_IF(std::is_unsigned<T>::value &&
                                     !!std::is_integral<T>::value)>
  std::pair<bool, T> operator()(size_t i, const T *) {
    auto child = root.get(i);
    try {
      if (child) return std::pair<bool, T>(true, child.uintValue<T>());
    } catch (JsonException &) {
    }
    return std::pair<bool, T>();
  }

  // floating-point element
  template <typename T, HCI_VALID_IF(std::is_floating_point<T>::value)>
  std::pair<bool, T> operator()(size_t i, const T *) {
    auto child = root.get(i);
    try {
      if (child) return std::pair<bool, T>(true, child.uintValue<T>());
    } catch (JsonException &) {
    }
    return std::pair<bool, T>();
  }

  // string element
  std::pair<bool, const char *> operator()(size_t i, const char *) {
    auto child = root.get(i);
    try {
      if (child)
        return std::pair<bool, const char *>(true, child.stringValue());
    } catch (JsonException &) {
    }
    return std::pair<bool, const char *>();
  }

  // enum element
  template <typename T, HCI_VALID_IF(std::is_enum<T>::value)>
  std::pair<bool, T> operator()(size_t i, const T *) {
    return json_enum<T>::value(root.get(i));
  }

  // table element
  template <typename T, HCI_VALID_IF(::hci::detail::is_table<T>::value)>
  bool operator()(size_t i, T *p) {
    auto child = root.get(i);
    if (cJSON_IsObject(child))
      return json_table<T>::init(child, p), true;
    else
      return false;
  }

  // array element
  template <typename T>
  bool operator()(size_t i, array<T> *p) {
    auto child = root.get(i);
    if (cJSON_IsArray(child))
      return json_array<T>::init(child, p), true;
    else
      return false;
  }
};

template <typename T>
inline void json_table<T>::init(const JsonValue &json, T *p) {
  p->init(json_table_visitor(json));
}

template <typename T>
inline void json_array<T>::init(const JsonValue &json, array<T> *p) {
  p->init(json_array_visitor(json));
}

#if 0
class HCI_LOCAL common_init_visitor {
 public:
  common_init_visitor(const JsonValue &root) : root_(root) {}  // NOLINT

#define NUMBER_FIELD(type)                                                    \
  std::pair<bool, type> operator()(unsigned, const char *key, const type *) { \
    auto child = root_.get(key);                                              \
    if (!child || !cJSON_IsNumber(child)) return std::pair<bool, type>();     \
    auto v = static_cast<type>(child->valuedouble);                           \
    return std::pair<bool, type>(true, v);                                    \
  }

  NUMBER_FIELD(int32_t);
  NUMBER_FIELD(int64_t);
  NUMBER_FIELD(float);

  std::pair<bool, bool> operator()(unsigned, const char *key, const bool *) {
    auto child = root_.get(key);
    if (!child || !cJSON_IsBool(child)) return std::pair<bool, bool>();
    return std::pair<bool, bool>(true, cJSON_IsTrue(child));
  }

  std::pair<bool, JsonValue> operator()(unsigned, const char *key,
                                        const JsonValue *) {
    auto child = root_.get(key);
    if (!child) return std::pair<bool, JsonValue>();
    cJSON_DetachItemViaPointer(root_, child);
    return std::pair<bool, JsonValue>(true, JsonValue(child.release(), false));
  }

  template <typename T, HCI_VALID_IF(std::is_enum<T>::value)>
  std::pair<bool, T> operator()(unsigned, const char *key, const T *) {
    auto child = root_.get(key);
    if (!child) return std::pair<bool, T>();
    if (cJSON_IsNumber(child)) {
      auto v = static_cast<int>(child->valuedouble);
      return std::pair<bool, T>(true, static_cast<T>(v));
    }
    if (!cJSON_IsString(child)) return std::pair<bool, T>();
    T v = T();
    valueOf(cJSON_GetStringValue(child), &v);
    return std::pair<bool, T>(true, v);
  }

  std::pair<bool, const char *> operator()(unsigned, const char *key,
                                           const char *) {
    auto child = root_.get(key);
    if (!child || !cJSON_IsString(child))
      return std::pair<bool, const char *>();
    auto v = cJSON_GetStringValue(child);
    return std::pair<bool, const char *>(true, v);
  }

 protected:
  const JsonValue &root_;
};
#endif

HCI_NS_END(hci, sdk);

HCI_NS_BEGIN(hci, detail);
template <>
struct is_plain<::hci::sdk::JsonValue> {
  static const bool value = true;
};

template <>
struct HCI_LOCAL np_obj_t::field<::hci::sdk::JsonValue> {
  ::hci::sdk::JsonValue value;

  field() : value() {}
  field(::hci::sdk::JsonValue &&v) : value(std::move(v)) {}
  field(const ::hci::sdk::JsonValue &v) : value(v.copy()) {}
  field(const field &o) : value(o.value.copy()) {}
  void set(::hci::sdk::JsonValue &&v) { value = std::move(v); }
  void set(const ::hci::sdk::JsonValue &v) { value = v.copy(); }
  void set(const field &o) { value = o.value.copy(); }
  void reset() { value = ::hci::sdk::JsonValue(); }
  void operator=(const field &o) { value = o.value.copy(); }
  const ::hci::sdk::JsonValue &get() const { return value; }
};

HCI_NS_END(hci, detail);

#undef _hci_strdup

#endif  // HCI_SDK_JSON_H_
