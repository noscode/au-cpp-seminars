#include <stdlib.h>
#include <iostream>
#include <vector>
#include "linq.h"

int main()
{
	using namespace std;
	vector<int> a = { 1, 2, 3, 4, 5, 6 };

	auto even_seq = from(a)
		.where([](int x) { return x % 2 == 0; });

	for (auto i : even_seq.to_vector())
		cout << i << " ";
	cout << endl;

	auto squares = even_seq.select<int>([](int x){ return x * x; });

	for (auto i : squares.to_vector())
		cout << i << " ";
	cout << endl;

	cout << squares.any([](int x){ return x > 130; }) << endl;

	cout << squares.count([](int x){ return x > 15; }) << endl;

	cin.get();
	return 0;
}
