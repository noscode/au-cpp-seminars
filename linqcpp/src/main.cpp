#include <iostream>
#include <vector>
#include <string>
#include <functional>

#include "linq.h"

template<class T>
std::string to_string(const T& val)
{
    return std::to_string(val);
}

int main()
{
    using std::vector;
    using std::cout;
    using std::endl;

    enumerable<int> en_x =
         from({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10})
        .where([](int a) { return a > 5; })
        .print(cout);
    enumerable<std::string> en = en_x
        .select([](int a) { return a * a; })
        .print(cout)
        .select(to_string<int>)
        .print(cout);

    std::cout << en.count() << std::endl;
    std::cout << en.count([](const std::string &str) { return str == "100"; }) << std::endl;
    std::cout << en.any() << std::endl;
    std::cout << en.any([](const std::string &str) { return str == "3"; }) << std::endl;

    auto vec = en.toVector();
    for (auto elem : vec)
        std::cout << elem << " ";
    std::cout << std::endl;

    return 0;
}
