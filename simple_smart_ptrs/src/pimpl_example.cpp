#include "pimpl_example.h"
#include <iostream>

struct pimpl_example::pimpl
{
    //~pimpl();
};

/*pimpl_example::pimpl::~pimpl()
{
    std::cout << "pimpl destructor" << std::endl;
}*/

pimpl_example::~pimpl_example()
{}
