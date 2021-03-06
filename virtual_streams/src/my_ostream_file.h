#pragma once
#include <cstdio>
#include "my_ostream.h"

struct my_ostream_file : my_ostream
{
    explicit my_ostream_file(const char *path);

    void print(int i) override;
    void print(double d) override;
    void print(const std::string &str) override;

    ~my_ostream_file() override;
protected:
    my_ostream_file(const my_ostream_file &src);
    my_ostream_file& operator=(const my_ostream_file& src);

    FILE* file_;
};
