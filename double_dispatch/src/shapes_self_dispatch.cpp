#include "shapes_self_dispatch.h"

void point::intersect(shape &other)
{
    other.intersect_impl(*this);
}

void point::intersect_impl(point &other)
{
    intersect_impl_print(*this, other);
}

void point::intersect_impl(rectangle &other)
{
    intersect_impl_print(*this, other);
}

void point::intersect_impl(circle &other)
{
    intersect_impl_print(*this, other);
}

void rectangle::intersect(shape &other)
{
    other.intersect_impl(*this);
}

void rectangle::intersect_impl(point &other)
{
    intersect_impl_print(*this, other);
}

void rectangle::intersect_impl(rectangle &other)
{
    intersect_impl_print(*this, other);
}

void rectangle::intersect_impl(circle &other)
{
    intersect_impl_print(*this, other);
}

void circle::intersect(shape &other)
{
    other.intersect_impl(*this);
}

void circle::intersect_impl(point &other)
{
    intersect_impl_print(*this, other);
}

void circle::intersect_impl(rectangle &other)
{
    intersect_impl_print(*this, other);
}

void circle::intersect_impl(circle &other)
{
    intersect_impl_print(*this, other);
}
