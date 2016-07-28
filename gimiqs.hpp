#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <sstream>

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


namespace gimiqs {

    /*
    |                   L                   |                   H                   |
    |        LL         |        LH         |        HL         |        HH         |
    |   LLL   |   LLH   |   LHL   |   LHH   |   HLL   |   HLH   |   HHL   |   HHH   |
    |LLLL|LLLH|LLHL|LLHH|LHLL|LHLH|LHHL|LHHH|HLLL|HLLH|HLHL|HLHH|HHLL|HHLH|HHHL|HHHH|
    */

    struct Briq {
        union {
            long L;
            struct { union { int LL;
                             struct { union { short LLL;
                                              struct { char LLLL, LLLH; }; };
                                      union { short LLH;
                                              struct { char LLHL, LLHH; }; }; }; };
                     union { int LH;
                             struct { union { short LHL;
                                              struct { char LHLL, LHLH; }; };
                                      union { short LHH;
                                              struct { char LHHL, LHHH; }; }; }; }; }; };
        union {
            long H;
            struct { union { int HL;
                             struct { union { short HLL;
                                              struct { char HLLL, HLLH; }; };
                                      union { short HLH;
                                              struct { char HLHL, HLHH; }; }; }; };
                     union { int HH;
                             struct { union { short HHL;
                                              struct { char HHLL, HHLH; }; };
                                      union { short HHH;
                                              struct { char HHHL, HHHH; }; }; }; }; }; };
    }; // struct Briq

    std::string show_briq(Briq& b);

    class Bucket {
        std::vector<Briq> briqs;
    public:
        Bucket() {
            this->push();
            briqs[0].LLL = 0xFF00; // little endian ... [0x00 0xFF]
            briqs[0].LH = 0;
        }

        void push() {
            Briq b {0};
            briqs.push_back(b);

            // increment max_index
            int max_index = this->max_index() + 1;
            briqs[0].LH = BYTE_SWAP_32(max_index);
        }

        void pop() {
            // can not remove first briq of bucket
            // (it has index of last briq)
            if (briqs.size() == 1) {
                std::cout << "sorry, the bucket is already empty. plz try again." << std::endl;
                return;
            }
            briqs.pop_back();

            // decrement max_index
            int max_index = this->max_index() - 1;
            briqs[0].LH = BYTE_SWAP_32(max_index);
        }

        Briq& at(int index) {
            return briqs.at(index);
        }

        std::string to_s() {
            std::stringstream ss;
            for (auto it = briqs.begin(); it != briqs.end(); ++it) {
                ss << show_briq(*it) << " ";
            }
            std::string s = ss.str();
            s.pop_back();
            return s;
        }

        int max_index() {
            return BYTE_SWAP_32(briqs[0].LH);
        }

        Briq& top() {
            return briqs.back();
        }
    }; // class Bucket

    class Baseplate {
        std::map<short, Bucket> buckets;
    public:
        Baseplate() {
            this->make_bucket(0xFFFF);
            buckets[0xFFFF].at(0).LLL = 0xFFFF;
        }

        void make_bucket(short index) {
            Bucket bc;
            buckets[index] = bc;
        }

        void slay_bucket(short index) {
            buckets.erase(index);
        }

        Bucket& at(short index) {
            return buckets[index];
        }

        std::string to_s() {
            std::stringstream ss;
            for (auto it = buckets.begin(); it != buckets.end(); ++it) {
                // bucket number
                ss << "[" << std::hex << std::setw(4) << std::setfill('0') << std::uppercase << it->first << "]";
                // content of bucket
                ss << "{" << it->second.to_s() << "}" << "\n";
            }
            return ss.str();
        }
    }; // class Baseplate

} // namespace gimiqs
