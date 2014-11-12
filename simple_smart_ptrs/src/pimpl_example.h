#pragma once
#include "scoped_ptr.h"

struct pimpl_example
{
    ~pimpl_example();
private:
    struct pimpl;
    scoped_ptr<pimpl> pimpl_ptr_;
};
