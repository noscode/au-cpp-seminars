#pragma once

#include "shared_buffer.h"
namespace lazy {

struct lazy_string
{
    lazy_string(const char *src);
    lazy_string(const lazy_string &src);
    lazy_string(lazy_string &&src);
    lazy_string& operator=(lazy_string src);

    bool empty() const;
    size_t get_size() const;
    char get_at(size_t ix) const;
    void set_at(size_t ix, char value);
    const char* c_str() const;
    lazy_string& operator+=(const lazy_string &src);

    static const size_t npos = static_cast<size_t>(-1);
private:
    explicit lazy_string(size_t buf_size);

    shared_buffer buf_;
};

lazy_string operator+(const lazy_string &str1, const lazy_string &str2);
bool operator<(const lazy_string &str1, const lazy_string &str2);
std::ostream& operator<<(std::ostream& os, const lazy_string& str);

} /* namespace lazy */
