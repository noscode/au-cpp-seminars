#include <string.h>
#include <utility>
#include <cstdlib>

#include "shared_buffer.h"

struct shared_buffer::shared {
    size_t ref_cntr_;
    size_t size_;
    char data_[1];
};

shared_buffer::shared_buffer(size_t data_size)
    : shared_(nullptr)
{
    const size_t prealloced_data_size = sizeof(shared_->data_);
    const size_t shared_size = sizeof(*shared_) - prealloced_data_size + data_size;

    shared_ = (shared*)malloc(shared_size);
    memset(shared_, 0, shared_size);

    shared_->size_ = data_size;
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

    free(shared_);
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
