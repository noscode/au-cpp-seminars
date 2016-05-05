#ifndef __MY_ASSERT_H__
#define __MY_ASSERT_H__

#ifndef CONFIG_DEBUG
#define CONFIG_DEBUG 0
#endif

#if (CONFIG_DEBUG != 0)
#include <cstdio>
#include <cstdlib>

#define myassert(cond) do { \
    if (!(cond)) { \
        fprintf(stderr, "%s:%s:%d assertion '%s' failed\n", \
                __FILE__, __func__, __LINE__, #cond); \
        abort(); \
    } \
} while(0)

#else

#define myassert(...)

#endif // CONFIG_DEBUG
#endif //__MY_ASSERT_H__
