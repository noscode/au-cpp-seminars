#include "my_ostream_file.h"

int main()
{
    my_ostream_file cout_;
    my_ostream_file_init(&cout_, "ccout.txt");
    my_ostream &cout = *my_ostream_file_upcast(&cout_);

    cout << "Hello from C++ to pure C world!" << "\n"
        << "This is int: " << 777 << "\n"
        << "This is double: " << 777.777 << "\n";
    return 0;
}
