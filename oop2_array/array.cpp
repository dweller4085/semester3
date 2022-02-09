/*

Необходимые методы класса:
	1d. конструктор по умолчанию;
	1a. конструктор из обычного массива;
	1c. конструктор копирования;
	2. деструктор;
	3. поиск элемента (возвращает индекс первого совпавшего элемента, либо -1, если совпадений нет);
	4i. ввод в консоль;
	4o. вывод в консоль;
	5. сортировка элементов (любым алгоритмом);
	6. вставка элемента по индексу;
	7m. удаление элемента по индексу;

Необходимые перегрузки:
	7o. удаление элемента по индексу (в виде перегрузки оператора "-=")
	8. получение ссылки на элемент по индексу ([]);

*/

#include <iostream>
#include <stdexcept>
#include <utility>

using namespace std;

template <typename T>
void qsort (T * const a, int const start, int const end) {
	if (start >= end) return;
	T const pivot = a[end];
	int i = start - 1;
	for (int j = start; j <= end; j++) {
		if (a[j] <= pivot) {
			swap (a[++i], a[j]);
		}
	}
	qsort (a, start, i - 1);
	qsort (a, i + 1, end);
}

template <typename T>
class Array {
	T * m_array;
	size_t m_size;

public:
//1d
	Array () : 
		m_array (new T [1] ()),
		m_size (1) {}
	
	Array (size_t size) :
		m_array (new T [size] ()),
		m_size (size) {}

	//1a
	Array (T const * const array, size_t const size) {
		m_size = size;
		if (size == 0) {
			throw length_error ("Cannot construct from a zero-length array");
		}
		else {
			m_array = new T [size];
		}

		for (size_t i = 0; i < size; i++) {
			m_array[i] = array[i];
		}
	}

	//1c
	Array (Array const & other) {
		m_size = other.m_size;
		m_array = new T [m_size];
		for (size_t i = 0; i < m_size; i++) {
			m_array[i] = other[i];
		}
	}
	
	//2
	~Array () {
		delete [] m_array;
	}
	
	//3
	int find (T const value) const {
		for (size_t i = 0; i < m_size; i++) {
			if (m_array[i] == value) { return i; };
		}
		return -1;
	}
	
	//4o
	Array & print_to_stdout () {
		for (size_t i = 0; i < m_size; i++) {
			cout << m_array[i] << " ";
		}
		cout << endl;
		return *this;
	}

	//4i
	Array & read_from_stdin () {
		for (size_t i = 0; i < m_size; i++) {
			cin >> m_array[i];
		}
		return *this;
	}
	
	//5
	Array & sort () {
		qsort <T> (m_array, 0, m_size - 1);
		return *this;
	}
	
	//6
	Array & insert_to (size_t const index, T const & value) {
		if (index > m_size) throw out_of_range ("Cannot insert past the expanded array's end");

		T * new_adress = new T [m_size + 1];
		
		for (size_t i = 0, j = 0; i < m_size + 1;) {
			if (i == index) {
				new_adress[i] = value;
				j = 1;
				i++;
			}
			else {
				new_adress[i] = m_array[i - j];
				i++;
			}
		}

		delete [] m_array;
		m_array = new_adress;
		m_size++;

		return *this;
	}
	
	//7m
	Array & remove_at (size_t const index) {
		if (m_size - 1 == 0) throw length_error ("Cannot reduce array's size to zero elements");
		if (index > m_size - 1) throw out_of_range ("Cannot remove nonexistent elements that are past the array's end");

		T * new_adress = new T [m_size - 1];
		
		for (size_t i = 0, j = 0; i < m_size;) {
			if (i == index) {
				j = 1;
				i++;
			}
			else {
				new_adress[i - j] = m_array[i];
				i++;
			}
		}
		
		delete [] m_array;
		m_array = new_adress;
		m_size--;

		return *this;
	}
	
	//7o
	Array & operator -= (size_t const index) {
		return remove_at (index);
	}
	
	//8
	T & operator [] (size_t const index) const {
		if (index >= m_size) {
			throw out_of_range ("Cannot index past the array's end");
		}
		else {
			return m_array[index];
		}
	}


/***********************************2022-01-18*****************************************/

	T const & max () const {
		T max = m_array[0];
		size_t index = 0;
		for (size_t i = 1; i < m_size; i++) {
			if (m_array[i] > max) {
				max = m_array[i]; index = i;
			}
		}
		return m_array[index];
	}

	T const & min () const {
		T min = m_array[0];
		size_t index = 0;
		for (size_t i = 1; i < m_size; i++) {
			if (m_array[i] < min) {
				min = m_array[i]; index = i;
			}
		}
		return m_array[index];
	}

	Array & operator = (Array const & other) {
		if (this == &other) return *this;
		delete [] m_array;
		m_size = other.m_size;
		m_array = new T [m_size];
		for (size_t i = 0; i < m_size; i++) {
			m_array[i] = other.m_array[i];
		}
		return *this;
	}

	Array & operator = (Array const && other) {
		if (this == &other) return *this;
		delete [] m_array;
		m_array = other.m_array;
		m_size = other.m_size;
		return *this;
	}

	Array operator + (Array const & other) const {
		Array result (m_size + other.m_size);
		for (size_t i = 0; i < m_size; i++) {
			result[i] = m_array[i];
		}
		for (size_t i = m_size; i < m_size + other.m_size; i++) {
			result[i] = other.m_array[i];
		}
		return result;
	}

	void operator += (Array const & other) {
		operator = (move(operator + (other)));
	}

	bool operator == (Array const & other) const {
		if (m_size != other.m_size) throw length_error ("I can't");
		for (size_t i = 0; i < m_size; i++) {
			if (m_array[i] != other.m_array[i]) return false;
		}
		return true;
	}

	bool operator != (Array const & other) const {
		return not operator == (other);
	}

/**************************************************************************************/
};

int main () {
}