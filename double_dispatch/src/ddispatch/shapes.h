#pragma once

struct shape
{
    shape(double x, double y)
        : x_(x)
        , y_(y)
    {}

    virtual ~shape()
    {}

protected:
    shape(const shape &src) = default;
    shape(shape &&src) = default;
    shape& operator=(const shape &src) = default;
    shape& operator=(shape &&src) = default;
private:
    double x_;
    double y_;
};

struct point : shape
{
    using shape::shape;
};

struct rectangle : shape
{
    rectangle(double left_x, double top_y,
        double width, double height)
        : shape(left_x, top_y)
        , width_(width)
        , height_(height)
    {}

private:
    double width_;
    double height_;
};

struct circle : shape
{
    circle(double center_x, double center_y, double radius)
        : shape(center_x, center_y)
        , radius_(radius)
    {}
private:
    double radius_;
};

// Polymorphic by 2 runtime types function and
// its initialization
void shape_init_intersect_dd();
void intersect(shape &sh1, shape& sh2);
