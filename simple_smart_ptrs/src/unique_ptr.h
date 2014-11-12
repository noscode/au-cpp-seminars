#pragma once
#include "scoped_ptr.h"

template<typename T>
struct unique_ptr: scoped_ptr<T>
{
    using scoped_ptr<T>::scoped_ptr;
    unique_ptr(unique_ptr &&src);
    unique_ptr& operator=(unique_ptr &&src);
};

template<typename T>
unique_ptr<T>::unique_ptr(unique_ptr<T> &&src)
{
    swap(*this, src);
}

template<typename T>
unique_ptr<T>& unique_ptr<T>::operator=(unique_ptr<T> &&src)
{
    (*this).reset();
    swap(*this, src);
    return *this;
}
