#include "gimiqs.hpp"

#include <vector>
#include <istream>
#include <type_traits>

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

namespace gimiqs {
    std::string show_briq(Briq& b) {
        std::string ret;

        if (b.LLLL == 0x00) {
            switch (b.LLLH) {
                case 0:
                ret = "N";
                break;
                case -1: {
                    std::stringstream ss;
                    ss << "MAX<" << BYTE_SWAP_32(b.LH) << ">";
                    ret = ss.str();
                }
                return ret;
                default:
                break;
            }

        } else if (b.LLLL == -1) {
            ret = "SIG";
            switch (b.LLLH) {
                case -1:
                ret += "<INTERPRETER>";
                break;
                default:
                break;
            }
        } else {
            std::string annot_str {b.LLLL};
            ret = "annot is [" + annot_str + "] ";
            return ret;
        }

        if (b.LH > 0 || b.HH > 0) ret += "<";

        if (b.LH > 0) {
            std::stringstream ss;
            ss << BYTE_SWAP_32(b.LH);
            ret += "P:" + ss.str();
        }

        if (b.LH > 0 && b.HH > 0) ret += " ";

        if (b.HH > 0) {
            std::stringstream ss;
            ss << BYTE_SWAP_32(b.HH);
            ret += "Q:" + ss.str();
        }

        if (b.LH > 0 || b.HH > 0) ret += ">";

        return ret;
    }
} // namespace gimiqs

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

bool check_args_size(std::vector<std::string> tokens, int required_arg_count) {
    if (tokens.size() != required_arg_count + 1) {
        std::cout << "sorry, rmbr command requires " << required_arg_count << " arg... plz try again. " << std::endl;
        return false;
    }
    return true;
}

bool is_digit_string(std::string s) {
    if (s.empty()) return false;
    for (auto it = s.begin(); it != s.end(); ++it) {
        if (!isdigit(*it)) return false;
    }
    return true;
}

bool is_hex_digit_string(std::string s) {
    if (s.empty()) return false;
    for (auto it = s.begin(); it != s.end(); ++it) {
        if (!isxdigit(*it)) return false;
    }
    return true;
}

void parse_assembler(gimiqs::Baseplate& bp, std::vector<std::string> tokens) {
    if (tokens[0] == "quit") {
        exit(0);
    } else if (tokens[0] == "mkbc") {
        if (tokens.size() != 2) {
            std::cout << "sorry, mkbc command requires 1 arg... plz try again. " << std::endl;
        } else {
            std::cout << "you wanna make bucket of #" << tokens[1] << ", i see." << std::endl;
            bp.make_bucket(std::stoi(tokens[1]));
        }
    } else if (tokens[0] == "rmbc") {
        if (tokens.size() != 2) {
            std::cout << "sorry, rmbc command requires 1 arg... plz try again. " << std::endl;
        } else {
            bp.slay_bucket(std::stoi(tokens[1]));
        }
    } else if (tokens[0] == "mkbr") {
        if (tokens.size() != 2) {
            std::cout << "sorry, mkbr command requires 1 arg... plz try again. " << std::endl;
        } else {
            bp.at(std::stoi(tokens[1])).push();
        }
    } else if (tokens[0] == "rmbr") {
        if (tokens.size() != 2) {
            std::cout << "sorry, rmbr command requires 1 arg... plz try again. " << std::endl;
        } else {
            bp.at(std::stoi(tokens[1])).pop();
        }
    } else if (tokens[0] == "adlc" || tokens[0] == "adhc") {
        if (check_args_size(tokens, 2)) {
            // first arg is briq index
            // <bucket_number>:<briq_index> or <bucket_number>
            auto briq_index_tokens = split(tokens[1], ':');
            gimiqs::Briq& target_briq = bp.at(-1).at(0);
            short bucket_number;
            switch (briq_index_tokens.size()) {
                case 1: {
                    // should be formula <bucket_number>
                    std::string arg1 = briq_index_tokens[0];
                    if (is_digit_string(arg1)) {
                        // std::cout << "arg1[" << arg1 << "]" << std::endl;
                        bucket_number = std::stol(arg1);
                    }
                    target_briq = bp.at(bucket_number).top();
                    break;
                }

                case 2: {
                    int briq_index;

                    // should be formula <bucket_number>:<briq_index>
                    std::string arg1 = briq_index_tokens[0];
                    if (is_digit_string(arg1)) {
                        bucket_number = std::stol(arg1);
                    }
                    std::string arg2 = briq_index_tokens[1];
                    if (is_digit_string(arg2)) {
                        briq_index = std::stol(arg2);
                    }
                    target_briq = bp.at(bucket_number).at(briq_index);
                    break;
                }

                default:
                break;
            }

            auto token2 = tokens[2];
            if (is_digit_string(token2)) {
                // std::cout << token2 << std::endl;
                if (tokens[0] == "adlc") {
                    target_briq.L = BYTE_SWAP_64(std::stol(token2));
                } else if (tokens[0] == "adhc") {
                    target_briq.H = BYTE_SWAP_64(std::stol(token2));
                }
            }

            // std::cout << "check " << std::stol("0xFF", nullptr, 16) << std::endl;
        }
    } else if (tokens[0] == "adll") {
    } else if (tokens[0] == "adlh") {
    } else if (tokens[0] == "adhl") {
    } else if (tokens[0] == "adhh") {
    } else if (tokens[0] == "orlc") {
    } else if (tokens[0] == "orhc") {
    } else if (tokens[0] == "orll") {
    } else if (tokens[0] == "orlh") {
    } else if (tokens[0] == "orhl") {
    } else if (tokens[0] == "orhh") {
    } else if (tokens[0] == "notc") {
    } else if (tokens[0] == "noth") {
    } else if (tokens[0] == "show") {
        ; // do nothing (= only print current state of baseplate)
    } else {
        std::cout << "sorry, i can not get what you mean... ('" << tokens[0] << "'). plz try again. " << std::endl;
    }
}

int main() {
    gimiqs::Baseplate bp;

    // test_briq(bp);

    // bp.at(-1)

    while (true) {
        char buffered_string[256];
        std::cout << "@|| ";
        std::cin.getline(buffered_string, sizeof(buffered_string));
        std::string s { buffered_string };
        parse_assembler(bp, split(s, ' '));
        std::cout << bp.to_s() << std::endl;
    }
}
