#include "my_ostream_file.h"

static void finish_work_with_ostream(my_ostream *out)
{
    *out << "Hello from C++ to pure C world!" << "\n"
        << "This is int: " << 777 << "\n"
        << "This is double: " << 777.777 << "\n";
    //Compiler calls non-virtual destructor declared in my_ostream
    my_ostream_delete(out);
}

int main()
{
    //Or my_ostream_file out_derived;
    my_ostream_file *out_derived =
        my_ostream_file_new("ccout.txt");
    my_ostream *out_base = my_ostream_file_upcast(out_derived);

    finish_work_with_ostream(out_base);
    //my_ostream_file_delete(out_derived);
    return 0;
}
