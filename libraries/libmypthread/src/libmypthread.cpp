#include <cstdio>
#include <pthread.h>
#include <dlfcn.h>
#include <exception>
#include <unordered_map>
#include <unordered_set>

/*
 * XXX disclaimer. This is intended only for learning.
 * This lib works really bad. Its pthread_mutex_lock
 * and pthread_mutex_unlock are not reenterable
 * but should be because libstdc++ can call throwing
 * operator new any time. Also gcc exception handling
 * routines use pthread mutexes too.
 */

int (*original_pthread_mutex_lock)(pthread_mutex_t*) noexcept;
int (*original_pthread_mutex_unlock)(pthread_mutex_t*) noexcept;

size_t recursive_lib_calls_counter = 0;
pthread_mutex_t recursive_lib_calls_counter_lock =
    PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

// using pthread_t as hash key - no guarantee of portability
// on other UNIX systems
pthread_mutex_t mutex_nonrecursive_check_state_lock =
    PTHREAD_MUTEX_INITIALIZER;
std::unordered_map<pthread_t, std::unordered_set<pthread_mutex_t*>>
    mutex_nonrecursive_check_state;

pthread_mutex_t mutex_deadlock_check_state_lock =
    PTHREAD_MUTEX_INITIALIZER;
std::unordered_map<pthread_t, std::unordered_set<pthread_mutex_t*>>
    mutex_deadlock_check_state;

struct mutex_op_skip_exception_t: std::exception
{};

struct pthread_mutex_locker_t
{
    pthread_mutex_locker_t(pthread_mutex_t *mutex)
        : mutex_(mutex)
    {
        fprintf(stderr, "pthread_mutex_locker_t locking %p by %lu\n",
                mutex_, pthread_self());
        original_pthread_mutex_lock(mutex_);
    }

    ~pthread_mutex_locker_t()
    {
        fprintf(stderr, "pthread_mutex_locker_t unlocking %p by %lu\n",
                mutex_, pthread_self());
        original_pthread_mutex_unlock(mutex_);
    }
private:
    pthread_mutex_t *mutex_;
};

bool print_mutex_lock(pthread_mutex_t *mutex)
{
    fprintf(stderr, "Locking %p by %lu \n", mutex, pthread_self());
    return true;
}

bool print_mutex_unlock(pthread_mutex_t *mutex)
{
    fprintf(stderr, "Unlocking %p by %lu \n", mutex, pthread_self());
    return true;
}

bool mutex_nonrecursive_check_on_lock(pthread_mutex_t *mutex)
{
    pthread_mutex_locker_t locker(&mutex_nonrecursive_check_state_lock);

    if (mutex_nonrecursive_check_state[pthread_self()].find(mutex) !=
            mutex_nonrecursive_check_state[pthread_self()].end())
    {
        fprintf(stderr, "Recursive lock of %p by %lu detected\n",
                mutex, pthread_self());
        return false;
    }
    mutex_nonrecursive_check_state[pthread_self()].insert(mutex);
    return true;
}

bool mutex_nonrecursive_check_on_unlock(pthread_mutex_t *mutex)
{
    pthread_mutex_locker_t locker(&mutex_nonrecursive_check_state_lock);
    mutex_nonrecursive_check_state[pthread_self()].erase(mutex);
    return true;
}

bool mutex_deadlock_check_on_lock(pthread_mutex_t *mutex)
{
    // TODO
    // Track currently locked mutexes by each thread
    // If mutex is locked (see pthread_mutex_trylock)
    // then check that there is no other thread holding
    // mutex and waiting for some mutex locked by current thread
    return true;
}

bool mutex_deadlock_check_on_unlock(pthread_mutex_t *mutex)
{
    // TODO
    return true;
}

extern "C" {

int pthread_mutex_lock(pthread_mutex_t *mutex) noexcept
{ 
    try
    {
        bool cont =
            print_mutex_lock(mutex) &&
            mutex_nonrecursive_check_on_lock(mutex) &&
            mutex_deadlock_check_on_lock(mutex);
        if (!cont)
            return 0;
    }
    catch(...) {}

    return original_pthread_mutex_lock(mutex);
}

int pthread_mutex_unlock(pthread_mutex_t *mutex) noexcept
{
    try {
        bool cont =
            print_mutex_unlock(mutex) &&
            mutex_nonrecursive_check_on_unlock(mutex) &&
            mutex_deadlock_check_on_unlock(mutex);
        if (!cont)
            return 0;
    }
    catch(...) {}

    return original_pthread_mutex_unlock(mutex);
}

}

// Exact algorithm for mutex lock ordering checking:
// keep each unique sequence
// of lock operations as ordered array of mutex addresses.
// Then for each new lock operation find all the locking
// sequences that contain at least the same locks.
// Compare current thread locking sequence and all the stored
// sequences. They should be equal up until and including current
// mutex address.

static void __attribute__ ((constructor)) mypthread_init(void)
{
    original_pthread_mutex_lock =
        reinterpret_cast<decltype(original_pthread_mutex_lock)>(
            dlsym(RTLD_NEXT, "pthread_mutex_lock"));
    if (original_pthread_mutex_lock == NULL)
    {
        fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
        return;
    }

    original_pthread_mutex_unlock =
        reinterpret_cast<decltype(original_pthread_mutex_unlock)>(
            dlsym(RTLD_NEXT, "pthread_mutex_unlock"));
    if (original_pthread_mutex_unlock == NULL)
    {
        fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
        return;
    }
}

static void __attribute__ ((destructor)) mypthread_deinit(void)
{
    original_pthread_mutex_lock = nullptr;
    original_pthread_mutex_unlock = nullptr;
}
