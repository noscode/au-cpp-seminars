#include <iostream>
#include <fstream>

int fib_rec_helper(int ix, int prev1, int prev2)
{
    return ix ? fib_rec_helper(--ix, prev2, prev1 + prev2) : prev1;
}

int fib_rec(int ix)
{
    return fib_rec_helper(ix, 0, 1);
}

int fib_it(int ix)
{
    int prev1 = 0;
    int prev2 = 1;

    while(ix > 0)
    {
        int prev2_old = prev2;
        prev2 = prev1 + prev2;
        prev1 = prev2_old;
        --ix;
    }

    return prev1;
}

int main()
{
    using namespace std;

    ifstream in("fib.in.txt");
    ofstream out("fib.out.txt");

    int fib_ix;
    in >> fib_ix;

    out << fib_rec(fib_ix) << endl;
    out << fib_it(fib_ix) << endl;
}
