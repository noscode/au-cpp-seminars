#include <iostream>
#include <functional>
#include <algorithm>
#include <iterator>

template<typename ITERATOR>
using vt = typename std::iterator_traits<ITERATOR>::value_type;

template<typename ITERATOR>
void print(ITERATOR begin, ITERATOR end)
{
    std::cout << "=========================" << std::endl;
    std::for_each(begin, end, [](const vt<ITERATOR> &val) { std::cout << val << std::endl; });
}

template<typename ITERATOR>
size_t elem_num_in_sorted(ITERATOR begin, ITERATOR end, const vt<ITERATOR> &elem)
{
    auto lower_bound = std::lower_bound(begin, end, elem);
    auto upper_bound = std::upper_bound(begin, end, elem);
    return std::distance(lower_bound, upper_bound);
}

template<typename CONTAINER>
typename CONTAINER::iterator set_add(CONTAINER &cont, const typename CONTAINER::value_type &elem)
{
    auto lower_bound = std::lower_bound(cont.begin(), cont.end(), elem);
    if (lower_bound != cont.end() && *lower_bound == elem)
        return cont.end();
    else
        return cont.insert(lower_bound, elem);
}

template<typename CONTAINER, typename UNARY_PREDICATE>
size_t erase_if(CONTAINER &cont, UNARY_PREDICATE pr)
{
    auto new_end = std::remove_if(cont.begin(), cont.end(), pr);
    size_t erased_cnt = std::distance(new_end, cont.end());
    cont.erase(new_end, cont.end());
    return erased_cnt;
}

template<typename ITERATOR>
void merge_sort(ITERATOR begin, ITERATOR end)
{
    if (std::distance(begin, end) == 1)
    {
        return;
    }
    ITERATOR mid = begin;
    std::advance(mid, std::distance(begin, end) / 2);
    merge_sort(begin, mid);
    merge_sort(mid, end);
    std::inplace_merge(begin, mid, end);
}

template<typename ITERATOR>
void heap_sort(ITERATOR begin, ITERATOR end)
{
    std::make_heap(begin, end);
    size_t pop_heap_cnt = std::distance(begin, end);
    while(pop_heap_cnt)
    {
        pop_heap(begin, end);
        --end;
    }
}

#define ARRAY_SIZE(ar) (sizeof(ar) / sizeof((ar)[0]))

int main() {
    int sorted_ar[] = { 1, 2, 3, 4, 5, 6, 7, 7, 7, 8, 9 , 10, 11};
    std::cout << elem_num_in_sorted(sorted_ar, sorted_ar + ARRAY_SIZE(sorted_ar), 7) << std::endl;

    std::vector<int> set;
    set_add(set, 10);
    set_add(set, 10);
    set_add(set, 10);
    set_add(set, 5);
    set_add(set, 12);
    set_add(set, 6);
    print(set.begin(), set.end());

    erase_if(set, [](int val) { return val > 6; });
    print(set.begin(), set.end());

    std::vector<int> unsorted1 = { 10, 9, 4, 345, 54, 2, 4, 12, 4, 2, 34, 34, 56, 4, 67, 3465, 3, 4, 5, 34, 5 };
    std::vector<int> unsorted2(unsorted1);

    merge_sort(unsorted1.begin(), unsorted1.end());
    print(unsorted1.begin(), unsorted1.end());

    merge_sort(unsorted2.begin(), unsorted2.end());
    print(unsorted2.begin(), unsorted2.end());
    return 0;
}
