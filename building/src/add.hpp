#pragma once

int add(int, int);
extern const int A1;
const int A2 = 20;

inline int& get_a()
{
    static int a = add(A1, A2);
    return a;
}
