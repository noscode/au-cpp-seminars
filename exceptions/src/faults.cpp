#include <cstddef>
#include <cstdlib>
#include <new>
#include <iostream>

struct fault_distribution
{
    fault_distribution()
    {
        // Make faults reproduction stable
        srand(0);
    }

    bool should_fault()
    {
        // chance to fail
        return rand() < (RAND_MAX / 60);
        return false;
    }
};

void* operator new(std::size_t count)
{
    static fault_distribution fdistr;

    if (fdistr.should_fault())
    {
        std::cerr << "injecting allocation fault" << std::endl;
        throw std::bad_alloc();
    }

    return malloc(count);
}

void operator delete(void* ptr)
{
    free(ptr);
}
