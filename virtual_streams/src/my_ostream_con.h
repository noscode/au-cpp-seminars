#pragma once
#include "my_ostream.h"

struct my_ostream_con : my_ostream
{
    void print(int i) override;
    void print(double d) override;
    void print(const std::string &str) override;
};
