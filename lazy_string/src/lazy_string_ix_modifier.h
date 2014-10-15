#pragma once
#include <cstddef>

namespace lazy
{

struct lazy_string;
struct lazy_string_ix_modifier
{
    operator char() const;
    lazy_string_ix_modifier& operator=(char val);

private:
    lazy_string_ix_modifier(lazy_string &str, size_t ix);

    lazy_string &str_;
    size_t ix_;

    friend class lazy_string;
};

} //namespace lazy
