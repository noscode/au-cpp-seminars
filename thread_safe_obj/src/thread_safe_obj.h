#pragma once

#include <mutex>

namespace thread_ops
{

    namespace details
    {
        struct noncopyable
        {
            noncopyable() = default;
            noncopyable(const noncopyable &src) = delete;
            noncopyable& operator=(const noncopyable& src) = delete;
        };
    } // namespace details

    template<class T>
    struct data_monitor: details::noncopyable
    {
        typedef std::mutex mutex_t;

        data_monitor(T& object, mutex_t& mut)
            : lock_(mut)
            , object_(object)
        {}

        data_monitor(data_monitor&& other)
            : lock_  (std::move(other.lock_))
            , object_(other.object_)
        {}

        data_monitor& operator=(data_monitor other)
        {
            swap(other);
            return *this;
        }

        T& get() const { return object_; }
        operator T&() const { return get(); }
        T& operator*() const { return get(); }
        T* operator->() const { return &object_;}

        // doesn't work for const monitor
        data_monitor& operator=(T const& obj)
        {
            object_ = obj;
            return *this;
        }

        data_monitor& operator=(T&& obj)
        {
            object_ = std::forward<T>(obj);
            return *this;
        }

        void swap(data_monitor& other)
        {
            using std::swap;
            swap(lock_, other.lock_);
            swap(object_, other.object_);
        }

    private:
        std::unique_lock<mutex_t> lock_;
        T& object_;
    };

    template <class T>
    struct thread_safe_obj
    {
        typedef data_monitor<T const> const_data_monitor_t;
        typedef data_monitor<T> data_monitor_t;
        typedef typename data_monitor_t::mutex_t mutex_t;

        thread_safe_obj() {}

        explicit thread_safe_obj(T const& object) : object_(object) {}
        explicit thread_safe_obj(T&& object) : object_(std::forward<T>(object)) {}

        const_data_monitor_t monitor() const { return const_data_monitor_t(object_, mut_); }
        data_monitor_t monitor() { return data_monitor_t(object_, mut_); }

    private:
        mutable mutex_t mut_;
        T object_;
    };

    // returns previous value 
    template<class T>
    T exchange(thread_safe_obj<T>& obj, T value)
    {
        using namespace std;
        swap(obj.monitor(), value);

        return value;
    }

} // namespace thread_ops
