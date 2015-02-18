#include <iostream>
#include <string>
#include <utility>
#include <cassert>
#include <vector>
#include "prefetch_stream.h"

template<typename STREAM, typename READT>
void test_read(STREAM &in, const READT &expected)
{
    READT read;
    in >> read;
    std::cout << read << std::endl;
    assert(read == expected);
}

template<typename ISTREAM, typename EXPECTED_ARRAY>
void test_stream(ISTREAM &in, EXPECTED_ARRAY expected)
{
    assert(expected.size() >= 10);
    size_t i = 0;
    for (; i != 3; ++i)
        test_read(in, expected[i]);
    ISTREAM in_copy(in);
    for (; i != 6; ++i)
    {
        test_read(in, expected[i]);
        test_read(in_copy, expected[i]);
    }
    ISTREAM in_moved(std::move(in));
    for (; i != 10; ++i) {
        test_read(in_moved, expected[i]);
        test_read(in_copy, expected[i]);
    }
    in = std::move(in_moved);
    for (; i != expected.size(); ++i)
    {
        test_read(in, expected[i]);
        test_read(in_copy, expected[i]);
    }
}

int main()
{
    std::vector<int> expected_int = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10
        , 11, 12, 13, 14, 15, 16
    };
    ifstream in_int("input_int.txt");
    prefetch_stream<int> in_pref_int("input_int.txt");

    test_stream(in_int, expected_int);
    test_stream(in_pref_int, expected_int);

    std::vector<std::string> expected_str = {
        "qwerty1", "qwerty2", "qwerty3", "qwerty4", "qwerty5"
        , "qwerty6", "qwerty7", "qwerty8", "qwerty9", "qwerty10"
        , "qwerty11", "qwerty12", "qwerty13", "qwerty14", "qwerty15"
    };
    ifstream in_str("input_str.txt");
    prefetch_stream<std::string> in_pref_str("input_str.txt");

    test_stream(in_str, expected_str);
    test_stream(in_pref_str, expected_str);
    return 0;
}
