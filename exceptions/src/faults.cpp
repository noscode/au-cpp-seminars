#include <cstddef>
#include <cstdlib>
#include <new>

struct fault_distribution
{
    fault_distribution()
    {
        // Make faults reproduction stable
        srand(0);
    }

    bool should_fault()
    {
        // 50% chance to fail
        return rand() > (RAND_MAX / 2);
    }
};

void* operator new(std::size_t count)
{
    static fault_distribution fdistr;

    void *ptr = malloc(count);
    if ((!ptr) || fdistr.should_fault())
        throw std::bad_alloc();
    return ptr;
}

void operator delete(void* ptr)
{
    free(ptr);
}
