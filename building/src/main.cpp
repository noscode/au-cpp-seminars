#include <iostream>
#include "add.hpp"

extern int global_a;

int main()
{
    std::cout << add(A1, A2) << std::endl;
    std::cout << get_a() << std::endl;
    return 0;
}
