#include <iostream>
#include <vector>

std::vector<std::string> split(std::string s, char delim) {
    std::vector<std::string> ss;
    if (s.empty()) return ss;
    for (auto p = 0, delim_idx = 0; p != -1;) {
        delim_idx = s.find(delim, p); // std::cout << delim_idx << std::endl;
        if (delim_idx != p) ss.push_back(s.substr(p, delim_idx - p)); // std::cout << p << "&" << delim_idx << std::endl;
        p = s.find_first_not_of(delim, delim_idx); // std::cout << p << "&" << delim_idx << std::endl;
    }
    return ss;
}

void display(std::vector<std::string> vec) {
    auto disp = [](std::string token){ std::cout << "[" << token << "]" << std::endl; };
    std::for_each(vec.begin(), vec.end(), disp);
}

int main() {
    std::string s1 { "abc defg ghij" };
    std::string s2;
    std::string s3 { " a  b " };
    std::string s4 { "wowow" };

    display(split(s1, ' '));
    display(split(s2, ' '));
    display(split(s3, ' '));
    display(split(s4, ' '));

    return 0;
}
