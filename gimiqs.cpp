#include "gimiqs.hpp"

#include <vector>
#include <string>
#include <istream>
#include <iostream>
// #include <iomanip>
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

std::vector<std::string> split(const std::string &str, char sep) {
    std::vector<std::string> v;

    auto first = str.begin();

    while (first != str.end()) {
        auto last = first; // iterator of last char before separator

        while (last != str.end() && *last != sep) ++last;

        // push slice of original std::string from first to last
        if (first != last) v.push_back(std::string(first, last));

        if (last != str.end()) ++last;

        first = last;
    }

    return v;
}

void test_briq(gimiqs::Baseplate& bp) {
    std::cout << "hello, piq." << std::endl;

    gimiqs::Bucket& bc = bp.at(-1);

    gimiqs::Briq& b = bc.at(0);

    b.L = BYTE_SWAP_64(0x0123456789ABCDEF);
    b.H = BYTE_SWAP_64(0xFEDCBA9876543210);

    printf("piq1: %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX\n", b.LLLL, b.LLLH, b.LLHL, b.LLHH, b.LHLL, b.LHLH, b.LHHL, b.LHHH, b.HLLL, b.HLLH, b.HLHL, b.HLHH, b.HHLL, b.HHLH, b.HHHL, b.HHHH);
    printf("piq2: %04hX %04hX %04hX %04hX %04hX %04hX %04hX %04hX\n", b.LLL, b.LLH, b.LHL, b.LHH, b.HLL, b.HLH, b.HHL, b.HHH);
    printf("piq3: %08X %08X %08X %08X\n", b.LL, b.LH, b.HL, b.HH);
    printf("piq4: %016lX %016lX\n", b.L, b.H);

    std::cout << "Briq is " << (std::is_pod<gimiqs::Briq>::value ? "POD!" : "not POD!") << std::endl;
}

void parse_assembler(gimiqs::Baseplate& bp, std::vector<std::string> tokens) {
    if (tokens[0] == "quit") {
        exit(0);
    } else if (tokens[0] == "mkbc") {
        if (tokens.size() != 2) {
            std::cout << "sorry, skbc command requires 1 arg... plz try again. " << std::endl;
        } else {
            std::cout << "you wanna make bucket of #" << tokens[1] << ", i see." << std::endl;
        }
    } else {
        std::cout << "sorry, i can not get what you mean... ('" << tokens[0] << "'). plz try again. " << std::endl;
    }
}

int main() {
    gimiqs::Baseplate bp;

    test_briq(bp);

    while (true) {
        char buffered_string[256];
        std::cin.getline(buffered_string, sizeof(buffered_string));
        std::string s { buffered_string };
        parse_assembler(bp, split(s, ' '));
    }
}
