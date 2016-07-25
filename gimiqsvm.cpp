#include "gimiqsvm.hpp"

//
namespace gimiqs {
    Briq getQ(Baseplate bp, Briq b) {
        return bp.get_bucket(b.bucket_index_G).at(b.briq_index_G);
    }
}

// convert inputed hex string to bytes (128bit)
std::vector<unsigned char> hex_string2bytes(std::string hex_str) {
    std::vector<unsigned char> ret(16, 0);

    for (char i = 0; i < 16; ++i) {
        char c1 = hex_str[i];
        char c2 = hex_str[i + 1];
        if ('0' <= c1 && c1 <= '9') {

        }
        unsigned char result_char1 =
    }

    return ret;
}

int main() {
    std::cout << "hello, BriqsVM!" << std::endl;
    auto bp = gimiqs::Baseplate();

    bool is_push_mode = false;

    while(true) {
        // start moving gimVM

        auto entry_point_briq = bp.get_bucket(-1).top(); // that's difinition
        if (getQ(bp, entry_point_briq).annot == '~') {
            ; // do something
        } else {
            std::cout << "now pausing..." << std::endl;
        }

        std::string a;
        if (!is_push_mode) {
            std::cout << "@|| ";
        }
        std::cin >> a;

        if (a == "@@@") {
            std::cout << "bye. " << std::endl;
            break;
            // push new briq
        } else if (is_push_mode) {
            short bc_idx = 0;
            gimiqs::Bucket bc;
            try {
                bc_idx = std::stoi(a);
                if (!bp.exists_bucket(bc_idx)) {
                    std::cout << "out of range, plz try again." << std::endl;
                    is_push_mode = false;
                    continue;
                }
                bc = bp.get_bucket(bc_idx);
            } catch (std::exception e) {
                std::cout << "invalid command! plz try again." << std::endl;
                is_push_mode = false;
                continue;
            } catch (std::out_of_range e) {
                std::cout << "out of range! plz try again." << std::endl;
                is_push_mode = false;
                continue;
            }

            auto b = gimiqs::Briq('~', 0);

            // if only one N is pushed(it's default state), delete it.
            if (bc.at(0).annot == 'N') {
                bc.replace(0, b);
            } else {
                bc.push(b);
            }

            bp.push_bucket(bc_idx, bc);

            bp.print_bucket();

            is_push_mode = false;
        } else {
            if (a == "mkbc") {
                auto b = gimiqs::Bucket();
                b.push(bp.get_bucket(0).at(0));
                for (short i = 0; i < 32767; ++i) {
                    if (!bp.exists_bucket(i)) {
                        bp.push_bucket(i, b);
                        break;
                    }
                }
            } else if (a == "push") {
                is_push_mode = true;
                continue;
            } else {
                std::cout << "invalid command! plz try again." << a << std::endl;
                continue;
            }
            bp.print_bucket();
        }
    }
}
