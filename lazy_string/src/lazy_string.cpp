#include <ostream>
#include <string.h>

#include "lazy_string.h"

lazy_string::lazy_string(size_t size)
    : buf_(size)
{}

lazy_string::lazy_string(const char *src)
    : lazy_string(strlen(src) + 1)
{
    memcpy(buf_.get_data(), src, buf_.get_size());
}

lazy_string::lazy_string(const lazy_string &src)
    : buf_(src.buf_)
{}

lazy_string::lazy_string(lazy_string &&src)
    : buf_(src.buf_)
{}

bool lazy_string::empty() const
{
    return buf_.get_size() == 1;
}

size_t lazy_string::get_size() const
{
    return buf_.get_size() - 1;
}

char lazy_string::get_at(size_t ix) const
{
    return buf_.get_data()[ix];
}

const char* lazy_string::c_str() const
{
    return buf_.get_data();
}

//std::ostream& operator<<(std::ostream& os, const lazy_string& str)
void print(std::ostream& os, const lazy_string& str)
{
    for (size_t i = 0; i < str.buf_.get_size(); ++i)
    {
        os << str.buf_.get_data()[i];
    }
    //return os;
}

//lazy_string operator+(const lazy_string &str1, const lazy_string &str2)
lazy_string concat(const lazy_string &str1, const lazy_string &str2)
{
    lazy_string str_concat(str1.get_size() + str2.get_size());
    memcpy(str_concat.buf_.get_data(), str1.buf_.get_data(), str1.get_size());
    memcpy(str_concat.buf_.get_data() + str1.get_size(), str2.buf_.get_data(), str2.get_size());
    return str_concat;
}

// mutability
lazy_string& lazy_string::operator=(lazy_string src)
{
    std::swap(buf_, src.buf_);
    return *this;
}

void lazy_string::set_at(size_t ix, char value)
{
    shared_buffer changed_buf(buf_.get_size());
    memcpy(changed_buf.get_data(), buf_.get_data(), buf_.get_size());
    changed_buf.get_data()[ix] = value;
    buf_ = changed_buf;
}
