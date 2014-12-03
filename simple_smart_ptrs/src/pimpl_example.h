#pragma once
#include "scoped_ptr.h"
#include "shared_ptr.h"

struct pimpl_example
{
    pimpl_example();
    ~pimpl_example();
private:
    struct pimpl;
    shared_ptr<pimpl> pimpl_ptr_;
};
