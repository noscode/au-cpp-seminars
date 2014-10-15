#include <iostream>
#include <vector>
#include "lazy_string.h"

int main()
{
    lazy_string str_src("foobar");
    std::vector<lazy_string> strings(10000, str_src);
    str_src.set_at(2, '0');
    for (lazy_string &str : strings)
    {
        //std::cout << str << std::endl;
        print(std::cout, str); std::cout << std::endl;
    }
    //std::cout << str_src + str_src << std::endl;
    print(std::cout, concat(str_src, str_src)); std::cout << std::endl;
    //std::cout << (str_src = "assign my rvalue!") << std::endl;
    print(std::cout, str_src = "assign my rvalue!"); std::cout << std::endl;
    print(std::cout, str_src = str_src); std::cout << std::endl;
    return 0;
}
