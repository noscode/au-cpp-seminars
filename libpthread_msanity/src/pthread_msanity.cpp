#include <cstdio>
#include <pthread_msanity.h>
#include <dlfcn.h>

typedef int (*pthread_mutex_lock_func)(pthread_mutex_t*);
typedef int (*pthread_mutex_unlock_func)(pthread_mutex_t*);

pthread_mutex_lock_func original_pthread_mutex_lock;
pthread_mutex_unlock_func original_pthread_mutex_unlock;

static void __attribute__ ((constructor)) pthread_msanity_init(void)
{
    original_pthread_mutex_lock = (pthread_mutex_lock_func)
        dlsym(RTLD_NEXT, "pthread_mutex_lock");
    if (original_pthread_mutex_lock == NULL) {
        fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
        return;
    }
    original_pthread_mutex_unlock = (pthread_mutex_unlock_func)
        dlsym(RTLD_NEXT, "pthread_mutex_unlock");
    if (original_pthread_mutex_unlock == NULL) {
        fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
        return;
    }
}

static void __attribute__ ((destructor))  pthread_msanity_deinit(void)
{
    original_pthread_mutex_lock = nullptr;
    original_pthread_mutex_unlock = nullptr;
}

int pthread_mutex_lock(pthread_mutex_t *mutex) throw()
{
    fprintf(stderr, "Lock %p \n", mutex);
    return original_pthread_mutex_lock(mutex);
}

int pthread_mutex_unlock(pthread_mutex_t *mutex) throw()
{
    fprintf(stderr, "Unlock %p \n", mutex);
    return original_pthread_mutex_unlock(mutex);
}

// Looks quite simple.
// So there is a task for skilled students:
// implement mutex lock ordering checking.
// The algorithm is the following:
// keep each unique sequence
// of lock operations as ordered array of mutex addresses.
// Then for each new lock operation find all the locking
// sequences that contain at least the same locks.
// Compare current thread locking sequence and all the stored
// sequences. They should be equal up until and including current
// mutex address.
