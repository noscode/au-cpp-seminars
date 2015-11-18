#include <iostream>
#include <string>
#include "array.h"

int main() {
    my::array<int, 10> ar1;
    for(size_t i = 0; i < ar1.size(); ++i) {
        ar1[i] = i;
        std::cout << ar1[i] << std::endl;
    }

    my::array<std::string, 3> ar2("foobar");
    for(size_t i = 0; i < ar2.size(); ++i) {
        std::cout << ar2[i] << std::endl;
    }
    return 0;
}
