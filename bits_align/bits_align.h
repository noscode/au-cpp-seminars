#pragma once
#include <cstdlib>
#include <stdint.h>

namespace bits_align
{

template<size_t bits_num>
struct align_uint
{   
    static_assert(bits_num <= 64, "bits_num is in uint size bounds");
    typedef typename align_uint<bits_num + 1>::type type;
};

template<> struct align_uint<8>  { typedef uint8_t  type; };
template<> struct align_uint<16> { typedef uint16_t type; };
template<> struct align_uint<32> { typedef uint32_t type; };
template<> struct align_uint<64> { typedef uint64_t type; };

}; //namespace bits_align
