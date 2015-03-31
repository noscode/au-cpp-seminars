#include <iostream>
#include <cassert>
#include <type_traits>
#include "comp_lang.h"
#include "comp_lang_std.h"

template<typename T, size_t size_min, size_t size_max>
using is_size_between =
    std::enable_if< ct_is_inclusive<ct_int<size_min>, ct_int<sizeof(T)>, ct_int<size_max>>::value >;

template<typename T>
T* opt_memcpy(T *dst, const T *src,
        typename is_size_between<T, 1, 8>::type *en=nullptr)
{
    std::cout << "1-8" << std::endl;
    for (size_t i = 0; i < sizeof(*dst); ++i)
    {
        reinterpret_cast<char*>(dst)[i] =
            reinterpret_cast<const char*>(src)[i];
    }
    return dst;
}

template<typename T>
T* opt_memcpy(T *dst, const T *src,
        typename is_size_between<T, 9, 128>::type *en=nullptr)
{
    std::cout << "9-128" << std::endl;
    for (size_t i = 0; i < sizeof(*dst) / sizeof(size_t); ++i)
    {
        reinterpret_cast<size_t*>(dst)[i] =
            reinterpret_cast<const size_t*>(src)[i];
    }
    return dst;
}

template<typename T>
T* opt_memcpy(T *dst, const T *src,
        typename std::enable_if< ct_great<ct_int<sizeof(*dst)>, ct_int<129>>::value >::type *en=nullptr)
{
    std::cout << "129-.." << std::endl;
    return memcpy(dst, src, sizeof(*dst));
}

static void test_memcpy()
{
    struct foo
    {
        size_t data[16];
        bool operator==(const foo &other)
        {
            for (size_t i = 0; i < 16; ++i)
                if (data[i] != other.data[i])
                    return false;
            return true;
        }
    };
    foo f1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    foo f2;
    opt_memcpy(&f2, &f1);
    assert(f1 == f2);
}

int main()
{
    typedef ct_plus<ct_int<2>, ct_int<100>> ct_102;
    typedef ct_mul<ct_102, ct_int<1000>> ct_102000;
    typedef ct_int<1> ct_1;
    
    std::cout << ct_102::value << std::endl;
    std::cout << ct_102000::value << std::endl;
    std::cout << ct_1::value << std::endl;
    std::cout << ct_min<ct_102000, ct_1>::value << std::endl;
    std::cout << ct_fib<ct_int<10>>::value << std::endl;
    std::cout << fib(10) << std::endl;
    test_memcpy();
    return 0;
}
