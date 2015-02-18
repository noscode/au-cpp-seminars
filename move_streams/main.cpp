#include <iostream>
#include <string>
#include <utility>
#include "prefetch_stream.h"

template<typename STREAM>
static void print_stream(STREAM &in)
{
    typename STREAM::value_type read;
    in >> read;
    while(in.good()) {
        std::cout << read << std::endl;
        in >> read;
    }
}

int main() {
    int read_int;
    prefetch_stream<int, 10> in_int("input_int.txt");
    in_int >> read_int;
    in_int >> read_int;
    in_int >> read_int;
    decltype(in_int) in_int_copy(in_int);
    in_int >> read_int;
    in_int >> read_int;
    in_int >> read_int;
    decltype(in_int) in_int_moved(std::move(in_int));
    print_stream(in_int_copy);
    print_stream(in_int_moved);

    std::string read_str;
    prefetch_stream<std::string, 10> in_str("input_str.txt");
    in_str >> read_str;
    in_str >> read_str;
    in_str >> read_str;
    decltype(in_str) in_str_copy(in_str);
    in_str >> read_str;
    in_str >> read_str;
    in_str >> read_str;
    decltype(in_str) in_str_moved(std::move(in_str));
    print_stream(in_str_copy);
    print_stream(in_str_moved);

    return 0;
};
