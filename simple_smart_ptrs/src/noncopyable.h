#pragma once
struct noncopyable
{
    noncopyable();
    noncopyable(const noncopyable &src) = delete;
    noncopyable& operator=(const noncopyable &src) = delete;
};

inline noncopyable::noncopyable() {}
