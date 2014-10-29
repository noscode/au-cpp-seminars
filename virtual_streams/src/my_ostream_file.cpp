#include "my_ostream_file.h"

my_ostream_file::my_ostream_file(const char *path)
{
    file_ = fopen(path, "w");
}

void my_ostream_file::print(int i)
{
    fprintf(file_, "%d", i);
}

void my_ostream_file::print(double d)
{
    fprintf(file_, "%f", d);
}

void my_ostream_file::print(const std::string &str)
{
    fprintf(file_, "%s", str.c_str());
}

my_ostream_file::~my_ostream_file()
{
    fclose(file_);
}
