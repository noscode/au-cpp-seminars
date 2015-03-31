#pragma once
#include <type_traits>

template<int VAL>
struct ct_int
{
    static constexpr int value = VAL;
};

template<typename CTLOP, typename CTROP>
struct ct_plus
{
    static constexpr auto value = CTLOP::value + CTROP::value;
};

template<typename CT_EXPR>
using ct_inc = ct_plus<CT_EXPR, ct_int<1>>;

template<typename CTLOP, typename CTROP>
struct ct_minus
{
    static constexpr auto value = CTLOP::value - CTROP::value;
};

template<typename CT_EXPR>
using ct_dec = ct_minus<CT_EXPR, ct_int<1>>;

template<typename CT_EXPR>
struct ct_unminus
{
    static constexpr auto value = -CT_EXPR::value;
};

template<typename CTLOP, typename CTROP>
struct ct_mul
{
    static constexpr auto value = CTLOP::value * CTROP::value;
};

template<typename CTLOP, typename CTROP>
struct ct_div
{
    static constexpr auto value = CTLOP::value / CTROP::value;
};

template<typename CTLOP, typename CTROP>
struct ct_less
{
    static constexpr auto value = CTLOP::value < CTROP::value;
};

template<typename CTLOP, typename CTROP>
struct ct_great
{
    static constexpr auto value = CTLOP::value > CTROP::value;
};

template<typename CTLOP, typename CTROP>
struct ct_equal
{
    static constexpr auto value = CTLOP::value == CTROP::value;
};

namespace {
    template<bool ct_false, typename CT_TRUE_OP, typename CT_FALSE_OP>
    struct ct_if_helper
    {
        static constexpr auto value = CT_FALSE_OP::value;
    };

    template<typename CT_TRUE_OP, typename CT_FALSE_OP>
    struct ct_if_helper<true, CT_TRUE_OP, CT_FALSE_OP>
    {
        static constexpr auto value = CT_TRUE_OP::value;
    };
}

template<typename CT_COND_OP, typename CT_TRUE_OP, typename CT_FALSE_OP>
struct ct_if
{
    static constexpr auto value =
        ct_if_helper<CT_COND_OP::value, CT_TRUE_OP, CT_FALSE_OP>::value;
};

template<typename CT_LCOND_OP, typename CT_RCOND_OP>
struct ct_or
{
    static constexpr auto value =
        CT_LCOND_OP::value || CT_RCOND_OP::value;
};

template<typename CT_LCOND_OP, typename CT_RCOND_OP>
struct ct_and
{
    static constexpr auto value =
        CT_LCOND_OP::value && CT_RCOND_OP::value;
};
