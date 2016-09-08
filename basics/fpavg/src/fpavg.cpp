#include <iostream>

int main()
{
    using namespace std;
    float sum = 0.0f;
    size_t count = 0;

    cout << "Type 5 numbers please" << endl;
    while (count < 5) {
        float num = 0.0f;
        cin >> num;
        if (cin.fail() || cin.eof())
            break;

        sum += num;
        count++;
        cout << "current avg: " << sum / count << std::endl;
    };
    cout << "bye" << std::endl;
}
