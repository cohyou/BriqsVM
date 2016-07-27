#include <vector>
#include <map>

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

    class Bucket {
        std::vector<Briq> briqs;
    public:
        Bucket() {
            this->push();
        }

        void push() {
            Briq b {0};
            briqs.push_back(b);
        }

        void pop() {
            briqs.pop_back();
        }

        Briq& at(int index) {
            return briqs.at(index);
        }
    }; // class Bucket

    class Baseplate {
        std::map<short, Bucket> buckets;
    public:
        Baseplate() {
            this->make_bucket(0xFFFF);
        }

        void make_bucket(short index) {
            Bucket bc;
            buckets[index] = bc;
        }

        Bucket& at(short index) {
            return buckets[index];
        }
    }; // class Baseplate

} // namespace gimiqs
