#pragma once
#include <iostream>

namespace imprecise
{
/*
 * ddouble:
 * Class representing floating point number with its accuracy.
 * All the operations done with ddouble variable change resulting
 * ddouble number accuracy.
 */
struct ddouble
{
    ddouble();
    ddouble(double value);
    ddouble(double value, double delta);

    float get_value() const;
    float get_delta() const;

    ddouble& operator+=(const ddouble& rhs);
    ddouble& operator-=(const ddouble& rhs);
    ddouble& operator*=(const ddouble& rhs);
    ddouble& operator/=(const ddouble& rhs);
    ddouble operator-();

private:
    double val_;
    double dv_;

    constexpr static double NUM_REPRESENT_DELTA = 10e-15;
};

ddouble operator+(const ddouble& lhs, const ddouble& rhs);
ddouble operator-(const ddouble& lhs, const ddouble& rhs);
ddouble operator*(const ddouble& lhs, const ddouble& rhs);
ddouble operator/(const ddouble& lhs, const ddouble& rhs);

bool operator<(const ddouble& lhs, const ddouble& rhs);
bool operator<=(const ddouble& lhs, const ddouble& rhs);
bool operator==(const ddouble& lhs, const ddouble& rhs);
bool operator>(const ddouble& lhs, const ddouble& rhs);
bool operator>=(const ddouble& lhs, const ddouble& rhs);

std::ostream& operator<<(std::ostream &out, const ddouble& what);
std::istream& operator>>(std::istream &in, ddouble& where);
} //namespace imprecise
