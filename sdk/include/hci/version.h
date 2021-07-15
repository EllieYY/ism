
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/version.h
 *
 * 定义版本信息结构体
 */
#ifndef HCI_VERSION_H_
#define HCI_VERSION_H_

#include <hci/hci.h>

#include <string.h>

namespace hci {

/**
 * 版本号
 */
struct HciVersion {
  /**
   * 主版本号
   *
   * 不同的主版本之间不保证兼容
   */
  int major;

  /**
   * 次版本号
   *
   * 相同主版本号下，较大次版本号兼容较小次版本号。当有新增的功能且不影响
   * 兼容性时，应增加次版本号
   */
  int minor;

  /**
   * 小版本号
   *
   * 无新增功能，仅涉及缺陷修复时只增加小版本号
   */
  int patch;

  int reserved;  // padding

  /**
   * 定制版本中使用此字段
   */
  const char *suffix;

  /**
   * 源码版本，svn revision 或者 git revision
   */
  const char *refspec;

  /**
   * 静态方法，构造一个 HciVersion 结构体
   */
  HCI_LOCAL static HciVersion make(int major, int minor, int patch,
                                   const char *suffix = nullptr) {
    HciVersion v = {major, minor, patch, 0, suffix, nullptr};
    return v;
  }
  HCI_LOCAL const char *sfx() const { return suffix ? suffix : ""; }

  template <size_t N>
  const char *to_string(char (&sz)[N]) const {
    if (suffix && suffix[0] != 0) {
      if (refspec && refspec[0] != 0) {
        snprintf(sz, N, "%d.%d.%d-%s (%s)", major, minor, patch, suffix,
                 refspec);
      } else {
        snprintf(sz, N, "%d.%d.%d-%s", major, minor, patch, suffix);
      }
    } else {
      if (refspec && refspec[0] != 0) {
        snprintf(sz, N, "%d.%d.%d (%s)", major, minor, patch, refspec);
      } else {
        snprintf(sz, N, "%d.%d.%d", major, minor, patch);
      }
    }
    return sz;
  }
#define __CMPVER(op, ltr, gtr)     \
  if (major < o.major) return ltr; \
  if (major > o.major) return gtr; \
  if (minor < o.minor) return ltr; \
  if (minor > o.minor) return gtr; \
  if (patch < o.patch) return ltr; \
  if (patch > o.patch) return gtr; \
  return strcmp(sfx(), o.sfx()) op 0

  HCI_LOCAL bool operator<(const HciVersion &o) const {
    __CMPVER(<, true, false);
  }
  HCI_LOCAL bool operator>(const HciVersion &o) const {
    __CMPVER(>, false, true);
  }
  HCI_LOCAL bool operator==(const HciVersion &o) const {
    __CMPVER(==, false, false);
  }
  HCI_LOCAL bool operator!=(const HciVersion &o) const {
    __CMPVER(!=, true, true);
  }
  HCI_LOCAL bool operator<=(const HciVersion &o) const {
    __CMPVER(<=, true, false);
  }
  HCI_LOCAL bool operator>=(const HciVersion &o) const {
    __CMPVER(>=, false, true);
  }
};

};  // namespace hci

#endif  // HCI_VERSION_H_
