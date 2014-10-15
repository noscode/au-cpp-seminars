#include "lazy_string_ix_modifier.h"
#include "lazy_string.h"

#include <iostream>

namespace lazy
{

lazy_string_ix_modifier::lazy_string_ix_modifier(lazy_string &str, size_t ix)
    : str_(str)
    , ix_(ix)
{}

lazy_string_ix_modifier::operator char() const
{
    return str_.get_at(ix_);
}

lazy_string_ix_modifier& lazy_string_ix_modifier::operator=(char val)
{
    str_.set_at(ix_, val);
    return *this;
}

} //namespace lazy
