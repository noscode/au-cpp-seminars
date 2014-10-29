#include "my_ostream_con.h"
#include <cstdio>

void my_ostream_con::print(int i)
{
    fprintf(stdout, "%d", i);
}

void my_ostream_con::print(double d)
{
    fprintf(stdout, "%f", d);
}

void my_ostream_con::print(const std::string &str)
{
    fprintf(stdout, "%s", str.c_str());
}
