#include "ifstream.h"

ifstream::ifstream()
    : file_(nullptr)
{}

ifstream::ifstream(const std::string &path)
    : file_(fopen(path.c_str(), "r"))
    , path_(path)
{}

ifstream::ifstream(const ifstream &src)
    : file_(fopen(src.path_.c_str(), "r"))
    , path_(src.path_)
{
    fpos_t pos;
    fgetpos(src.file_, &pos);
    fsetpos(file_, &pos);
}

ifstream::ifstream(ifstream &&src)
    : ifstream()
{
    swap(src);
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
    if (file_)
        return feof(file_);
    else return true;
}

bool ifstream::fail() const
{
    if (file_)
        return ferror(file_);
    else
        return true;
}

ifstream& operator>>(ifstream &in, int &out)
{
    if (in.good())
    {
        fscanf(in.file_, "%d", &out);
    }
    return in;
}

ifstream& operator>>(ifstream &in, double &out)
{
    if (in.good())
    {
        fscanf(in.file_, "%lf", &out);
    }
    return in;
}

ifstream& operator>>(ifstream &in, std::string &out)
{
    if (in.good())
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
    }
    return in;
}
