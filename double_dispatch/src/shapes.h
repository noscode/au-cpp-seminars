#pragma once

struct shape
{
    shape(double x, double y)
        : x_(x)
        , y_(y)
    {}

    virtual ~shape()
    {}
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
    {}
private:
    double radius_;
};
