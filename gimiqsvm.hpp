#include <iostream>
// #include <stack>
#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

namespace gimiqs {
    class BasePlate;

    struct Briq {
        char annot;
        char sub_annot;
        short bucket_index_P;
        short bucket_index_Q;
        unsigned int briq_index_P;
        unsigned int briq_index_Q;

        Briq(char an1, char an2) {
            set_defalut(an1, an2, 0, 0, 0, 0);
        }

        Briq(char an1, char an2, short bcidx1, short bcidx2, unsigned int bridx1, unsigned int bridx2) {
            set_defalut(an1, an2, bcidx1, bcidx2, bridx1, bridx2);
        }

        void set_defalut(char an1, char an2, short bcidx1, short bcidx2, unsigned int bridx1, unsigned int bridx2) {
            annot = an1;
            sub_annot = an2;
            bucket_index_P = bcidx1;
            bucket_index_Q = bcidx2;
            briq_index_P = bridx1;
            briq_index_Q = bridx2;
        }

        std::string to_s() {
            std::string ret;
            if (annot == '~') {
                ret = "N";
            } else if (annot == 'N') {
                ret = "";
            } else {
                std::string annot_str {annot};
                ret = "annot is [" + annot_str + "]";
                return ret;
            }

            if (briq_index_P > 0) {
                std::stringstream ss;
                ss << briq_index_P;
                ret += "P:" + ss.str() + " ";
            }

            if (briq_index_Q > 0) {
                std::stringstream ss;
                ss << briq_index_Q;
                ret += "Q:" + ss.str() + " ";
            }

            return ret;
        }

    };

    class Bucket {
        std::vector<Briq> briqs;
    public:
        void push(Briq b) {
            briqs.push_back(b);
            // std::cout << "size: " << briqs.size() << std::endl;
        }
        Briq top() {
            return briqs.front();
        }
        Briq at(unsigned int idx) {
            return briqs.at(idx);
        }

        long size() {
            return briqs.size();
        }

        void replace(unsigned int idx, Briq b) {
            if (idx < briqs.size()) {
                briqs[idx] = b;
            } else {
                std::cout << "out of range" << std::endl;
            }
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

        void push_bucket(short idx, Bucket bc) {
            buckets[idx] = bc;
        }

        void print_bucket() {
            for (auto it = buckets.begin(); it != buckets.end(); ++it) {
                std::cout << "{";
                std::cout << std::hex << std::setw(4) << std::setfill('0') << std::uppercase << it->first;
                std::cout << std::dec << std::setw(0) << std::setfill(' ') << std::nouppercase << ":"; // set to default1
                // std::cout << "size: " << it->second.size() << " ";
                // std::cout << it->second.size() << " ";
                std::cout << "[";
                for (auto i = 0; i < it->second.size(); ++i) {
                    std::cout << it->second.at(i).to_s();
                    if (i < it->second.size() - 1) {
                        std::cout << " ";
                    }
                }
                // std::cout << "]} " << it->second.size() << std::endl;
                std::cout << "]} " << std::endl;
            }
        }

        bool exists_bucket(short idx) {
            return (buckets.count(idx) == 1);
        }
    };
}
