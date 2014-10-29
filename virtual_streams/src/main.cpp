#include "my_ostream_con.h"
#include "my_ostream_file.h"

my_ostream_con cout;
my_ostream_file cerr("errors.txt");

int main()
{
    cout << "Hello world on console!" << "\n"
        << "1234: " << 1 << 2 << 3 << 4 << "\n"
        << "777.777: " << 777.777 <<  "\n";
    cerr << "Hello world on error stream!" << "\n"
        << "No errors, just junk!" << "\n"
        << "1234: " << 1 << 2 << 3 << 4 << "\n"
        << "777.777: " << 777.777 <<  "\n";
    return 0;
}
