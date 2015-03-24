#pragma once
#include <ostream>
#include <iterator>
#include <string>

template<class T>
struct ostream_iterator :
    std::iterator<std::output_iterator_tag, void,
        void, void, void>
{
    typedef std::ostream ostream_type;
    typedef ostream_iterator<T> my_type;

    explicit ostream_iterator(ostream_type& ostream);
    ostream_iterator(ostream_type& ostream, const std::string &delimeter);
    ostream_iterator(const my_type& src);
    ~ostream_iterator();

    my_type& operator*();
    my_type& operator=(const T &out_val);

    my_type& operator++();
    my_type operator++(int);
protected:
    std::ostream &out_stream_;
    std::string delimeter_;
};

template<typename T>
ostream_iterator<T>::ostream_iterator(ostream_type& ostream)
    : out_stream_(ostream)
    , delimeter_("")
{}

template<typename T>
ostream_iterator<T>::ostream_iterator(ostream_type& ostream,
        const std::string &delimeter)
    : out_stream_(ostream)
    , delimeter_(delimeter)
{}

template<typename T>
ostream_iterator<T>::ostream_iterator(const my_type& src)
    : out_stream_(src.out_stream_)
    , delimeter_(src.delimeter_)
{}

template<typename T>
ostream_iterator<T>::~ostream_iterator()
{}

template<typename T>
auto ostream_iterator<T>::operator=(const T &out_val) -> my_type&
{
    out_stream_ << out_val << delimeter_;
    return *this;
}

template<typename T>
auto ostream_iterator<T>::operator*() -> my_type&
{
    return *this;
}

template<typename T>
auto ostream_iterator<T>::operator++() -> my_type&
{
    return *this;
}

template<typename T>
auto ostream_iterator<T>::operator++(int) -> my_type
{
    return *this;
}
