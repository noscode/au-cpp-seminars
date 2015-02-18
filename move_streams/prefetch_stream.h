#pragma once

#include "ifstream.h"
#include <array>
#include <utility>

template<typename T, size_t PREFETCH_SIZE>
struct prefetch_stream;

template<typename T, size_t PREFETCH_SIZE>
prefetch_stream<T, PREFETCH_SIZE>& operator>>(prefetch_stream<T, PREFETCH_SIZE> &in, T& dst);

template<typename T, size_t PREFETCH_SIZE=10>
struct prefetch_stream
{
    typedef prefetch_stream type;
    typedef T value_type;
    static constexpr size_t prefetch_size = PREFETCH_SIZE;

    explicit prefetch_stream(const char *path);
    prefetch_stream(const prefetch_stream &src) = default;
    prefetch_stream(prefetch_stream &&src) = default;
    prefetch_stream& operator=(const prefetch_stream &src) = default;
    prefetch_stream& operator=(prefetch_stream &&src) = default;

    void swap(prefetch_stream &src);
    bool good() const;
protected:
    std::array<T, PREFETCH_SIZE> prefetched_;
    ifstream ifstream_;
    size_t read_pos_;
    size_t prefetched_cnt_;

    friend prefetch_stream& operator>> <T, PREFETCH_SIZE>(prefetch_stream &in, T& dst);
};

template<typename T, size_t PREFETCH_SIZE>
prefetch_stream<T, PREFETCH_SIZE>& operator>>(prefetch_stream<T, PREFETCH_SIZE> &in, T& dst);

template<typename T, size_t PREFETCH_SIZE>
void swap(prefetch_stream<T, PREFETCH_SIZE> &first, prefetch_stream<T, PREFETCH_SIZE> &second)
{
    first.swap(second);
}

template<typename T, size_t PREFETCH_SIZE>
prefetch_stream<T, PREFETCH_SIZE>::prefetch_stream(const char *path)
    : ifstream_(path)
    , read_pos_(0)
    , prefetched_cnt_(0)
{}

template<typename T, size_t PREFETCH_SIZE>
void prefetch_stream<T, PREFETCH_SIZE>::swap(prefetch_stream &other)
{
    std::swap(ifstream_, other.ifstream_);
    std::swap(prefetched_, other.prefetched_);
    std::swap(read_pos_, other.read_pos_);
    std::swap(prefetched_cnt_, other.prefetched_cnt_);
}

template<typename T, size_t PREFETCH_SIZE>
bool prefetch_stream<T, PREFETCH_SIZE>::good() const
{
    return ifstream_.good() || (read_pos_ < prefetched_cnt_);
}

template<typename T, size_t PREFETCH_SIZE>
prefetch_stream<T, PREFETCH_SIZE>& operator>>(prefetch_stream<T, PREFETCH_SIZE> &in, T& dst)
{
    if (in.read_pos_ == in.prefetched_cnt_)
    {
        in.read_pos_ = 0;
        in.prefetched_cnt_ = 0;

        while (in.prefetched_cnt_ < in.prefetched_.size() && !in.ifstream_.eof())
        {
            in.ifstream_ >> in.prefetched_[in.prefetched_cnt_];
            if (in.ifstream_.fail()) { break; }
            in.prefetched_cnt_++;
        }
    }

    if (in.read_pos_ < in.prefetched_cnt_)
    {
        dst = std::move(in.prefetched_[in.read_pos_++]);
    }
    return in;
}
