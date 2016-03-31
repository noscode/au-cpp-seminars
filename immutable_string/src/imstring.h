#pragma once

#include <cstddef>
#include <cstring>
#include <memory>
#include <ostream>
#include <stack>
#include <cassert>
#include <utility>

struct imstring
{
    imstring(const char *ssrc)
        : shared_imstring_buf_(new imstring_buf(ssrc))
    {}
    imstring(const imstring &src1, const imstring &src2)
        : shared_imstring_buf_(new imstring_buf(
                    src1.shared_imstring_buf_, src2.shared_imstring_buf_))
    {}
    imstring(const imstring &src) = default;
    imstring& operator=(const imstring &src) = delete;

    char operator[](size_t ix) const
    {
        return shared_imstring_buf_->cstr()[ix];
    }

    size_t size() const
    {
        return shared_imstring_buf_->length();
    }

    const char* c_str() const
    {
        return shared_imstring_buf_->cstr();
    }

    std::ostream& print(std::ostream& out) const
    {
        return shared_imstring_buf_->print(out);
    }

private:
    struct imstring_iterator;
public:
    imstring_iterator begin() const
    {
        return imstring_iterator(shared_imstring_buf_.get());
    }

    imstring_iterator end() const
    {
        return imstring_iterator();
    }

private:
    struct imstring_buf
    {
        imstring_buf(const char* sstr)
            : is_lazy_(false)
            , cstr_(nullptr)
            , cstr_len_(strlen(sstr) + 1)
        {
            cstr_ = new char[cstr_len_];
            memcpy(cstr_, sstr, cstr_len_);
        }

        imstring_buf(std::shared_ptr<imstring_buf> &concat_src1,
                std::shared_ptr<imstring_buf> &concat_src2)
            : is_lazy_(true)
            , cstr_(nullptr)
            , cstr_len_(0)
            , left_(concat_src1)
            , right_(concat_src2)
        {}

        ~imstring_buf()
        {
            if (!is_lazy_)
            {
                delete[] cstr_;
            }
        }

        const char *cstr()
        {
            unlazy();
            return cstr_;
        }

        size_t length()
        {
            if (!is_lazy_)
            {
                return cstr_len_ -1;
            }
            return left_->length() + right_->length();
        }

        std::ostream& print(std::ostream &out)
        {
            if (is_lazy_)
            {
                left_->print(out);
                right_->print(out);
            }
            else
            {
                out << cstr_;
            }
            return out;
        }

    private:
        void unlazy()
        {
            if (!is_lazy_)
            {
                return;
            }
            size_t new_cstr_len =
                left_->length() + right_->length() + 1;
            cstr_ = new char[new_cstr_len];
            cstr_len_ = new_cstr_len;
            is_lazy_ = false;
            memcpy(cstr_, left_->cstr(), left_->length());
            memcpy(cstr_ + left_->length(), right_->cstr(), right_->length() + 1);
            left_.reset();
            right_.reset();
        }

        bool is_lazy_;
        // no lazy concat
        char *cstr_;
        size_t cstr_len_; // len includes \0 char
        // for lazy concat
        std::shared_ptr<imstring_buf> left_;
        std::shared_ptr<imstring_buf> right_;
        friend struct imstring_iterator;
    };

    struct imstring_iterator: std::iterator<std::forward_iterator_tag, char>
    {
        imstring_iterator() = default;
        imstring_iterator(imstring_buf *root_imstring_buf)
        {
            dfs_stack_.emplace(root_imstring_buf);
            advance_to_next_valid_char();
        }

        imstring_iterator(const imstring_iterator &src) = default;
        imstring_iterator(imstring_iterator &&src) = default;
        imstring_iterator& operator=(const imstring_iterator &src) = default;
        imstring_iterator& operator=(imstring_iterator &&src) = default;

        const char& operator*() const
        {
            return *this->operator->();
        }

        const char* operator->() const
        {
            const dfs_step &cur_step = dfs_stack_.top();
            assert(!cur_step.imstring_buf_->is_lazy_);
            return &cur_step.imstring_buf_->cstr_[cur_step.it_char_ix_];
        }

        bool operator==(const imstring_iterator &other) const
        {
            // TODO we also can compare 2 not past the end iterators
            // using comparasion of each dfs_step in dfs stacks of iterators
            if (this->dfs_stack_.empty() && other.dfs_stack_.empty())
            {
                return true;
            }
            else if (this->dfs_stack_.size() == 1)
            {
                return (other.dfs_stack_.size() == 1)
                    && (this->dfs_stack_.top() == other.dfs_stack_.top());
            }
            else
            {
                return false;
            }
        }

        bool operator!=(const imstring_iterator &other) const
        {
            return !(*this == other);
        }

        imstring_iterator& operator++()
        {
            advance_to_next_valid_char();
            return *this;
        }

        imstring_iterator operator++(int)
        {
            imstring_iterator tmp(*this);
            advance_to_next_valid_char();
            return std::move(tmp);
        }
    private:
        struct dfs_step
        {
            dfs_step(imstring_buf *imstring_buf)
                : imstring_buf_(imstring_buf)
                , it_char_ix_((size_t)((ssize_t)-1)) // XXX dirty: (++it_char_ix_) == 0
                , is_visited_left_(false)
                , is_visited_right_(false)
            {}
            dfs_step(const dfs_step &src) = default;
            dfs_step(dfs_step &&src) = default;

            bool operator==(const dfs_step &other) const
            {
                return (imstring_buf_ == other.imstring_buf_)
                    && (it_char_ix_ == other.it_char_ix_)
                    && (is_visited_left_ == other.is_visited_left_)
                    && (is_visited_right_ == other.is_visited_right_);
            }

            imstring_buf *imstring_buf_;
            // if imstring_buf_ is not lazy concat
            size_t it_char_ix_; 
            // if imstring_buf_ is lazy concat
            bool is_visited_left_;
            bool is_visited_right_;
        };
        std::stack<dfs_step> dfs_stack_;

        void advance_to_next_valid_char()
        {
            if (dfs_stack_.empty())
            {
                return; // iteration has finished
            }

            dfs_step &cur_step = dfs_stack_.top();
            if (cur_step.imstring_buf_->is_lazy_)
            {
                advance_to_next_valid_char_lazy();
            }
            else
            {
                advance_to_next_valid_char_nonlazy();
            }
        }

        void advance_to_next_valid_char_nonlazy()
        {
            dfs_step &cur_step = dfs_stack_.top();
            ++cur_step.it_char_ix_;

            if (cur_step.it_char_ix_ == cur_step.imstring_buf_->length()) {
                dfs_stack_.pop();
                advance_to_next_valid_char();
            }
        }

        void advance_to_next_valid_char_lazy()
        {
            dfs_step &cur_step = dfs_stack_.top();
            if (!cur_step.is_visited_left_)
            {
                cur_step.is_visited_left_ = true;
                dfs_stack_.emplace(cur_step.imstring_buf_->left_.get());
                advance_to_next_valid_char();
            }
            else if (!cur_step.is_visited_right_)
            {
                cur_step.is_visited_right_ = true;
                dfs_stack_.emplace(cur_step.imstring_buf_->right_.get());
                advance_to_next_valid_char();
            }
            else
            {
                dfs_stack_.pop();
                advance_to_next_valid_char();
            }
        }
    };

    mutable std::shared_ptr<imstring_buf> shared_imstring_buf_;
};

std::ostream& operator<<(std::ostream &out, const imstring &str)
{
    str.print(out);
    return out;
}

imstring operator+(const imstring &src1, const imstring &src2)
{
    return imstring(src1, src2);
}
