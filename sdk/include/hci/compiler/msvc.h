// _MSC_VER values
// 1200: MSVC 6.0
// 1300: MSVC 2002 (7.0)
// 1310: MSVC 2003 (7.1)
// 1400: MSVC 2005 (8.0)
// 1500: MSVC 2008 (9.0)
// 1600: MSVC 2010 (10.0)
// 1700: MSVC 2012 (11.0)
// 1800: MSVC 2013 (12.0)
// 1900: MSVC 2015 (14.0)
// 1910: MSVC 2017 RTW (15.0)
// 1911: MSVC 2017 Update 3
// 1912: MSVC 2017 Update 5
// 1913: MSVC 2017 Update 6
// 1914: MSVC 2017 Update 7
// 1915: MSVC 2017 Update 8
// 1916: MSVC 2017 Update 9
// 1920: MSVC 2019 RTW (16.0)

#define HCI_IS_MSVC 1

#if _MSC_VER < 1900
#error "PLEASE USE MSVC 2015+ COMPILER"
#endif

#define HCI_EXPORT __declspec(dllexport)
#define HCI_IMPORT __declspec(dllimport)
#define HCI_LOCAL
#define HCI_FORCEINLINE __forceinline
#define HCI_NORETURN __declspec(noreturn)
#define HCI_WARN_UNUSED_RESULT
#define HCI_UNUSED
#define HCI_USED
#define HCI_NO_ASAN
#define HCI_NO_TSAN
#define HCI_NO_UBSAN
#define HCI_NON_NULL(...)

#define __attribute__(x)

#define HCI_HAS_CPP11 1

#if _MSC_LANG >= 201402L
#define HCI_HAS_CPP14 1
#endif

#if _MSC_LANG >= 201703L
#define HCI_HAS_CPP17 1
#endif

#define HCI_LIKELY(exp) (exp)
#define HCI_UNLIKELY(exp) (exp)

#define _CRT_DECLARE_NONSTDC_NAMES 1
#define _CRT_NONSTDC_NO_DEPRECATE 1

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <hci/compiler/common.h>
