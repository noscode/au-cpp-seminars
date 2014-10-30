#pragma once
#include <string>

struct my_ostream
{
    virtual void print(int i) = 0;
    virtual void print(double d) = 0;
    virtual void print(const std::string &str) = 0;
    virtual ~my_ostream();
};

my_ostream& operator<<(my_ostream &out, int i);
my_ostream& operator<<(my_ostream &out, double d);
my_ostream& operator<<(my_ostream &out, const std::string &str);
