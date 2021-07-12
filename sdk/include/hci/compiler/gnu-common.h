
#define HCI_EXPORT __attribute__((visibility("default")))
#define HCI_IMPORT __attribute__((visibility("default")))
#define HCI_LOCAL __attribute__((visibility("hidden")))
#define HCI_FORCEINLINE __attribute__((always_inline)) inline
#define HCI_NORETURN __attribute__((__noreturn__))
#define HCI_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#define HCI_UNUSED __attribute__((unused))
#define HCI_USED __attribute__((used))
#ifdef __cpp_attributes
#define HCI_NO_ASAN __attribute__((no_sanitize_address))
#define HCI_NO_TSAN __attribute__((no_sanitize_thread))
#define HCI_NO_UBSAN __attribute__((no_sanitize_undefined))
#else
#define HCI_NO_ASAN
#define HCI_NO_TSAN
#define HCI_NO_UBSAN
#endif
#define HCI_NON_NULL(...) __attribute__((nonnull(__VA_ARGS__)))

#if __cplusplus >= 201103L
#define HCI_HAS_CPP11 1
#endif

#if __cplusplus >= 201402L
#define HCI_HAS_CPP14 1
#endif

#if __cplusplus >= 201703L
#define HCI_HAS_CPP17 1
#endif

#define HCI_LIKELY(exp) __builtin_expect(!!(exp), 1)
#define HCI_UNLIKELY(exp) __builtin_expect(!!(exp), 0)

#include <hci/compiler/common.h>
