#include "gimiqs.hpp"

#include <iostream>
#include <type_traits>

#define _BYTE1(x) (  x        & 0xFF )
#define _BYTE2(x) ( (x >>  8) & 0xFF )
#define _BYTE3(x) ( (x >> 16) & 0xFF )
#define _BYTE4(x) ( (x >> 24) & 0xFF )
#define _BYTE5(x) ( (x >> 32) & 0xFF )
#define _BYTE6(x) ( (x >> 40) & 0xFF )
#define _BYTE7(x) ( (x >> 48) & 0xFF )
#define _BYTE8(x) ( (x >> 56) & 0xFF )

#define BYTE_SWAP_16(x) ((uint16_t)( _BYTE1(x)<<8 | _BYTE2(x) ))
#define BYTE_SWAP_32(x) ((uint32_t)( _BYTE1(x)<<24 | _BYTE2(x)<<16 | _BYTE3(x)<<8 | _BYTE4(x) ))
#define BYTE_SWAP_64(x) ((uint64_t)( _BYTE1(x)<<56 | _BYTE2(x)<<48 | _BYTE3(x)<<40 | _BYTE4(x)<<32 | _BYTE5(x)<<24 | _BYTE6(x)<<16 | _BYTE7(x)<<8 | _BYTE8(x) ))

int main() {
    std::cout << "hello, piq." << std::endl;

    gimiqs::Briq b {0};

    b.L = BYTE_SWAP_64(0x0123456789ABCDEF);
    b.H = BYTE_SWAP_64(0xFEDCBA9876543210);

    printf("piq1: %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX\n", b.LLLL, b.LLLH, b.LLHL, b.LLHH, b.LHLL, b.LHLH, b.LHHL, b.LHHH, b.HLLL, b.HLLH, b.HLHL, b.HLHH, b.HHLL, b.HHLH, b.HHHL, b.HHHH);
    printf("piq2: %04hX %04hX %04hX %04hX %04hX %04hX %04hX %04hX\n", b.LLL, b.LLH, b.LHL, b.LHH, b.HLL, b.HLH, b.HHL, b.HHH);
    printf("piq3: %08X %08X %08X %08X\n", b.LL, b.LH, b.HL, b.HH);
    printf("piq4: %016lX %016lX\n", b.L, b.H);

    std::cout << "Briq is " << (std::is_pod<gimiqs::Briq>::value ? "POD!" : "not POD!") << std::endl;
}
