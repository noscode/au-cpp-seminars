#include <iostream>
#include <typeinfo>

#include "shapes.h"
#include "double_dispatcher.h"

template<typename T, typename U>
void intersect_print(T &left, U &right)
{
    std::cout << "Completed dispatching of "
        << typeid(T).name() << " and "
        << typeid(U).name()
        << std::endl;
}

struct intersect_dd_tag;
typedef double_dispatcher<shape, intersect_dd_tag> intersect_dd_t;

void shape_init_intersect_dd()
{
    intersect_dd_t::reg(intersect_print<point, point>);
    intersect_dd_t::reg(intersect_print<point, rectangle>);
    intersect_dd_t::reg(intersect_print<point, circle>);
    intersect_dd_t::reg(intersect_print<rectangle, rectangle>);
    intersect_dd_t::reg(intersect_print<rectangle, circle>);
    intersect_dd_t::reg(intersect_print<circle, circle>);
}

void intersect(shape &sh1, shape& sh2)
{
    intersect_dd_t::call(sh1, sh2);
}
