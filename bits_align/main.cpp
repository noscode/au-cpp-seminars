#include "bits_align.h"
#include <iostream>

int main()
{
    using namespace std;
    using namespace bits_align;
    align_uint<0>::type bits_0_aligned = 0x0; //type is uint8_t
    align_uint<8>::type bits_8_aligned = 0xFF; //type is uint8_t
    align_uint<16>::type bits_16_aligned = 0xFFFF; //type is uint16_t
    align_uint<20>::type bits_20_aligned = 0xFFFFF; //type is uint32_t
    align_uint<24>::type bits_24_aligned = 0xFFFFFF; //type is uint32_t
    align_uint<32>::type bits_32_aligned = 0xFFFFFFFF; //type is uint32_t
    align_uint<44>::type bits_44_aligned = 0xFFFFFFFFFFF; //type is uint64_t
    align_uint<64>::type bits_64_aligned = 0xFFFFFFFFFFFFFFFFULL; //type is uint64_t
    //align_uint<65>::type bits_65_aligned = 0xFFFFFFFFFFF; //compilation error

    std::cout
        << "bits 0. size: " << sizeof(bits_0_aligned)
        << " val.: " << bits_0_aligned << std::endl
        << "bits 8. size: " << sizeof(bits_8_aligned)
        << " val.: " << bits_8_aligned << std::endl
        << "bits 16. size: " << sizeof(bits_16_aligned)
        << " val.: " << bits_16_aligned << std::endl
        << "bits 20. size: " << sizeof(bits_20_aligned)
        << " val.: " << bits_20_aligned << std::endl
        << "bits 24. size: " << sizeof(bits_24_aligned)
        << " val.: " << bits_24_aligned << std::endl
        << "bits 32. size: " << sizeof(bits_32_aligned)
        << " val.: " << bits_32_aligned << std::endl
        << "bits 44. size: " << sizeof(bits_44_aligned)
        << " val.: " << bits_44_aligned << std::endl
        << "bits 64. size: " << sizeof(bits_64_aligned)
        << " val.: " << bits_64_aligned << std::endl;
    return 0;
}
