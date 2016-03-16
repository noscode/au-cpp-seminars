#pragma once
#include <istream>
#include <iterator>

template<class T>
struct istream_iterator :
    std::iterator<std::input_iterator_tag, T,
        std::ptrdiff_t, const T*, const T&>
{
    typedef std::istream istream_type;
    typedef istream_iterator<T> my_type;

    istream_iterator();
    explicit istream_iterator(istream_type& istream);
    istream_iterator(const my_type& src);
    ~istream_iterator();

    const T& operator*() const;
    const T* operator->() const;

    my_type& operator++();
    my_type operator++(int);

    bool operator==(const my_type &other) const;
    bool operator!=(const my_type &other) const;

protected:
    std::istream *in_stream_;
    T current_value_;
};

template<typename T>
istream_iterator<T>::istream_iterator()
    : in_stream_(nullptr)
{}

template<typename T>
istream_iterator<T>::istream_iterator(istream_type& istream)
    : in_stream_(&istream)
{
    ++*this;
}

template<typename T>
istream_iterator<T>::istream_iterator(const my_type& src)
    : in_stream_(src.in_stream_)
    , current_value_(src.current_value_)
{}

template<typename T>
istream_iterator<T>::~istream_iterator()
{}

template<typename T>
const T& istream_iterator<T>::operator*() const
{
    return current_value_;
}

template<typename T>
const T* istream_iterator<T>::operator->() const
{
    return &current_value_;
}

template<typename T>
auto istream_iterator<T>::operator++() -> my_type&
{
    if (in_stream_ && !(*in_stream_ >> current_value_))
    {
        in_stream_ = nullptr;
    }
    return *this;
}

template<typename T>
auto istream_iterator<T>::operator++(int) -> my_type
{
    my_type tmp = *this;
    ++*this;
    return tmp;
}

template<typename T>
bool istream_iterator<T>::operator==(const my_type &other) const
{
    return (in_stream_ == nullptr
        && other.in_stream_ == nullptr);
}

template<typename T>
bool istream_iterator<T>::operator!=(const my_type &other) const
{
    return !(*this == other);
}
