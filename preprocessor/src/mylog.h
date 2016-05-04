#ifndef __MY_LOG_H__
#define __MY_LOG_H__

#include <cstdio>
#include <unistd.h>

#ifndef CONFIG_LOG_LEVEL
#define CONFIG_LOG_LEVEL 4
#endif

enum class LOG_LEVELS
{
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    NO_LOG = 5
};

#define LOG(LEVEL, fmtstr, ...) do { \
    if (LOG_LEVELS::LEVEL >= (LOG_LEVELS)CONFIG_LOG_LEVEL) \
    { \
        printf("%s:%s:%d:%d: " fmtstr "\n", \
                __FILE__, __func__, __LINE__, getpid(), \
                ##__VA_ARGS__); \
    } \
} while(0)

#endif // __MY_LOG_H__
