#include "shapes_self_dispatch.h"

int main()
{
    point p(0, 0);
    rectangle r(0, 0, 0, 0);
    circle c(0, 0, 0);

    shape &ps = p;
    shape &pr = r;
    shape &pc = c;

    ps.intersect(pr);
    ps.intersect(ps);
    ps.intersect(pc);

    pr.intersect(pr);
    pr.intersect(ps);
    pr.intersect(pc);

    pc.intersect(pr);
    pc.intersect(ps);
    pc.intersect(pc);
    return 0;
}
