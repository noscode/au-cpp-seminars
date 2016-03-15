#pragma once
#include <iostream>
#include <typeinfo>

struct point;
struct rectangle;
struct circle;

struct shape
{
    shape(double x, double y)
        : x_(x)
        , y_(y)
    {}

    virtual void intersect(shape &other) = 0;
    virtual void intersect_impl(point &other) = 0;
    virtual void intersect_impl(rectangle &other) = 0;
    virtual void intersect_impl(circle &other) = 0;

    template<typename T, typename U>
    static void intersect_impl_print(T &left, U &right)
    {
        std::cout << "Completed dispatching of "
            << typeid(T).name() << " and " 
            << typeid(U).name()
            << std::endl;
    }

private:
    double x_;
    double y_;
};

struct point : shape
{
    point(double x, double y)
        : shape(x, y)
    {}
    void intersect(shape &other) override;
    void intersect_impl(point &other) override;
    void intersect_impl(rectangle &other) override;
    void intersect_impl(circle &other) override;
};

struct rectangle : shape
{
    rectangle(double left_x, double top_y,
        double width, double height)
        : shape(left_x, top_y)
        , width_(width)
        , height_(height)
    {}

    void intersect(shape &other) override;
    void intersect_impl(point &other) override;
    void intersect_impl(rectangle &other) override;
    void intersect_impl(circle &other) override;
private:
    double width_;
    double height_;
};

struct circle : shape
{
    circle(double center_x, double center_y, double radius)
        : shape(center_x, center_y)
    {}

    void intersect(shape &other) override;
    void intersect_impl(point &other) override;
    void intersect_impl(rectangle &other) override;
    void intersect_impl(circle &other) override;
private:
    double radius_;
};

inline void intersect(shape &sh1, shape &sh2)
{
    sh1.intersect(sh2);
}
