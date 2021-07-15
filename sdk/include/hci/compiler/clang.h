
#define HCI_IS_CLANG 1

#if (__clang_major__ * 100 + __clang_minor__) < 303
#error "PLEASE USE clang++ 3.3+ COMPILER"
#endif

#include <hci/compiler/gnu-common.h>
