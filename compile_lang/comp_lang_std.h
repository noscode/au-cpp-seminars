#pragma once
#include "comp_lang.h"

template<typename CTLOP, typename CTROP>
struct ct_min
{
    typedef ct_if<ct_less<CTLOP, CTROP>, CTLOP, CTROP> value_type;
    static constexpr auto value = value_type::value;
};

template<typename CTLOP, typename CTROP>
struct ct_max
{
    typedef ct_if<ct_great<CTLOP, CTROP>, CTLOP, CTROP> value_type;
    static constexpr auto value = value_type::value;
};

template<typename CT_MIN_EXPR, typename CT_VAL, typename CT_MAX_EXPR>
struct ct_is_inclusive
{
    static constexpr auto value =
        ct_and<
            ct_equal<ct_min<CT_MIN_EXPR, CT_VAL>, CT_MIN_EXPR>,
            ct_equal<ct_max<CT_VAL, CT_MAX_EXPR>, CT_MAX_EXPR>
        >::value;
};

template<typename CT_N>
struct ct_fib
{
    static_assert(ct_great<CT_N, ct_int<-1>>::value, "Fib number should be >= 0");
    typedef ct_dec<CT_N> CT_NPREV;
    typedef ct_dec<CT_NPREV> CT_NPPREV;

    static constexpr auto value =
        ct_if<ct_equal<CT_N, ct_int<0>>, ct_int<0>,
            ct_if<ct_equal<CT_N, ct_int<1>>, ct_int<1>,
                 ct_plus< ct_fib<CT_NPPREV>, ct_fib<CT_NPREV> > >>::value;
};

inline constexpr int fib(int n)
{
    return n == 0 ? 0 : (n == 1 ? 1 : (fib(n - 1) + fib(n - 2)));
}
