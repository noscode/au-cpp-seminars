#include "ifstream.h"

ifstream::ifstream()
    : file_(nullptr)
    , path_(nullptr)
{}

ifstream::ifstream(const char *path)
    : file_(fopen(path, "r"))
    , path_(path)
{}

ifstream::ifstream(const ifstream &src)
    : file_(fopen(src.path_, "r"))
    , path_(src.path_)
{
    fpos_t pos;
    fgetpos(src.file_, &pos);
    fsetpos(file_, &pos);
}

ifstream::ifstream(ifstream &&src)
    : file_(src.file_)
    , path_(src.path_)
{
    src.file_ = nullptr;
    src.path_ = nullptr;
}

ifstream& ifstream::operator=(ifstream src)
{
    swap(src);
    return *this;
}

ifstream::~ifstream()
{
    if (file_)
    {
        fclose(file_);
    }
    path_ = nullptr;
}

void ifstream::swap(ifstream &other)
{
    std::swap(file_, other.file_);
    std::swap(path_, other.path_);
}

void swap(ifstream &first, ifstream &second)
{
    first.swap(second);
}

bool ifstream::good() const
{
    return !fail() && !eof();
}

bool ifstream::eof() const
{
    return feof(file_);
}

bool ifstream::fail() const
{
    return ferror(file_);
}

ifstream& operator>>(ifstream &in, int &out)
{
    fscanf(in.file_, "%d", &out);
    return in;
}

ifstream& operator>>(ifstream &in, double &out)
{
    fscanf(in.file_, "%lf", &out);
    return in;
}

ifstream& operator>>(ifstream &in, std::string &out)
{
    int chr = fgetc(in.file_);
    while(in.good()) {
        out += (char)chr;
        chr = fgetc(in.file_);
        if (chr == '\n')
        {
            break;
        }
    }
    return in;
}
