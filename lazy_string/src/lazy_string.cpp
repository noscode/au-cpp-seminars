#include <ostream>
#include <string.h>

#include "lazy_string.h"
namespace lazy {

lazy_string::lazy_string(size_t buf_size)
    : buf_(buf_size)
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

lazy_string& lazy_string::operator+=(const lazy_string &src)
{
    shared_buffer changed_buf(buf_.get_size() + src.get_size() + 1);
    memcpy(changed_buf.get_data(), buf_.get_data(), get_size());
    strcpy(changed_buf.get_data() + get_size(), src.c_str());
    buf_ = changed_buf;

    return *this;
}

lazy_string_ix_modifier lazy_string::operator[](size_t ix)
{
    return lazy_string_ix_modifier(*this, ix);
}

char lazy_string::operator[](size_t ix) const
{
    return get_at(ix);
}

bool operator<(const lazy_string &str1, const lazy_string &str2)
{
    return strcmp(str1.c_str(), str2.c_str()) < 0;
}

std::ostream& operator<<(std::ostream& os, const lazy_string& str)
{
    return os << str.c_str();
}

lazy_string operator+(const lazy_string &str1, const lazy_string &str2)
{
    lazy_string str_concat(str1);
    str_concat += str2;
    return str_concat;
}

} //namespace lazy
