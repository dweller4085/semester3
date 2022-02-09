#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include <cstdlib>
#include <cstdio>


#include "type_aliases.hpp"


using namespace std;

f64 shellsort (i32 * const array, i32 const size) {
	auto begin = chrono::steady_clock::now();
	
	i32 k = 1;
	switch (size) {
		case 10000: k = 7; break;
		case 100000: k = 9; break;
		case 1000000: k = 11; break;
	}
	
	i32 three = 1;
	for (i32 i = 0; i < k; i++) {
		three *= 3;
	}
	
	for (i32 gap = (three - 1)/2; three != 1; three /= 3) {
		for (i32 j, i = gap; i < size; ++i) {
			i32 tmp = array[i];
			for (j = i; j >= gap and array[j - gap] > tmp; j -= gap) {
				array[j] = array[j - gap];
			}
			array[j] = tmp;
		}
	}
	
	auto end = chrono::steady_clock::now();
	return chrono::duration_cast<std::chrono::duration<f64>>(end - begin).count();
}

bool is_sorted (i32 const * const array, i32 const size) {
	for (i32 i = 0; i < size - 1; ++i) {
		if (array[i] > array[i+1]) return false;
	}
	return true;
}


i32 main () {
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
	
	
	
	cout << (shellsort(arr4_10, 10000) + shellsort(arr4_10, 10000) + shellsort(arr4_10, 10000)) / 3.0f << endl;
	cout << (shellsort(arr5_10, 100000) + shellsort(arr5_10, 100000) + shellsort(arr5_10, 100000)) / 3.0f << endl;
	cout << (shellsort(arr6_10, 1000000) + shellsort(arr6_10, 1000000) + shellsort(arr6_10, 1000000)) / 3.0f << endl;
	cout << (shellsort(arr4_1000, 10000) + shellsort(arr4_1000, 10000) + shellsort(arr4_1000, 10000)) / 3.0f << endl;
	cout << (shellsort(arr5_1000, 100000) + shellsort(arr5_1000, 100000) + shellsort(arr5_1000, 100000)) / 3.0f << endl;
	cout << (shellsort(arr6_1000, 1000000) + shellsort(arr6_1000, 1000000) + shellsort(arr6_1000, 1000000)) / 3.0f << endl;
	cout << (shellsort(arr4_100000, 10000) + shellsort(arr4_100000, 10000) + shellsort(arr4_100000, 10000)) / 3.0f << endl;
	cout << (shellsort(arr5_100000, 100000) + shellsort(arr5_100000, 100000) + shellsort(arr5_100000, 100000)) / 3.0f << endl;
	cout << (shellsort(arr6_100000, 1000000) + shellsort(arr6_100000, 1000000) + shellsort(arr6_100000, 1000000)) / 3.0f << endl;
	
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
