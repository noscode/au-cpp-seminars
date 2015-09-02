#include <iostream>

int main()
{
    using namespace std;
    float sum = 0.0f;
    int count = 0;

    cout << "Type the number please" << endl;
    while (true) {
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
