#ifndef UTILS_H
#define UTILS_H

#define max(a, b) a > b ? a : b
#define min(a, b) a < b ? a : b

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(*arr))

#define ASSERT_STATIC_INITIALIZED assert(initCalled && "Init was not called.")

#endif // UTILS_H
