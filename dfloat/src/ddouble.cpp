#include <algorithm>
#include <cmath>

#include "ddouble.h"

namespace imprecise
{

constexpr double ddouble::NUM_REPRESENT_DELTA;

ddouble::ddouble()
    : ddouble(0.0)
{}

ddouble::ddouble(double value)
    : ddouble(value, 0.0)
{}

ddouble::ddouble(double value, double delta)
    : val_(value)
    , dv_(std::max(delta, std::abs(value * NUM_REPRESENT_DELTA)))
{}

float ddouble::get_value() const
{
    return val_;
}

float ddouble::get_delta() const
{
    return dv_;
}

ddouble& ddouble::operator+=(const ddouble& rhs)
{
    val_ += rhs.val_;
    dv_ += rhs.dv_;
    return *this;
}

ddouble& ddouble::operator-=(const ddouble& rhs)
{
    val_ -= rhs.val_;
    dv_ += rhs.dv_;
    return *this;
}

ddouble& ddouble::operator*=(const ddouble& rhs)
{
    val_ *= rhs.val_;
    //d(a)*b + d(b)*a
    dv_ = dv_ * rhs.val_ + rhs.dv_ * val_;
    return *this;
}

ddouble& ddouble::operator/=(const ddouble& rhs)
{
    //d(a)/b + ((a*d(b))/(b*b))
    val_ /= rhs.val_;
    dv_ = dv_ / rhs.val_ + ((val_ * rhs.dv_) / (rhs.val_ * rhs.val_));
    return *this;
}

ddouble operator-(const ddouble &src)
{
    ddouble new_val(-src.get_value(), src.get_delta());
    return new_val;
}

ddouble operator+(const ddouble& lhs, const ddouble& rhs)
{
    ddouble res(lhs);
    return res += rhs;
}

ddouble operator-(const ddouble& lhs, const ddouble& rhs)
{
    ddouble res(lhs);
    return res -= rhs;
}

ddouble operator*(const ddouble& lhs, const ddouble& rhs)
{
    ddouble res(lhs);
    return res *= rhs;
}

ddouble operator/(const ddouble& lhs, const ddouble& rhs)
{
    ddouble res(lhs);
    return res /= rhs;
}

bool operator<(const ddouble& lhs, const ddouble& rhs)
{
    double maxl = lhs.get_value() + lhs.get_delta();
    double minr = rhs.get_value() - rhs.get_delta();
    return maxl < minr;
}

bool operator<=(const ddouble& lhs, const ddouble& rhs)
{
    return lhs == rhs || lhs < rhs;
}

bool operator==(const ddouble& lhs, const ddouble& rhs)
{
    double minl = lhs.get_value() - lhs.get_delta();
    double maxl = lhs.get_value() + lhs.get_delta();
    double minr = rhs.get_value() - rhs.get_delta();
    double maxr = rhs.get_value() + rhs.get_delta();
    return ((minl <= minr) && (minr <= maxl)) || ((minl <= maxr) && (maxr <= maxl));
}

bool operator>(const ddouble& lhs, const ddouble& rhs)
{
    return !(lhs == rhs) && !(lhs < rhs);
}

bool operator>=(const ddouble& lhs, const ddouble& rhs)
{
    return !(lhs < rhs);
}

std::ostream& operator<<(std::ostream &out, const ddouble& what)
{
    out << what.get_value() << " " << what.get_delta() << " ";
    return out;
}

std::istream& operator>>(std::istream &in, ddouble& where)
{
    double value, delta;
    char ch;
    in >> value >> ch >> delta >> ch;
    where = ddouble(value, delta);
    return in;
}

} // namespace imprecise
