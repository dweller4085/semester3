/*
Необходимые методы класса:
- конструкторы (по умолчанию, с параметрами (количество строк/столбцов и значения разрядов), конструктор из матрицы char, конструктор копирования);
- деструктор;
- ввод/вывод в консоль;
- вес матрицы (количество единичных компонент);
- конъюнкция всех строк (возвращает булев вектор);

Необходимые перегрузки:
- присваивание (=);
- получение строки ([ ]);
- построчное побитовое умножение (&);
- построчное побитовое сложение (|);
*/

#include <iostream>
#include <stdexcept>

#include "..//oop3_bvector//bvector.hpp"

using namespace std;

class BooleanMatrix {
public:
  BooleanMatrix () {
    m_row_count = 1;
    m_vector_array = new BooleanVector [m_row_count] ();
  }

  BooleanMatrix (size_t n_rows, size_t n_columns, unsigned char value) {
    if (n_rows == 0 or n_columns == 0) throw length_error ("Cannot allow matrix have a zero dimension");

    m_row_count = n_rows;
    m_column_count = n_columns;

    m_vector_array = new BooleanVector [m_row_count];

    unsigned char const zero = 0;
    value &= (unsigned char) 1;

    for (size_t row = 0; row < m_row_count; row++) {
      m_vector_array[row] = BooleanVector (m_column_count, value);
    }
  }

  BooleanMatrix (char const ** const array, size_t n_rows, size_t n_columns) {
    if (n_rows == 0 or n_columns == 0) throw length_error ("Cannot allow matrix have a zero dimension");

    m_row_count = n_rows;
    m_column_count = n_columns;

    m_vector_array = new BooleanVector [m_row_count];

    for (size_t row = 0; row < n_rows; row++) {
      m_vector_array[row] = BooleanVector(array[row]);
    }
  }

  BooleanMatrix (BooleanMatrix const & other) {
    m_row_count = other.m_row_count;
    m_column_count = other.m_column_count;

    m_vector_array = new BooleanVector [m_row_count];

    for (size_t row = 0; row < m_row_count; row++) {
      m_vector_array[row] = other.m_vector_array[row];
    }
  }

  ~BooleanMatrix () {
    delete [] m_vector_array;
  }

  BooleanMatrix & read_from_stdin () {
    for (size_t row = 0; row < m_row_count; row++) {
      m_vector_array[row].read_from_stdin();
    }
    return *this;
  }

  BooleanMatrix & write_to_stdout () {
    for (size_t row = 0; row < m_row_count; row++) {
      m_vector_array[row].write_to_stdout();
    }
    return *this;
  }

  size_t weight () const {
    size_t total = 0;
    for (size_t row = 0; row < m_row_count; row++) {
      for (size_t column = 0; column < m_column_count; column++) {
        total += m_vector_array[row][column];
      }
    }
    return total;
  }

  BooleanVector AND_all_rows () {
    BooleanVector result = m_vector_array[0];
    for (size_t row = 1; row < m_row_count; row++) {
      result = result & m_vector_array[row];
    }
    return result;
  }

  BooleanVector OR_all_rows () {
    BooleanVector result = m_vector_array[0];
    for (size_t row = 1; row < m_row_count; row++) {
      result = result | m_vector_array[row];
    }
    return result;
  }

  size_t row_weight (size_t i) {
    return operator [](i).weight();
  }

  BooleanMatrix & invert (size_t row, size_t column) {
    operator[](row).invert_at(column);
    return *this;
  }

  BooleanMatrix & set (size_t row, size_t column) {
    operator[](row).set(column);
    return *this;
  }

  BooleanMatrix & clear (size_t row, size_t column) {
    operator[](row).clear(column);
    return *this;
  }

  BooleanMatrix & operator = (BooleanMatrix & other) {
		if (this == &other) return *this;
    delete [] m_vector_array;
    
    m_row_count = other.m_row_count;
    m_column_count = other.m_column_count;

    m_vector_array = new BooleanVector [m_row_count];

    for (size_t row = 0; row < m_row_count; row++) {
      m_vector_array[row] = other.m_vector_array[row];
    }
    
    return *this;
  }

  BooleanVector & operator [] (size_t index) {
    if (index >= m_row_count) throw out_of_range ("Cannot index past matrix end");
    return m_vector_array[index];
  }

  BooleanMatrix operator & (BooleanMatrix & other) {
    if (other.m_row_count != m_row_count or other.m_column_count != m_column_count) throw invalid_argument ("Cannot perform binary operations on matrices of different dimensions");
    BooleanMatrix result = other;

    for (size_t row = 0; row < m_row_count; row++) {
      result.m_vector_array[row] = m_vector_array[row] & other.m_vector_array[row];
    }

    return result;
  }

  BooleanMatrix operator | (BooleanMatrix & other) {
    if (other.m_row_count != m_row_count or other.m_column_count != m_column_count) throw invalid_argument ("Cannot perform binary operations on matrices of different dimensions");
    BooleanMatrix result = other;

    for (size_t row = 0; row < m_row_count; row++) {
      result.m_vector_array[row] = m_vector_array[row] | other.m_vector_array[row];
    }

    return result;
  }

  BooleanMatrix operator ^ (BooleanMatrix & other) {
    if (other.m_row_count != m_row_count or other.m_column_count != m_column_count) throw invalid_argument ("Cannot perform binary operations on matrices of different dimensions");
    BooleanMatrix result = other;

    for (size_t row = 0; row < m_row_count; row++) {
      result.m_vector_array[row] = m_vector_array[row] ^ other.m_vector_array[row];
    }

    return result;
  }

  BooleanMatrix operator ~ () {
    BooleanMatrix result = *this;
    for (size_t row = 0; row < m_row_count; row++) {
      result.m_vector_array[row] = ~m_vector_array[row];
    }
    return result;
  }

private:
  size_t m_row_count;
  size_t m_column_count;
  BooleanVector * m_vector_array;
};

int main () {
  //char const bstr [4][5] = {"0100", "0011", "1010", "1110"};
  //char const ** ppbstr = bstr; 
  //BooleanMatrix a (ppbstr, 4, 4);

  BooleanMatrix a (4, 4, 0);
  BooleanMatrix b (4, 4, 1);
  b.write_to_stdout();
  cout << b.weight() << endl;
  a.read_from_stdin();
  cout << "------AND all rows:------" << endl;
  a.AND_all_rows().write_to_stdout();
  cout << "------b[2] inverted:------" << endl;
  BooleanMatrix c = a;
  b[2].invert().write_to_stdout();
  cout << "------a | b------" << endl;
  b = (c | b).write_to_stdout();

}