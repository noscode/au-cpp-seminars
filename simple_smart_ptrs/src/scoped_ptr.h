#pragma once
#include <utility>
#include "noncopyable.h"

template<typename T>
struct scoped_ptr;

template<typename T>
void swap(scoped_ptr<T> &ptr1, scoped_ptr<T> &ptr2);

template<typename T>
struct scoped_ptr: noncopyable
{
    using element_type = T;

    explicit scoped_ptr(T *ptr=nullptr);
    ~scoped_ptr();
    void reset(T *ptr=nullptr);

    T& operator*() const;
    T* operator->() const;
    T* get() const;

    explicit operator bool() const;

    friend void swap<T>(scoped_ptr<T> &ptr1, scoped_ptr<T> &ptr2);
private:
    T *ptr_;
};

template<typename T>
scoped_ptr<T>::scoped_ptr(T *ptr)
    : ptr_(ptr)
{}

template<typename T>
scoped_ptr<T>::~scoped_ptr()
{
    reset();
}

template<typename T>
void scoped_ptr<T>::reset(T *ptr)
{
    delete ptr_;
    ptr_ = ptr;
}

template<typename T>
T& scoped_ptr<T>::operator*() const
{
    return *ptr_;
}

template<typename T>
T* scoped_ptr<T>::operator->() const
{
    return ptr_;
}

template<typename T>
T* scoped_ptr<T>::get() const
{
    return ptr_;
}

template<typename T>
scoped_ptr<T>::operator bool() const
{
    return get() != nullptr;
}

template<typename T>
void swap(scoped_ptr<T> &ptr1, scoped_ptr<T> &ptr2)
{
    std::swap(ptr1.ptr_, ptr2.ptr_);
}
