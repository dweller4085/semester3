#include <iostream>
#include <chrono>
#include <random>
#include <cstdlib>
#include <cstdio>

#include "type_aliases.hpp"

using namespace std;


#define il(i) (2 * i + 1)
#define ir(i) (2 * i + 2)
#define ip(i) ((i - 1) / 2)

void sift_down (i32 * const a, i32 const start, i32 const end) {
	i32 r = start;
	while (il(r) <= end) {
		i32 c = il(r);
		i32 t = r;
		
		if (a[t] < a[c]) {
			t = c;
		}
		
		if (c + 1 <= end and a[t] < a[c + 1]) {
			t = c + 1;
		}

		if (t == r) {
			break;
		} else {
			swap (a[r], a[t]);
			r = t;
		}
	}
}

f64 heapsort (i32 * const a, i32 const size) {
	auto begin = chrono::steady_clock::now();
	
	i32 const e = size - 1;
	i32 const s = 0;
	
	{ //heapify
		for (i32 start = ip(e); start >= 0; --start) {
			sift_down (a, start, e);
		}
	}
	
	for (i32 end = e; end > 0;) {
		swap (a[s], a[end]);
		--end;
		sift_down (a, s, end);
	}
	
	auto end = chrono::steady_clock::now();
	return chrono::duration_cast<std::chrono::duration<f64>>(end - begin).count();
}


int main() {
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
	
	cout << (heapsort(arr4_10, 10000) + heapsort(arr4_10, 10000) + heapsort(arr4_10, 10000)) / 3.0f << endl;
	cout << (heapsort(arr5_10, 100000) + heapsort(arr5_10, 100000) + heapsort(arr5_10, 100000)) / 3.0f << endl;
	cout << (heapsort(arr6_10, 1000000) + heapsort(arr6_10, 1000000) + heapsort(arr6_10, 1000000)) / 3.0f << endl;
	cout << (heapsort(arr4_1000, 10000) + heapsort(arr4_1000, 10000) + heapsort(arr4_1000, 10000)) / 3.0f << endl;
	cout << (heapsort(arr5_1000, 100000) + heapsort(arr5_1000, 100000) + heapsort(arr5_1000, 100000)) / 3.0f << endl;
	cout << (heapsort(arr6_1000, 1000000) + heapsort(arr6_1000, 1000000) + heapsort(arr6_1000, 1000000)) / 3.0f << endl;
	cout << (heapsort(arr4_100000, 10000) + heapsort(arr4_100000, 10000) + heapsort(arr4_100000, 10000)) / 3.0f << endl;
	cout << (heapsort(arr5_100000, 100000) + heapsort(arr5_100000, 100000) + heapsort(arr5_100000, 100000)) / 3.0f << endl;
	cout << (heapsort(arr6_100000, 1000000) + heapsort(arr6_100000, 1000000) + heapsort(arr6_100000, 1000000)) / 3.0f << endl;
	
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