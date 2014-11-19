#include "pimpl_example.h"
#include <iostream>

struct pimpl_example::pimpl
{
    ~pimpl();
};

pimpl_example::pimpl::~pimpl()
{
    std::cout << "pimpl destructor" << std::endl;
}

void pimpl_example::pimpl_deleter(pimpl *ptr)
{
    delete ptr;
}

pimpl_example::pimpl_example()
    : pimpl_ptr_(new pimpl, pimpl_deleter)
{}

pimpl_example::~pimpl_example()
{}
