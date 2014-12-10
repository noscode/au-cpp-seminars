#include <iostream>
#include "shapes.h"
#include "shapes_intersect.h"
#include "double_dispatch.h"

template<typename BASE_TYPE, typename TAG>
typename double_dispatcher<BASE_TYPE, TAG>::dd_map_type
    double_dispatcher<BASE_TYPE, TAG>::dd_map;

int main()
{
    /*
    void intersect(point &pl, point &pr);
    void intersect(point &p, rectangle &r);
    void intersect(point &p, circle &c);
    void intersect(rectangle &r, circle &c);
    void intersect(rectangle &rl, rectangle &rr);
    void intersect(circle &cl, circle &cr);
    */
    double_dispatcher<shape>::reg(intersect<point, point>);
    double_dispatcher<shape>::reg(intersect<point, rectangle>);
    double_dispatcher<shape>::reg(intersect<point, circle>);
    double_dispatcher<shape>::reg(intersect<rectangle, rectangle>);
    double_dispatcher<shape>::reg(intersect<rectangle, circle>);
    double_dispatcher<shape>::reg(intersect<circle, circle>);

    point p(0, 0);
    rectangle r(0, 0, 0, 0);
    circle c(0, 0, 0);

    shape &sp = p;
    shape &sr = r;
    shape &sc = c;

    double_dispatcher<shape>::call(sp, sr);
    double_dispatcher<shape>::call(sp, sp);
    double_dispatcher<shape>::call(sp, sc);

    double_dispatcher<shape>::call(sr, sr);
    double_dispatcher<shape>::call(sr, sp);
    double_dispatcher<shape>::call(sr, sc);

    double_dispatcher<shape>::call(sc, sr);
    double_dispatcher<shape>::call(sc, sp);
    double_dispatcher<shape>::call(sc, sc);

    return 0;
}
