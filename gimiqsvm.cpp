#include "gimiqsvm.hpp"

void log(std::string s) {
    std::cout << s << std::endl;
}

//
namespace gimiqs {
    Briq getQ(Baseplate bp, Briq b) {
        return bp.get_bucket(b.bucket_index_Q).at(b.briq_index_Q);
    }
}

// convert single string like '1B' to 0x1B(=16*16+11=267)
unsigned char hex_string2byte(char c1, char c2) {
    unsigned char result = 0;
    unsigned char uc1 = (unsigned char)c1;
    unsigned char wr = 0;

    if ('0' <= uc1 && uc1 <= '9') {
        wr += uc1 - 48;
    } else if ('A' <= uc1 && uc1 <= 'F') {
        wr += uc1 - 55;
    } else if ('a' <= uc1 && uc1 <= 'f') {
        wr += uc1 - 87;
    } else {
        std::cout << "invalid string, [" << c1 << "]" << "plz try again." << std::endl;
    }
    result += (wr << 4);

    // std::cout << "[" << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << (int)wr << "|" << (int)uc1 << "]" << std::endl;
    // std::cout << "[" << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << (int)result << "|" << (int)uc1 << "]" << std::endl;

    if ('0' <= c2 && c2 <= '9') {
        result += c2 - 48;
    } else if ('A' <= c2 && c2 <= 'F') {
        result += c2 - 55;
    } else if ('a' <= c2 && c2 <= 'f') {
        result += c2 - 87;
    } else {
        std::cout << "invalid string, [" << c2 << "]" << "plz try again." << std::endl;
    }

    // std::cout << "[" << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << (int)result << "|" << (int)uc1 << "]" << std::endl;

    return result;
}

// convert inputed hex string to bytes (64bit)
// treat as BIG endian
std::vector<unsigned char> hex_string2bytes(std::string hex_str) {
    std::vector<unsigned char> ret(8, 0);

    // 8byte (string size is 16) only
    if (hex_str.size() != 16) {
        log("invalid string, [" + hex_str + "]" + "plz try again.");
        return ret;
    }

    for (char i = 0; i < 8; ++i) {
        char c1 = hex_str[i];
        char c2 = hex_str[i + 1];
        ret.push_back(hex_string2byte(c1, c2));
    }

    return ret;
}

int main() {

    // std::cout << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << hex_string2byte('1', 'B') << std::endl;
    // std::cout << "hello, BriqsVM!" << std::endl;
    auto bp = gimiqs::Baseplate();

    bool is_push_mode = false;
    bool is_push_hex_mode = false;

    gimiqs::Briq target_briq(0, 0);
    gimiqs::Bucket bc;
    short bc_idx = 0;

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
            // now push [short] <- here!!
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

            target_briq = gimiqs::Briq('~', 0);

            // if only one N is pushed(it's default state), delete it.
            if (bc.at(0).annot == 'N') {
                bc.replace(0, target_briq);
            } else {
                bc.push(target_briq);
            }

            bp.push_bucket(bc_idx, bc);

            is_push_mode = false;
            is_push_hex_mode = true;
            continue;
        } else if (is_push_hex_mode) {
            // now push [short] [hex_string] <- here!!
            // std::vector<unsigned char> ret(8) { hex_string2bytes(a) };
            std::vector<unsigned char> ret { hex_string2bytes(a) };

            unsigned int value_p = 0;
            unsigned int value_q = 0;
            for (int i = 0; i < 4; ++i) {
                value_p += ret[i] << (i - 3);
            }
            for (int i = 4; i < 8; ++i) {
                value_q += ret[i] << (i - 3);
            }

            // this is ad hoc (should be selectable formar 16byte or latter)
            // e.g. pushf 1 1A2B3C4D pushl 2 1A2B3C4D
            std::cout << value_p << " " << value_q << std::endl;
            target_briq.briq_index_P = value_p;
            target_briq.briq_index_Q = value_q;

            is_push_hex_mode = false;

            bc.replace(0, target_briq);
            bp.push_bucket(bc_idx, bc);

            bp.print_bucket();
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
            } else if (a == "show") {
                ; // do nothing... only for print_bucket
            } else {
                std::cout << "invalid command! plz try again." << a << std::endl;
                continue;
            }
            bp.print_bucket();
        }
    }
}
