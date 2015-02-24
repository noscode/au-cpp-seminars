#include "istream_iterator.h"
#include "ostream_iterator.h"
#include <functional>
#include <algorithm>
#include <iostream>

static void inf_sum()
{
    istream_iterator<int> is_it(std::cin);
    istream_iterator<int> is_it_end;
    ostream_iterator<int> cout_it(std::cout, "\n");
    int sum = 0;
    std::transform(is_it, is_it_end, cout_it,
        [&sum](int val) {
            return sum += val;
        }
    );
}

int main()
{
    inf_sum();
    return 0;
}
