#include <iostream>
#include <fstream>

#include "ddouble.h"

static void write_ddouble(const imprecise::ddouble& val)
{
    std::ofstream file("/tmp/ddouble.txt");
    file << val;
}

static imprecise::ddouble read_ddouble()
{
    imprecise::ddouble d;
    std::ifstream file("/tmp/ddouble.txt");
    file >> d;
    return d;
}

int main()
{
    using namespace imprecise;

    ddouble d(1.25e10, 1e-10);
    write_ddouble(d);
    std::cout << "original: " << d << std::endl;
    std::cout << "restored: " << read_ddouble() << std::endl;
    return 0;
}
