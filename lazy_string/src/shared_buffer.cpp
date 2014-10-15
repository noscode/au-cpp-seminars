#include <string.h>
#include <utility>

#include "shared_buffer.h"

struct shared_buffer::shared {
    char *data_;
    size_t ref_cntr_;
    size_t size_;
};

shared_buffer::shared_buffer(size_t size)
    : shared_(nullptr)
{
    shared_ = new shared();
    memset(shared_, 0, sizeof(*shared_));

    shared_->data_ = new char[size];
    shared_->size_ = size;
    shared_->ref_cntr_ = 1;
}

shared_buffer::shared_buffer(const shared_buffer &src)
{
    shared_ = src.shared_;
    ++shared_->ref_cntr_;
}

shared_buffer::shared_buffer(shared_buffer &&src)
    : shared_(nullptr)
{
    std::swap(shared_, src.shared_);
}

shared_buffer& shared_buffer::operator=(shared_buffer src)
{
    std::swap(shared_, src.shared_);
    return *this;
}

shared_buffer::~shared_buffer()
{
    if (shared_ == nullptr)
        return;

    --shared_->ref_cntr_;
    if (shared_->ref_cntr_ != 0)
        return;
    delete[] shared_->data_;
    delete shared_;
}

char* shared_buffer::get_data()
{
    return shared_->data_;
}

const char* shared_buffer::get_data() const
{
    return shared_->data_;
}

size_t shared_buffer::get_size() const
{
    return shared_->size_;
}
