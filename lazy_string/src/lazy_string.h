#pragma once

#include "shared_buffer.h"

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
private:
    explicit lazy_string(size_t size);

    //friend std::ostream& operator<<(std::ostream& os, const lazy_string& str);
    //friend lazy_string operator+(const lazy_string &str1, const lazy_string &str2);
    friend void print(std::ostream& os, const lazy_string& str);
    friend lazy_string concat(const lazy_string &str1, const lazy_string &str2);

    shared_buffer buf_;
};
