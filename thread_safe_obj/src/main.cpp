#include <iostream>
#include <vector>
#include <array>
#include <utility>
#include "thread_safe_obj.h"

template<typename T=int, size_t N=1>
struct n_stack
{
    typedef std::array<T, N> element_t;
    void push(element_t &&elem)
    {
        auto data = data_unsafe_.monitor();
        data->push_back(std::forward<element_t>(elem));
    }

    void pop()
    {
        auto data = data_unsafe_.monitor();
        data->pop_back();
    }

    element_t& top()
    {
        auto data = data_unsafe_.monitor();
        return (*data)[data->size() - 1];
    }

    const element_t& top() const
    {
        auto data = data_unsafe_.monitor();
        return (*data)[data->size() - 1];
    }

    bool empty() const
    {
        return data_unsafe_.monitor()->empty();
    }

private:
    typedef std::vector<element_t> data_t;
    thread_ops::thread_safe_obj<data_t> data_unsafe_;
};

static thread_ops::data_monitor<int> locked_add(
        thread_ops::data_monitor<int> &&dst,
        thread_ops::data_monitor<int> &&src)
{
    dst = *dst + *src;
    return std::move(dst);
}

static thread_ops::data_monitor<int> locked_mul(
        thread_ops::data_monitor<int> &&dst,
        thread_ops::data_monitor<int> &&src)
{
    dst = *dst * *src;
    return std::move(dst);
}

int main()
{
    n_stack<int, 5> stack;
    stack.push({1, 2, 3, 4, 5});
    stack.push({2, 3, 4, 5, 1});
    stack.push({3, 4, 5, 1, 2});
    stack.push({4, 5, 1, 2, 3});
    stack.push({5, 1, 2, 3, 4});

    while(!stack.empty())
    {
        std::cout << stack.top()[0] << std::endl;
        stack.pop();
    }

    thread_ops::thread_safe_obj<int> safe_int1(100);
    thread_ops::thread_safe_obj<int> safe_int2(200);
    std::cout
        << *locked_mul(
                locked_add(
                     safe_int1.monitor(),
                     safe_int2.monitor()
                ),
                thread_ops::thread_safe_obj<int>(1).monitor()
            )
        << std::endl;

    return 0;
}
