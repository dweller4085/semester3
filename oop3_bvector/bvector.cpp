#include <iostream>
#include <stdexcept>
#include "bvector.hpp"
using namespace std;

BooleanVector::BooleanVector () {
	m_vector_size = 8;
	m_array_size = 1;
	m_array = new unsigned char [m_array_size] ();
}

BooleanVector::BooleanVector (size_t size, unsigned char value) {
	if (size == 0) throw length_error ("Cannot have a zero-length vector");

	m_vector_size = size;
	m_array_size = (m_vector_size - 1ull) / 8ull + 1ull;

	m_array = new unsigned char [m_array_size] ();

	value = value & (unsigned char) 1;

	for (size_t i = 0; i < m_array_size; i++) {
		unsigned char const zero = 0;
		m_array[i] = zero - value;
	}
}

BooleanVector::BooleanVector (char const * const array) {
	if (*array == '\0') throw length_error ("Cannot have a zero-length vector");

	size_t size;
	for (size = 1; array[size] != '\0'; size++);

	m_vector_size = size;
	m_array_size = (m_vector_size - 1ull) / 8ull + 1ull;

	m_array = new unsigned char [m_array_size] {};

	for (size_t i = 0; i < m_vector_size; i++) {
		unsigned char const one = 1;
		unsigned char k = i % 8;
		if (array[(size - 1ull) - i] != '0') {
			m_array[i/8] |= (one << k);
		}
	}
}

BooleanVector::BooleanVector (BooleanVector const & other) {
	m_vector_size = other.m_vector_size;
	m_array_size = other.m_array_size;

	m_array = new unsigned char [m_array_size] ();

	for (size_t i = 0; i < m_array_size; i++) {
		m_array[i] = other.m_array[i];
	}
}

BooleanVector::~BooleanVector () {
	delete [] m_array;
}

BooleanVector & BooleanVector::read_from_stdin () {
	unsigned char const one = 1;
	for (size_t i = 0; i < m_array_size; i++) {
		for (unsigned char j = 0; j < 8 and j + 8 * i + 1 <= m_vector_size; j++) {
			char in;
			cin >> in;
			if (in == '0') {
				m_array[i] &= ~(one << j);
			}
			else {
				m_array[i] |= (one << j);
			}
		}
	}
	return *this;
}

BooleanVector & BooleanVector::write_to_stdout () {
	unsigned char const one = 1;
	for (size_t i = 0; i < m_array_size; i++) {
		for (unsigned char j = 0; j < 8 and j + 8 * i + 1 <= m_vector_size; j++) {
			cout << (((one << j) & m_array[i]) ? '1' : '0') << " ";
		}
	}
	cout << endl;
	return *this;
}

BooleanVector & BooleanVector::invert () {
	for (size_t i = 0; i < m_array_size; i++) {
		m_array[i] = ~m_array[i];
	}
	return *this;
}

BooleanVector & BooleanVector::invert_at (size_t index) {
	if (index >= m_vector_size) throw out_of_range ("Cannot index past vector's end");
	unsigned char const one = 1;
	unsigned char k = index % 8;
	m_array[index/8] ^= one << k;
	return *this;
}

bool BooleanVector::operator [] (size_t index) const {
	if (index >= m_vector_size) throw out_of_range ("Cannot index past vector's end");
	unsigned char const one = 1;
	unsigned char k = index % 8;
	return (m_array[index/8] & (one << k));
}

BooleanVector BooleanVector::operator & (BooleanVector & other) const {
	if (other.m_array_size != m_array_size) throw invalid_argument ("Cannot perform binary operations on vectors of different length");
	BooleanVector result = other;
	for (size_t i = 0; i < m_array_size; i++) {
		result.m_array[i] = m_array[i] & other.m_array[i];
	}
	return result;
}

BooleanVector BooleanVector::operator | (BooleanVector const & other) const {
	if (other.m_array_size != m_array_size) throw invalid_argument ("Cannot perform binary operations on vectors of different length");
	BooleanVector result = other;
	for (size_t i = 0; i < m_array_size; i++) {
		result.m_array[i] = m_array[i] | other.m_array[i];
	}
	return result;
}

BooleanVector BooleanVector::operator ^ (BooleanVector & other) const {
	if (other.m_array_size != m_array_size) throw invalid_argument ("Cannot perform binary operations on vectors of different length");
	BooleanVector result = other;
	for (size_t i = 0; i < m_array_size; i++) {
		result.m_array[i] = m_array[i] ^ other.m_array[i];
	}
	return result;
}

BooleanVector & BooleanVector::operator = (BooleanVector const & other) {
	delete [] m_array;
	m_vector_size = other.m_vector_size;
	m_array_size = other.m_array_size;
	m_array = new unsigned char [m_array_size];
	for (size_t i = 0; i < m_array_size; i++) {
		m_array[i] = other.m_array[i];
	}
	return *this;
}

size_t BooleanVector::weight () const {
	size_t sum = 0;
	for (size_t i = 0; i < m_vector_size; ++i) {
		sum += (*this)[i] ? 1 : 0;
	}
	return sum;
}

BooleanVector BooleanVector::operator ~ () const {
	BooleanVector result = (*this);
	return result.invert();
}

bool BooleanVector::operator == (BooleanVector const & other) {
	for (size_t i = 0; i < m_vector_size; i++) {
		if ((*this)[i] != other[i]) return false;
	}
	return true;
}

BooleanVector & BooleanVector::set(size_t i) {
	BooleanVector mask = BooleanVector(m_vector_size, 0).invert_at(i);
	return ((*this) = operator | (mask));
}

BooleanVector & BooleanVector::clear(size_t i) {
	BooleanVector mask = BooleanVector(m_vector_size, 1).invert_at(i);
	return ((*this) = operator & (mask));
}

bool BooleanVector::operator != (BooleanVector const & other) {
	return not operator == (other);
}

BooleanVector BooleanVector::operator << (size_t k) {
	if (k >= m_vector_size) return BooleanVector(m_vector_size, 0);
	BooleanVector result (m_vector_size, 0);
	for (size_t i = k; i < m_vector_size; i++) {
		if ((*this)[i - k]) result.set(i);
	}
	return result;
}

BooleanVector BooleanVector::operator >> (size_t k) {
	if (k >= m_vector_size) return BooleanVector(m_vector_size, 0);
	BooleanVector result (m_vector_size, 0);
	for (size_t i = 0; i < m_vector_size - k; i++) {
		if ((*this)[i + k]) result.set(i);
	}
	return result;
}