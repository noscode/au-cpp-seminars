#pragma once

namespace my {

template<class T, size_t SIZE>
struct array {
    array(const T& src = T());
    array(const array &src);
    array& operator=(const array &src);
    ~array();

    T& operator[](size_t ix);
    const T& operator[](size_t ix) const;
    T* data();
    constexpr bool empty();
    constexpr size_t size();
private:
    alignas(T) char data_[SIZE * sizeof(T)];
};

template<class T, size_t SIZE>
array<T, SIZE>::array(const T& src)
{
    for (size_t i = 0; i < SIZE; ++i)
    {
        new (data() + i)T(src);
    }
}

template<class T, size_t SIZE>
array<T, SIZE>::array(const array &src)
{
    for (size_t i = 0; i < SIZE; ++i)
    {
        new (data() + i)T(src.data()[i]);
    }
}

template<class T, size_t SIZE>
auto array<T, SIZE>::operator=(const array& src) -> array&
{
    if (this == &src) return *this;

    for (size_t i = 0; i < SIZE; ++i)
    {
        data()[i] = src.data()[i];
    }
    return *this;
}

template<class T, size_t SIZE>
array<T, SIZE>::~array()
{
    for (size_t i = 0; i < SIZE; ++i)
    {
        data()[i].~T();
    }
}

template<class T, size_t SIZE>
T& array<T, SIZE>::operator[](size_t ix)
{
    return data()[ix];
}

template<class T, size_t SIZE>
const T& array<T, SIZE>::operator[](size_t ix) const
{
    return data()[ix];
}

template<class T, size_t SIZE>
T* array<T, SIZE>::data()
{
    void *data_void = static_cast<void*>(data_);
    T* data_t = static_cast<T*>(data_void);
    return data_t;
}

template<class T, size_t SIZE>
constexpr bool array<T, SIZE>::empty()
{
    return SIZE == 0;
}

template<class T, size_t SIZE>
constexpr size_t array<T, SIZE>::size()
{
    return SIZE;
}

} // namespace my
