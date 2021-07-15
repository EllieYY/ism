
#define HCI_IS_GCC 1

#if (__GNUC__ * 100 + __GNUC_MINOR__) < 406
#error "PLEASE USE g++ 4.6+ COMPILER"
#endif

#if (__GNUC__ * 100 + __GNUC_MINOR__) < 407
#define override
#define final
#endif

#include <hci/compiler/gnu-common.h>
