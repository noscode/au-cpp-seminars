#include <string>
#include <iostream>

int main()
{
    using namespace std;

    string str;
    string str_reverse;
    cin >> str;
    str_reverse = str;

    for (size_t i = 0; i < str.size(); ++i)
    {
        str_reverse[str.size() - 1 - i] = str[i];
    }

    cout << str_reverse << endl;

    return 0;
}
