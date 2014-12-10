#pragma once
#include "shapes.h"
#include <typeinfo>

template<typename T, typename U>
void intersect_impl_print(T &left, U &right)
{
    std::cout << "Completed dispatching of "
        << typeid(T).name() << " and " 
        << typeid(U).name()
        << std::endl;
}

template<typename ARG1, typename ARG2>
void intersect(ARG1 &arg1, ARG2 &arg2)
{
    intersect_impl_print(arg1, arg2);
}
