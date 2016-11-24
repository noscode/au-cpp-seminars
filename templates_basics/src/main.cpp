#include <iostream>
#include <array>
#include <string>
#include <utility>
#include <forward_list>
#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>

namespace mystd
{

template<class T>
void fill(T *begin, T *end, const T &value)
{
    for (T *it = begin; it != end; ++it)
    {
        *it = value;
    }
}

template<class T, class GEN>
void generate(T *begin, T *end, GEN gen)
{
    for (auto it = begin; it != end; ++it)
    {
        *it = gen();
    }
}

template<class T, class U>
U* copy(T *src_begin, T *src_end, U *dst_begin)
{
    auto dst_it = dst_begin;

    for (auto src_it = src_begin; src_it != src_end; ++src_it, ++dst_it)
    {
        *dst_it = *src_it;
    }
    return dst_it;
}

template<class T, class FUNC>
FUNC for_each(T *begin, T *end, FUNC func)
{
    for (auto it = begin; it != end; ++it)
        func(*it);

    return std::move(func);
}

} // namespace mystd

namespace util
{

template<class CONT>
void print_container(const CONT &cont)
{
    for (typename CONT::const_iterator it = cont.cbegin();
            it != cont.cend(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
}

} // namespace util

template<class T>
struct fifo
{
    fifo() = default;
    fifo(const fifo &src) = default;
    fifo& operator=(const fifo &src) = default;

    void push(const T& val)
    {
        if (lst_.empty())
            return lst_.push_front(val);

        auto prev_it = lst_.begin();
        auto it = lst_.begin();
        for (; it != lst_.end(); ++it)
            prev_it = it;

        lst_.insert_after(prev_it, val);
    }

    void pop()
    {
        if (lst_.empty())
            return;
        lst_.pop_front();
    }

    T& front()
    {
        return *lst_.begin();
    }

    const T& front() const
    {
        return *lst_.cbegin();
    }

    size_t size() const
    {
        return lst_.size();
    }

private:
    std::forward_list<T> lst_;
};

namespace mystd
{

template<class T, class COMP=std::less<T>>
struct priority_queue
{
    priority_queue() = default;
    priority_queue(const priority_queue &src) = default;
    priority_queue& operator=(const priority_queue &src) = default;

    void push(const T& val)
    {
        COMP comp;
        vec_.push_back(val);
        std::push_heap(vec_.begin(), vec_.end(), comp);
    }

    void pop()
    {
        if (vec_.empty())
            return;

        COMP comp;
        std::pop_heap(vec_.begin(), vec_.end(), comp);
        vec_.pop_back();
    }

    T& front()
    {
        return vec_[0];
    }

    const T& front() const
    {
        return vec_[0];
    }

    size_t size() const
    {
        return vec_.size();
    }

private:
    std::vector<T> vec_;
};

};

int main()
{
    // task 1
    std::array<std::string, 10> strings;
    mystd::fill(strings.data(), strings.data() + strings.size(),
            std::string("Hello!"));
    util::print_container(strings);

    std::array<int, 10> ints;
    mystd::generate(ints.data(), ints.data() + ints.size(),
            []() { static int i = 0; return i++; });
    util::print_container(ints);

    std::array<int, 10> ints2;
    assert(mystd::copy(ints.data(), ints.data() + ints.size(),
            ints2.data()) == (ints2.data() + ints2.size()));
    util::print_container(ints2);


    std::array<std::string, 10> strings2(strings);
    mystd::for_each(strings2.data(), strings2.data() + strings2.size(),
            [] (const std::string &str) { std::cout << str << " "; });
    std::cout << std::endl;

    // task 2
    fifo<size_t> fifo1;
    for (size_t i = 0; i < 10; ++i)
        fifo1.push(i);
    for (size_t i = 0; i < 10; ++i)
    {
        std::cout << fifo1.front() << " ";
        assert(fifo1.front() == i);
        fifo1.pop();
    }
    std::cout << std::endl;

    // task 3
    mystd::priority_queue<size_t> pqueue;
    mystd::priority_queue<size_t, std::greater<size_t>> pqueue1;
    for (size_t i = 0; i < 10; ++i)
    {
        pqueue.push(i);
        pqueue1.push(i);
    }
    for (int i = 9; i >= 0; --i)
    {
        std::cout << pqueue.front() << " ";
        assert(pqueue.front() == static_cast<size_t>(i));
        pqueue.pop();
    }
    std::cout << std::endl;
    for (size_t i = 0; i < 10; ++i)
    {
        std::cout << pqueue1.front() << " ";
        assert(pqueue1.front() == i);
        pqueue1.pop();
    }
    std::cout << std::endl;

    return 0;
}
