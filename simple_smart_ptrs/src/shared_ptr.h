#pragma once
#include <utility>
#include <functional>

template<typename T>
struct shared_ptr;

template<typename T>
struct weak_ptr;

template<typename T>
void swap(shared_ptr<T> &ptr1, shared_ptr<T> &ptr2);

template<typename T>
void default_delete_func(T *ptr);

template<typename T>
struct shared_ptr
{
    typedef std::function<void(T*)> del_func;

    shared_ptr();
    explicit shared_ptr(T *ptr,
        del_func deleter=default_delete_func<T>);
    explicit shared_ptr(const weak_ptr<T> &src);
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
        size_t weak_ref_cnt_;
        void *ptr_;
        del_func deleter_;
    };
    shared_data *shared_;

    void add_ref(shared_data *shared);
    friend class weak_ptr<T>;
};

template<typename T>
void swap(weak_ptr<T> &ptr1, weak_ptr<T> &ptr2);

template<typename T>
struct weak_ptr
{
    constexpr weak_ptr();
    template<typename Y>
    weak_ptr(const weak_ptr<Y> &src);
    template<typename Y>
    weak_ptr(const shared_ptr<Y> &src);

    template<typename Y>
    weak_ptr& operator=(const weak_ptr<Y> &src);
    template<typename Y>
    weak_ptr& operator=(const shared_ptr<Y> &src);

    ~weak_ptr();

    void reset();
    size_t use_count() const;
    bool expired() const { return use_count() == 0; }
    explicit operator bool() const { return !expired(); }
    shared_ptr<T> lock() const { return shared_ptr<T>(*this); }

    friend void swap<T>(weak_ptr<T> &ptr1, weak_ptr<T> &ptr2);
private:
    typedef typename shared_ptr<T>::shared_data shared_data;
    shared_data *shared_;
    void add_ref(shared_data *shared);
    friend class shared_ptr<T>;
};

template<typename T>
shared_ptr<T>::shared_ptr()
    : shared_ptr(nullptr, [](T *ptr) {})
{}

template<typename T>
shared_ptr<T>::shared_ptr(T *ptr, del_func deleter)
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
shared_ptr<T>::shared_ptr(const weak_ptr<T> &src)
{
    add_ref(src.shared_);
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
    //Check for complete type. See checked delete idiom.
    static_assert(sizeof(*ptr) > 0, "Checked delete");
    delete ptr;
}

template<typename T>
constexpr weak_ptr<T>::weak_ptr()
    : shared_(nullptr)
{}

//Better to implement as SFINAE
//overload matching predicate
template<typename SRC, typename DST>
void assert_pointers_convertable()
{
    SRC *psrc = nullptr;
    DST *pdst = psrc;
    (void)psrc; (void)pdst;
}

template<typename T>
template<typename Y>
weak_ptr<T>::weak_ptr(const weak_ptr<Y> &src)
{
    assert_pointers_convertable<Y, T>();
    add_ref(src.shared_);
}

template<typename T>
template<typename Y>
weak_ptr<T>::weak_ptr(const shared_ptr<Y> &src)
{
    assert_pointers_convertable<Y, T>();
    add_ref(src.shared_);
}

template<typename T>
template<typename Y>
weak_ptr<T>& weak_ptr<T>::operator=(const weak_ptr<Y> &src)
{
    assert_pointers_convertable<Y, T>();
    add_ref(src.shared_);
}

template<typename T>
template<typename Y>
weak_ptr<T>& weak_ptr<T>::operator=(const shared_ptr<Y> &src)
{
    assert_pointers_convertable<Y, T>();
    add_ref(src.shared_);
}

template<typename T>
void weak_ptr<T>::add_ref(weak_ptr<T>::shared_data *shared)
{
    shared_ = shared;
    if (shared_)
    {
        shared_->weak_ref_cnt_++;
    }
}

template<typename T>
weak_ptr<T>::~weak_ptr()
{
    if (!shared_)
        return;
    --shared_->weak_ref_cnt_;
    if (!shared_->weak_ref_cnt_ &&
        !shared_->ref_cnt_)
    {
        delete shared_;
    }
    shared_ = nullptr;
}

template<typename T>
void weak_ptr<T>::reset()
{
    weak_ptr<T> tmp;
    swap(*this, tmp);
}

template<typename T>
size_t weak_ptr<T>::use_count() const
{
    if (!shared_)
        return 0;
    return shared_->ref_cnt_;
}

template<typename T>
void swap(weak_ptr<T> &ptr1, weak_ptr<T> &ptr2)
{
    std::swap(ptr1.shared_, ptr2.shared_);
}
