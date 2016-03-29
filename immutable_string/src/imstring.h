#pragma once

#include <cstddef>
#include <cstring>
#include <memory>
#include <ostream>

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

    const char* begin() const
    {
        return shared_imstring_buf_->cstr();
    }

    const char* end() const
    {
        return shared_imstring_buf_->cstr() + shared_imstring_buf_->length();
    }

    size_t size() const
    {
        return shared_imstring_buf_->length();
    }

    const char* c_str() const
    {
        return begin();
    }

    std::ostream& print(std::ostream& out) const
    {
        return shared_imstring_buf_->print(out);
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
        }

        bool is_lazy_;
        // no lazy concat
        char *cstr_;
        size_t cstr_len_; // len includes \0 char
        // for lazy concat
        std::shared_ptr<imstring_buf> left_;
        std::shared_ptr<imstring_buf> right_;
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
