#include <iostream>
#include <stack>
#include <map>
#include <string>

namespace briqsvm {
    struct Briq {
        char annot;
        char sub_annot;
        short bucket_index_L;
        short bucket_index_G;
        int briq_index_L;
        int briq_index_G;

        Briq(char an1, char an2) {
            set_defalut(an1, an2, 0, 0, 0, 0);
        }

        Briq(char an1, char an2, short bcidx1, short bcidx2, int bridx1, int bridx2) {
            set_defalut(an1, an2, bcidx1, bcidx2, bridx1, bridx2);
        }

        void set_defalut(char an1, char an2, short bcidx1, short bcidx2, int bridx1, int bridx2) {
            annot = an1;
            sub_annot = an2;
            bucket_index_L = bcidx1;
            bucket_index_G = bcidx2;
            briq_index_L = bridx1;
            briq_index_G = bridx2;
        }

        std::string to_s() {
            if (annot == '~') {
                return "(N)";
            } else if (annot == 'N') {
                return "";
            }
            std::string annot_str {annot};
            return "annot is [" + annot_str + "]";
        }
    };

    class Bucket {
        std::stack<Briq> briqs;
    public:
        void push(Briq b) {
            briqs.push(b);
        }
        Briq top() {
            return briqs.top();
        }
    };

    class Baseplate {
        std::map<short, Bucket> buckets;
    public:
        Baseplate() {
            buckets[0] = Bucket();
            auto N = Briq('N', 0);
            buckets[0].push(N);

            buckets[-1] = Bucket();
            auto IP = Briq('~', 0);
            buckets[-1].push(IP);
        }

        Bucket get_bucket(short bucket_index) {
            return buckets[bucket_index];
        }
    };
}

int main() {
    std::cout << "hello, BriqsVM!" << std::endl;
    auto bp = briqsvm::Baseplate();

    while(true) {
        std::string a;
        std::cout << "@|| ";
        std::cin >> a;

        if (a == "@@@") {
            std::cout << "bye. " << std::endl;
            break;
        } else {
            std::cout << "you typed: " << a << std::endl;
            std::cout << "[FFFF]" << bp.get_bucket(-1).top().to_s() << std::endl;
            std::cout << "[0000]" << bp.get_bucket(0).top().to_s() << std::endl;
        }
    }
}
