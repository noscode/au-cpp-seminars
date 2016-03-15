#include "shapes.h"

namespace impl
{

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

} // namespace impl

void point::intersect(shape &other)
{
    other.intersect_impl(*this);
}

void point::intersect_impl(point &other)
{
    impl::intersect(*this, other);
}

void point::intersect_impl(rectangle &other)
{
    impl::intersect(*this, other);
}

void point::intersect_impl(circle &other)
{
    impl::intersect(*this, other);
}

void rectangle::intersect(shape &other)
{
    other.intersect_impl(*this);
}

void rectangle::intersect_impl(point &other)
{
    impl::intersect(*this, other);
}

void rectangle::intersect_impl(rectangle &other)
{
    impl::intersect(*this, other);
}

void rectangle::intersect_impl(circle &other)
{
    impl::intersect(*this, other);
}

void circle::intersect(shape &other)
{
    other.intersect_impl(*this);
}

void circle::intersect_impl(point &other)
{
    impl::intersect(*this, other);
}

void circle::intersect_impl(rectangle &other)
{
    impl::intersect(*this, other);
}

void circle::intersect_impl(circle &other)
{
    impl::intersect(*this, other);
}
