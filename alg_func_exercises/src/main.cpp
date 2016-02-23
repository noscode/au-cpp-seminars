#include <iostream>
#include <functional>
#include <algorithm>
#include <iterator>
#include <string>
#include <cassert>

template<typename ITERATOR>
using ivt = typename std::iterator_traits<ITERATOR>::value_type;

template<typename CONTAINER>
using cvt = typename CONTAINER::value_type;

template<typename ITERATOR>
void print(ITERATOR begin, ITERATOR end)
{
    std::cout << "=========================" << std::endl;
    std::for_each(begin, end, [](const ivt<ITERATOR> &val) { std::cout << val << std::endl; });
}

template<typename ITERATOR, typename COMPARATOR=std::less<ivt<ITERATOR> > >
size_t elem_num_in_sorted(ITERATOR begin, ITERATOR end, const ivt<ITERATOR> &elem,
        COMPARATOR comp = COMPARATOR())
{
    auto lower_bound = std::lower_bound(begin, end, elem, comp);
    auto upper_bound = std::upper_bound(begin, end, elem, comp);
    return std::distance(lower_bound, upper_bound);
}

template<typename CONTAINER>
typename CONTAINER::iterator set_add(CONTAINER &cont, const cvt<CONTAINER> &elem)
{
    auto lower_bound = std::lower_bound(cont.begin(), cont.end(), elem);
    if ((lower_bound != cont.end()) && (*lower_bound == elem))
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
        --pop_heap_cnt;
    }
}

template<typename CONT_ITERATOR>
struct linearized_container_iterator:
    std::iterator<
        std::forward_iterator_tag,
        typename std::iterator_traits<typename std::iterator_traits<CONT_ITERATOR>::value_type::iterator>::value_type
    >
{
    linearized_container_iterator(CONT_ITERATOR begin, CONT_ITERATOR end)
        : current_cont_it_(begin)
        , cont_end_it_(end)
        , current_it_(begin->begin())
    {
        if (current_it_ == current_cont_it_->end())
            ++(*this);
    }

    linearized_container_iterator()
    {}

    linearized_container_iterator(const linearized_container_iterator &src) = default;

    linearized_container_iterator& operator++()
    {
        if (current_cont_it_ == cont_end_it_)
            return *this;
        ++current_it_;
        if (current_it_ == current_cont_it_->end()) {
            ++current_cont_it_;
            if (current_cont_it_ == cont_end_it_)
                return *this;
            current_it_ = current_cont_it_->begin();
            if (current_it_ == current_cont_it_->end()) {
                ++(*this);
            }
        }
        return *this;
    }

    linearized_container_iterator& operator++(int)
    {
        linearized_container_iterator tmp(*this);
        ++(*this);
        return tmp;
    }

    bool operator==(const linearized_container_iterator &other)
    {
        return (current_cont_it_ == cont_end_it_) &&
            (other.current_cont_it_ == other.cont_end_it_);
    }

    bool operator!=(const linearized_container_iterator &other)
    {
        return !((*this) == other);
    }

    typename CONT_ITERATOR::value_type::value_type &operator*()
    {
        return *current_it_;
    }

protected:
    CONT_ITERATOR current_cont_it_;
    CONT_ITERATOR cont_end_it_;
    typename CONT_ITERATOR::value_type::iterator current_it_;
};

template<class CONTAINER_OF_CONTAINERS>
linearized_container_iterator<typename CONTAINER_OF_CONTAINERS::iterator>
    make_linearized_container_iterator(CONTAINER_OF_CONTAINERS &src)
{
    return linearized_container_iterator<typename CONTAINER_OF_CONTAINERS::iterator>(src.begin(), src.end());
}

template<class CONTAINER_OF_CONTAINERS>
linearized_container_iterator<typename CONTAINER_OF_CONTAINERS::iterator>
    make_linearized_container_iterator()
{
    return linearized_container_iterator<typename CONTAINER_OF_CONTAINERS::iterator>();
}

#define ARRAY_SIZE(ar) (sizeof(ar) / sizeof((ar)[0]))

int main() {
    int sorted_ar[] = { 1, 2, 3, 4, 5, 6, 7, 7, 7, 8, 9 , 10, 11};
    std::cout <<
        elem_num_in_sorted(sorted_ar, sorted_ar + ARRAY_SIZE(sorted_ar), 7)
        << std::endl;

    std::vector<int> set;
    assert(*set_add(set, 10) == 10);
    assert(set_add(set, 10) == set.end());
    assert(set_add(set, 10) == set.end());
    assert(*set_add(set, 5) == 5);
    set_add(set, 12);
    set_add(set, 6);
    assert(set[0] == 5);
    assert(set[1] == 6);
    assert(set[2] == 10);
    assert(set[3] == 12);
    assert(set.size() == 4);
    print(set.begin(), set.end());

    erase_if(set, [](int val) { return val > 6; });
    print(set.begin(), set.end());

    std::vector<int> unsorted1 = {
        10, 9, 4, 345, 54, 2, 4, 12
            , 4, 2, 34, 34, 56, 4, 67
            , 3465, 3, 4, 5, 34, 5
    };
    std::vector<int> unsorted2(unsorted1);

    merge_sort(unsorted1.begin(), unsorted1.end());
    print(unsorted1.begin(), unsorted1.end());

    heap_sort(unsorted2.begin(), unsorted2.end());
    print(unsorted2.begin(), unsorted2.end());

    typedef std::vector<std::vector<std::string>> vec_vec_string_t;
    vec_vec_string_t vec_vec_string = {
        { "1", "2", "3" },
        { "4", "5", "6" },
        { "7", "8", "9" },
        { "10", "11", "12" }
    };
    auto vec_vec_linearized_it = make_linearized_container_iterator(vec_vec_string);
    print(vec_vec_linearized_it, make_linearized_container_iterator<vec_vec_string_t>());

    return 0;
}
