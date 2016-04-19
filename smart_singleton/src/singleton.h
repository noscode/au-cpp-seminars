#pragma once
#include <memory>

template<class T>
struct singleton_t
{
    typedef std::shared_ptr<T> ptr;
    static ptr get_instance();
private:
    typedef std::weak_ptr<T> wptr;
    static wptr instance_;
};

template<class T>
auto singleton_t<T>::get_instance() -> ptr
{
    ptr p = instance_.lock();
    if (!p)
    {
        p = std::make_shared<T>();
        instance_ = p;
    }
    return p;
}
