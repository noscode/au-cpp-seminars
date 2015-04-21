#pragma once
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

int pthread_mutex_lock(pthread_mutex_t *mutex) throw();
int pthread_mutex_unlock(pthread_mutex_t *mutex) throw();

#ifdef __cplusplus
}
#endif
