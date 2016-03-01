#include <iostream>
#include <utility>
#include <string>
#include <functional>
#include <cassert>

#include "event.h"

static void test_event_add_fire()
{
    size_t eh_calls_cnt = 0;
    utils::event<int(const std::string&, const std::string&)> event;

    auto event_handler_printer = [&eh_calls_cnt](const std::string &str1, const std::string &str2) {
        std::cout << "Event handler " << str1 << " | " << str2 << std::endl;
        ++eh_calls_cnt;
        return eh_calls_cnt;
    };

    auto eh_del1 = event.add(event_handler_printer);
    auto eh_del2 = event.add(event_handler_printer);
    auto eh_del3 = event.add(event_handler_printer);

    assert(eh_calls_cnt == 0);
    event.fire("fire the", "1st time");
    assert(eh_calls_cnt == 3);
    eh_del2();
    event.fire("fire the", "2nd time");
    assert(eh_calls_cnt == 5);
    eh_del1();
    assert(eh_calls_cnt == 5);
    auto eh_del4 = event.add(event_handler_printer);
    auto eh_del5 = event.add(event_handler_printer);
    assert(eh_calls_cnt == 5);
    event.fire("fire the", "3rd time");
    assert(eh_calls_cnt == 8);
    (void)eh_del3;
    (void)eh_del4;
    (void)eh_del5;
}

static void test_event_handler_binding()
{
    using namespace std::placeholders;  // for _1, _2, _3...
    utils::event<void(const std::string&)> event;

    auto event_handler_printer = [](const std::string &str1, const std::string &str2) {
        std::cout << "Event handler " << str1 << " | " << str2 << std::endl;
    };

    event.add(std::bind(event_handler_printer, "Binded string eh 1!", _1));
    event.add(std::bind(event_handler_printer, "Binded string eh 2!", _1));
    event.fire("free argument to binded event handlers!");
}

static void test_event_copying()
{
    struct event_emmiter
    {
        utils::event<int(std::string&, std::string&)> event_;
    };
    event_emmiter em1;
    //event_emmiter em2 = em1; // should't compile
    event_emmiter em3(std::move(em1)); // should compile ok
    std::string str1("1");
    std::string str2("2");
    em3.event_.fire(str1, str2);
}

int main()
{
    test_event_add_fire();
    test_event_copying();
    test_event_handler_binding();
    return 0;
}
