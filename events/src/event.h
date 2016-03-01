#pragma once
#include <functional>
#include <memory>
#include <utility>
#include <cstddef>
#include <vector>

namespace utils
{
    namespace details
    {
        using handler_id_t = size_t;

        template<class R, class... ARGS>
        struct event_state
        {
            event_state()
                : handler_id_cntr_(0)
            {}

            using event_handler_func_t = std::function<R(ARGS...)>;
            handler_id_t handler_id_cntr_;
            // bool : true if event handler isn't deleted
            std::vector<std::pair<bool, event_handler_func_t>> handlers_;
        };
    }

    template<class>
    struct event;

    template<class R, class... ARGS>
    struct event_handler_deleter
    {
        void operator()();
        event_handler_deleter(event_handler_deleter&) = default;
        event_handler_deleter(event_handler_deleter&&) = default;
        event_handler_deleter& operator=(event_handler_deleter&) = default;
        event_handler_deleter& operator=(event_handler_deleter&&) = default;

    private:
        friend struct event<R(ARGS...)>;
        using event_state_t = details::event_state<R, ARGS...>;
        event_handler_deleter(event_state_t *event_state,
                details::handler_id_t handler_id)
           : state_(event_state)
           , handler_id_(handler_id)
        {};

        event_state_t *state_;
        details::handler_id_t handler_id_;
    };

    template<class R, class... ARGS>
    struct event<R(ARGS...)>
    {
        event();
        event(event&) = delete;
        event(event&&) = default;
        event& operator=(event&) = delete;
        event& operator=(event&&) = default;

        using event_handler_func_t = std::function<R(ARGS...)>;
        using event_handler_deleter_t = event_handler_deleter<R, ARGS...>;
        event_handler_deleter_t add(event_handler_func_t func);

        void fire(ARGS...);
    private:
        using state_t = details::event_state<R, ARGS...>;
        std::unique_ptr<state_t> state_ptr_;
    };

    template<class R, class... ARGS>
    void event_handler_deleter<R, ARGS...>::operator()()
    {
        auto &ev_handler = state_->handlers_[handler_id_];
        ev_handler.first = false;
        ev_handler.second = nullptr;
    }

    template<class R, class... ARGS>
    event<R(ARGS...)>::event()
        : state_ptr_(new state_t())
    {}

    template<class R, class... ARGS>
    auto event<R(ARGS...)>::add(event_handler_func_t func) -> event_handler_deleter_t
    {
        details::handler_id_t new_handler_id = state_ptr_->handler_id_cntr_++;
        // naive implementation
        assert(new_handler_id == state_ptr_->handlers_.size());
        state_ptr_->handlers_.emplace_back(
                std::make_pair(true, std::move(func)));

        event_handler_deleter_t deleter(state_ptr_.get(), new_handler_id);
        return std::move(deleter);
    }

    template<class R, class... ARGS>
    void event<R(ARGS...)>::fire(ARGS... args)
    {
        for(auto &ev_handler : state_ptr_->handlers_)
        {
            if (ev_handler.first)
            {
                ev_handler.second(args...);
            }
        }
    }
}
