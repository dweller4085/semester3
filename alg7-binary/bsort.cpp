#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

#include "type_aliases.hpp"

using namespace std;

inline u32 bitmask (u32 const bvector, u32 const k) {
	u32 const one = 1;
	return (bvector & (one << k)) >> k;
}

void bsort (vector<u32> & array, i32 const start, i32 const end, u32 const k) {
	if (start >= end or k < 0) return;
	i32 i = start, j = end;
	while (i <= j) {
		for (; i <= j and bitmask(array[i], k) == 0; i++);
		for (; i <= j and bitmask(array[j], k) == 1; j++);
		if (i < j) {
			swap (array[i], array[j]);
			i++; j--;
		}
	}
	bsort(array, start, j, k - 1);
	bsort(array, i, end, k - 1);
}

f64 bsort (vector<u32> & array) {
	auto begin = chrono::steady_clock::now();
	bsort (array, 0, array.size() - 1, 31);
	auto end = chrono::steady_clock::now();
	return chrono::duration_cast<std::chrono::duration<f64>>(end - begin).count();
}

int main () {
	vector<u32> a = {6, 3, 5423, 122, 776, 2};
	cout << bsort (a) << endl;
}