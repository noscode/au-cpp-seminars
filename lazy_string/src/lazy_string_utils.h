#pragma once
#include "lazy_string.h"
#include <string.h>

namespace lazy
{
    size_t find(const lazy_string &in, const char *what, size_t start_ix = 0);
}

inline size_t lazy::find(const lazy_string &in, const char *what, size_t start_ix)
{
    const char *cstr_in_start = in.c_str() + start_ix;
    const char *cstr_in_what = strstr(cstr_in_start, what);

    return cstr_in_what ? cstr_in_what - in.c_str() : lazy_string::npos;
}
