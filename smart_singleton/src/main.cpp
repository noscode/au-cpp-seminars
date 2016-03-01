#include "singleton.h"
#include <iostream>
#include <cassert>

struct MySingletonService: Singleton<MyService>
{
    static size_t create_count_;
    MySingletonService()
    {
        std::cout << "MySingletonService::MySingletonService()"
            << std::endl;
        create_count_++;
    }

    ~MySingletonService()
    {
        std::cout << "MySingletonService::~MySingletonService()"
            << std::endl;
    }
};

size_t MySingletonService::create_count_ = 0;
template<> typename Singleton<MySingletonService>::wptr
    Singleton<MySingletonService>::instance_ = {};

int main()
{
    auto p1 = MySingletonService::get_instance();
    auto p2 = MySingletonService::get_instance();
    auto p3 = MySingletonService::get_instance();
    assert(MySingletonService::create_count_ == 1);
    p1.reset(); p2.reset(); p3.reset();
    assert(MySingletonService::create_count_ == 1);
    p1 = MySingletonService::get_instance();
    p2 = MySingletonService::get_instance();
    assert(MySingletonService::create_count_ == 2);
    return 0;
}
