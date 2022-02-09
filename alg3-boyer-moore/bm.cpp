#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include "type_aliases.hpp"

using namespace std;

static i32 const alphabet_size = 256;

i32 bm (string const & text, string const & pat) {

	size_t const n = text.length();
	size_t const m = pat.length();
	
	vector<vector<i32>> bc_table(m, vector<i32>(alphabet_size));
	
	for (i32 c = 0; c < alphabet_size; c++) {
		for (i32 i = 0; i < m; i++) {
			for (i32 j = i - 1; ; j--) {
				if (j < 0) { bc_table[i][c] = i + 1; break; }
				if ((unsigned char)pat[j] == (unsigned char)c) { bc_table[i][c] = i - j; break; }
			}
		}
	}
	
	i32 i, j, k;
	
	k = i = j = m - 1;
	while (i < n) {
		while (text[k] == pat[j]) {
			if (j == 0) return i - m + 1;
			--k; --j;
		}
		i += bc_table[j][(i32)text[k]];
		j = m - 1;
		k = i;
	}
	return -1;
}

i32 main () {
	string texts [] = {
	 //0123456789012345678901
		"greka ehal cherez reku",
		"greka ehal cherez reku",
		"greka ehal cherez reku",
		"greka ehal cherez reku",
		"greka ehal cherez reku",
		"greka ehal cherez reku",
		"greka ehal cherez reku",
		"greka ehal cherez reku"
	};
	
	string patterns [] = {
		"reka",
		"reku",
		"gr",
		"reki",
		"greki",
		"",
		" ",
		"erez "
	};
	
	for (i32 i = 0; i < 8; i++) {
		cout << bm (texts[i], patterns[i]) << endl;
	}
	return 0;
}