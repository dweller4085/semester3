#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstdio>

#include "type_aliases.hpp"

using namespace std;

i32 partition (i32 * const a, i32 const start, i32 const end) {
	i32 pivot = a[end];
	i32 i = start - 1;
	for (i32 j = start; j <= end; j++) {
		if (a[j] <= pivot) {
			swap (a[++i], a[j]);
		}
	}
	swap (a[++i], a[end]);
	return i;
}

void qsort (i32 * const a, i32 const start, i32 const end) {
	if (start >= end) return;
	i32 pivot = partition (a, start, end);
	qsort (a, start, pivot - 1);
	qsort (a, pivot + 1, end);
}

f64 qsort (i32 * const array, i32 const size) {
	auto begin = chrono::steady_clock::now();
	qsort (array, 0, size - 1);
	auto end = chrono::steady_clock::now();
	return chrono::duration_cast<std::chrono::duration<f64>>(end - begin).count();
}

template <typename T>
void fill_arr_with_uniform_distr (vector<T> & array, T const lower, T const upper) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<T> distr(lower, upper);
	for (auto n : array) {
		n = distr(gen);
	}
}

bool is_sorted (i32 const * const array, i32 const size) {
	for (i32 i = 0; i < size - 1; ++i) {
		if (array[i] > array[i+1]) return false;
	}
	return true;
}

int main () {
	int32_t * const arr4_10 = (int32_t *) malloc (4 * 10000);
	int32_t * const arr5_10 = (int32_t *) malloc (4 * 100000);
	int32_t * const arr6_10 = (int32_t *) malloc (4 * 1000000);
	int32_t * const arr4_1000 = (int32_t *) malloc (4 * 10000);
	int32_t * const arr5_1000 = (int32_t *) malloc (4 * 100000);
	int32_t * const arr6_1000 = (int32_t *) malloc (4 * 1000000);
	int32_t * const arr4_100000 = (int32_t *) malloc (4 * 10000);
	int32_t * const arr5_100000 = (int32_t *) malloc (4 * 100000);
	int32_t * const arr6_100000 = (int32_t *) malloc (4 * 1000000);
	
	FILE * const f4_10 = fopen ("10_4.bin", "r");
	FILE * const f5_10 = fopen ("10_5.bin", "r");
	FILE * const f6_10 = fopen ("10_6.bin", "r");
	FILE * const f4_1000 = fopen ("1000_4.bin", "r");
	FILE * const f5_1000 = fopen ("1000_5.bin", "r");
	FILE * const f6_1000 = fopen ("1000_6.bin", "r");
	FILE * const f4_100000 = fopen ("100000_4.bin", "r");
	FILE * const f5_100000 = fopen ("100000_5.bin", "r");
	FILE * const f6_100000 = fopen ("100000_6.bin", "r");
	
	fread (arr4_10, 4, 10000, f4_10);
	fread (arr4_1000, 4, 10000, f4_1000);
	fread (arr4_100000, 4, 10000, f4_100000);
	fread (arr5_10, 4, 100000, f5_10);
	fread (arr5_1000, 4, 100000, f5_1000);
	fread (arr5_100000, 4, 100000, f5_100000);
	fread (arr6_10, 4, 1000000, f6_10);
	fread (arr6_1000, 4, 1000000, f6_1000);
	fread (arr6_100000, 4, 1000000, f6_100000);
	
	cout << qsort(arr4_10, 10000) /*+ qsort(arr4_10, 10000) + qsort(arr4_10, 10000)) / 3.0f*/ << endl;
	//cout << (qsort(arr5_10, 100000) + qsort(arr5_10, 100000) + qsort(arr5_10, 100000)) / 3.0f << endl;
	//cout << (qsort(arr6_10, 1000000) + qsort(arr6_10, 1000000) + qsort(arr6_10, 1000000)) / 3.0f << endl;
	//cout << (qsort(arr4_1000, 10000) + qsort(arr4_1000, 10000) + qsort(arr4_1000, 10000)) / 3.0f << endl;
	//cout << (qsort(arr5_1000, 100000) + qsort(arr5_1000, 100000) + qsort(arr5_1000, 100000)) / 3.0f << endl;
	//cout << (qsort(arr6_1000, 1000000) + qsort(arr6_1000, 1000000) + qsort(arr6_1000, 1000000)) / 3.0f << endl;
	//cout << (qsort(arr4_100000, 10000) + qsort(arr4_100000, 10000) + qsort(arr4_100000, 10000)) / 3.0f << endl;
	//cout << (qsort(arr5_100000, 100000) + qsort(arr5_100000, 100000) + qsort(arr5_100000, 100000)) / 3.0f << endl;
	//cout << (qsort(arr6_100000, 1000000) + qsort(arr6_100000, 1000000) + qsort(arr6_100000, 1000000)) / 3.0f << endl;
	
	fclose (f4_10);
	fclose (f4_1000);
	fclose (f4_100000);
	fclose (f5_10);
	fclose (f5_1000);
	fclose (f5_100000);
	fclose (f6_10);
	fclose (f6_1000);
	fclose (f6_100000);
}