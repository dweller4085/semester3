/*
Необходимые методы класса:
- конструкторы (по умолчанию, из массива char, конструктор копирования);
- деструктор;
- ввод/вывод в консоль (потоковый);
- проверка наличия элемента в множестве;
- поиск максимального/минимального элемента множества;
- получение мощности множества;

Необходимые перегрузки:
- присваивание (=);
- сравнение (==, !=);
- объединение множеств (|, |=);
- пересечение множеств (&, &=);
- разность множеств (/, /=);
- получение дополнения (~, множество, состоящее из элементов, которые не входят в исходное множество);
- добавление элемента к множеству (+, +=);
- удаление элемента из множества (-, -=);
*/

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "..\\oop3_bvector\\bvector.hpp"

using namespace std;

class Set : private BooleanVector {
public:
	Set () : BooleanVector (charset_size, 0) {}

	Set (string const & str) : BooleanVector (charset_size, 0) {
		for (const auto c : str) {
			size_t const t = static_cast<size_t>(c);
			if (t > after and t < before) {
				set (static_cast<size_t>(c));
			}
		}
	}

	Set (Set const & other) : BooleanVector (charset_size, 0)	{
		for (size_t i = 0; i < bytes_for_charset; i++) {
			m_array[i] = other.m_array[i];
		}
	}

	Set & operator = (Set const & other) {
		if (this == &other) return *this;
		for (size_t i = 0; i < bytes_for_charset; i++) {
			m_array[i] = other.m_array[i];
		}
		return *this;
	}

	Set & operator = (string const & str) {
		for (size_t i = 0; i < bytes_for_charset; i++) {
			m_array[i] = 0;
		}
		for (const auto c : str) {
			size_t const t = static_cast<size_t>(c);
			if (t > after and t < before) {
				set (static_cast<size_t>(c));
			}
		}
		return *this;
	}

	~Set () {}

	friend istream & operator >> (istream & is, Set & rhs) {
		string in;
		is >> in;
		rhs = in;
		return is;
	}

	friend ostream & operator << (ostream & os, Set & rhs) {
		string out;
		for (size_t i = 0; i < charset_size; i++) {
			if (rhs[i]) {
				out.push_back(static_cast<char>(i));
			}
		}
		os << out;
		return os;
	}

	bool has (char c) const {
		return operator [] (static_cast<size_t>(c));
	}

	size_t cardinality () const {
		return weight();
	}

	bool operator == (Set const & other) {
		for (size_t i = 0; i < m_vector_size; i++) {
			if ((*this)[i] != other[i]) return false;
		}
		return true;
	}

	bool operator != (Set const & other) {
		return not operator == (other);
	}


	Set operator | (Set const & other) const {
		Set result = *this;
		for (size_t i = 0; i < bytes_for_charset; i++) {
			result.m_array[i] |= other.m_array[i];
		}
		return result;
	}

	Set operator & (Set const & other) const {
		Set result = *this;
		for (size_t i = 0; i < bytes_for_charset; i++) {
			result.m_array[i] &= other.m_array[i];
		}
		return result;
	}

	Set operator / (Set const & other) const {
		Set result = *this;
		for (size_t i = 0; i < bytes_for_charset; i++) {
			result.m_array[i] &= (m_array[i] ^ other.m_array[i]);
		}
		return result;
	}

	Set operator ~ () {
		Set a = *this;
		a.invert();
		return a;
	}

	Set operator + (char c) {
		Set a = *this;
		a.set(static_cast<size_t>(c)); 
		return a;
	}

	Set operator - (char c) {
		Set a = *this;
		a.clear(static_cast<size_t>(c)); 
		return a;
	}

	void operator += (char c) {
		set(static_cast<size_t>(c));
	}

	void operator -= (char c) {
		clear(static_cast<size_t>(c));
	}
	

private:
	static size_t const charset_size = 256;
	static size_t const bytes_for_charset = (charset_size - 1ull) / 8ull + 1ull;
	static size_t const after = 31;
	static size_t const before = 127;
};

int main () {
	Set a {"aaaababaccc"};
	cout << a << endl;
	a += 'd';
	Set b = a + 'w';
	Set c = b / a;
	cout << (a + 'w' == b) << endl;
	cout << c << endl;

	cout << endl;
}
