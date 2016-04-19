#include "singleton.h"
#include <iostream>
#include <cassert>

struct my_singleton_service_t: singleton_t<my_singleton_service_t>
{
    static size_t create_count_;
    my_singleton_service_t()
    {
        std::cout << "my_singleton_service_t::my_singleton_service_t()"
            << std::endl;
        create_count_++;
    }

    ~my_singleton_service_t()
    {
        std::cout << "my_singleton_service_t::~my_singleton_service_t()"
            << std::endl;
    }

    void say_hello()
    {
        std::cout << "Hello! I am the " << create_count_
            << " instance!" << std::endl;
    }
};

size_t my_singleton_service_t::create_count_ = 0;
template<> typename singleton_t<my_singleton_service_t>::wptr
    singleton_t<my_singleton_service_t>::instance_ = {};

int main()
{
    auto p1 = my_singleton_service_t::get_instance();
    auto p2 = my_singleton_service_t::get_instance();
    auto p3 = my_singleton_service_t::get_instance();
    assert(my_singleton_service_t::create_count_ == 1);
    p1->say_hello(); p2->say_hello(); p3->say_hello();
    p1.reset(); p2.reset(); p3.reset();
    assert(my_singleton_service_t::create_count_ == 1);
    p1 = my_singleton_service_t::get_instance();
    p2 = my_singleton_service_t::get_instance();
    assert(my_singleton_service_t::create_count_ == 2);
    p1->say_hello(); p2->say_hello();
    return 0;
}
