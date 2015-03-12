#pragma once
#include <utility>
#include <functional>

template<typename T>
struct shared_ptr;

template<typename T>
void swap(shared_ptr<T> &ptr1, shared_ptr<T> &ptr2);

template<typename T>
void default_delete_func(T *ptr);

template<typename T>
struct shared_ptr
{
    typedef std::function<void(T*)> del_func;

    explicit shared_ptr(T *ptr=nullptr,
        del_func deleter=default_delete_func<T>);
    shared_ptr(const shared_ptr<T> &src);
    shared_ptr(shared_ptr<T> &&src);
    shared_ptr<T>& operator=(const shared_ptr<T> &src);
    ~shared_ptr();

    void reset(T *ptr=nullptr,
        del_func deleter=default_delete_func<T>);
    T& operator*() const;
    T* operator->() const;
    T* get() const;

    explicit operator bool() const;

    friend void swap<T>(shared_ptr<T> &ptr1, shared_ptr<T> &ptr2);
private:
    struct shared_data
    {
        size_t ref_cnt_;
        T *ptr_;
    };
    shared_data *shared_;

    void add_ref(shared_data *shared);
};

template<typename T>
shared_ptr<T>::shared_ptr(T *ptr,
    del_func deleter)
{
    add_ref(new shared_data{0, 0, ptr, deleter});
}

template<typename T>
shared_ptr<T>::shared_ptr(const shared_ptr<T> &src)
{
    add_ref(src.shared_);
}

template<typename T>
shared_ptr<T>::shared_ptr(shared_ptr<T> &&src)
    : shared_ptr()
{
    std::swap(shared_, src.shared_);
}

template<typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<T> &src)
{
    shared_ptr<T> tmp;
    swap(*this, tmp);
    add_ref(src.shared_);
    return *this;
}

template<typename T>
shared_ptr<T>::~shared_ptr()
{
    if (!shared_)
        return;
    if (!(--shared_->ref_cnt_))
    {
        shared_->deleter_(static_cast<T*>(shared_->ptr_));
        if (!shared_->weak_ref_cnt_)
        {
            delete shared_;
        }
    }
    shared_ = nullptr;
}

template<typename T>
void shared_ptr<T>::add_ref(shared_ptr<T>::shared_data *shared)
{
    shared_ = shared;
    if (shared_)
    {
        shared_->ref_cnt_++;
    }
}

template<typename T>
void shared_ptr<T>::reset(T *ptr, del_func deleter)
{
    shared_ptr<T> tmp;
    swap(*this, tmp);
    shared_->ptr_ = ptr;
    shared_->deleter_ = deleter;
}

template<typename T>
T& shared_ptr<T>::operator*() const
{
    return *static_cast<T*>(shared_->ptr_);
}

template<typename T>
T* shared_ptr<T>::operator->() const
{
    return static_cast<T*>(shared_)->ptr_;
}

template<typename T>
T* shared_ptr<T>::get() const
{
    return shared_ ? static_cast<T*>(shared_->ptr_) : nullptr;
}

template<typename T>
shared_ptr<T>::operator bool() const
{
    return get() != nullptr;
}

template<typename T>
void swap(shared_ptr<T> &ptr1, shared_ptr<T> &ptr2)
{
    std::swap(ptr1.shared_, ptr2.shared_);
}

template<typename T>
void default_delete_func(T *ptr)
{
    delete ptr;
}
