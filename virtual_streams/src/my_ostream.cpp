#include "my_ostream.h"

my_ostream& operator<<(my_ostream &out, int i)
{
    out.print(i);
    return out;
}

my_ostream& operator<<(my_ostream &out, double d)
{
    out.print(d);
    return out;
}

my_ostream& operator<<(my_ostream &out, const std::string &str)
{
    out.print(str);
    return out;
}

my_ostream::~my_ostream()
{
    //Nothing to destroy
}
