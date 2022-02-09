#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

class BooleanVector {
public:
  size_t m_vector_size;
  size_t m_array_size;
  unsigned char * m_array;

  BooleanVector () {
    m_vector_size = 8;
    m_array_size = 1;
    m_array = new unsigned char [m_array_size] ();
  }

  BooleanVector (size_t size, unsigned char value) {
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

  BooleanVector (char const * const array) {
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

  BooleanVector (BooleanVector const & other) {
    m_vector_size = other.m_vector_size;
    m_array_size = other.m_array_size;

    m_array = new unsigned char [m_array_size] ();

    for (size_t i = 0; i < m_array_size; i++) {
      m_array[i] = other.m_array[i];
    }
  }

  ~BooleanVector () {
    delete [] m_array;
  }

  BooleanVector & read_from_stdin () {
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

  BooleanVector & write_to_stdout () {
    unsigned char const one = 1;
    for (size_t i = 0; i < m_array_size; i++) {
      for (unsigned char j = 0; j < 8 and j + 8 * i + 1 <= m_vector_size; j++) {
        cout << (((one << j) & m_array[i]) ? '1' : '0') << " ";
      }
    }
    cout << endl;
    return *this;
  }

  BooleanVector & invert () {
    for (size_t i = 0; i < m_array_size; i++) {
      m_array[i] = ~m_array[i];
    }
    return *this;
  }

  BooleanVector & invert (size_t index) {
    if (index >= m_vector_size) throw out_of_range ("Cannot index past vector's end");
    unsigned char const one = 1;
    unsigned char k = index % 8;
    m_array[index/8] ^= one << k;
    return *this;
  }

  bool operator [] (size_t index) const {
    if (index >= m_vector_size) throw out_of_range ("Cannot index past vector's end");
    unsigned char const one = 1;
    unsigned char k = index % 8;
    return (m_array[index/8] & (one << k));
  }

  BooleanVector operator & (BooleanVector & other) const {
    if (other.m_array_size != m_array_size) throw invalid_argument ("Cannot perform binary operations on vectors of different length");
    BooleanVector result = other;
    for (size_t i = 0; i < m_array_size; i++) {
      result.m_array[i] = m_array[i] & other.m_array[i];
    }
    return result;
  }

  BooleanVector operator | (BooleanVector const & other) const {
    if (other.m_array_size != m_array_size) throw invalid_argument ("Cannot perform binary operations on vectors of different length");
    BooleanVector result = other;
    for (size_t i = 0; i < m_array_size; i++) {
      result.m_array[i] = m_array[i] | other.m_array[i];
    }
    return result;
  }

  BooleanVector operator ^ (BooleanVector & other) const {
    if (other.m_array_size != m_array_size) throw invalid_argument ("Cannot perform binary operations on vectors of different length");
    BooleanVector result = other;
    for (size_t i = 0; i < m_array_size; i++) {
      result.m_array[i] = m_array[i] ^ other.m_array[i];
    }
    return result;
  }

  BooleanVector & operator = (BooleanVector const & other) {
    delete [] m_array;
    m_vector_size = other.m_vector_size;
    m_array_size = other.m_array_size;
    m_array = new unsigned char [m_array_size];
    for (size_t i = 0; i < m_array_size; i++) {
      m_array[i] = other.m_array[i];
    }
    return *this;
  }
	
	size_t weight () const {
		size_t sum = 0;
		for (size_t i = 0; i < m_vector_size; ++i) {
			sum += (*this)[i] ? 1 : 0;
		}
		return sum;
	}
	
	BooleanVector operator ~ () const {
		BooleanVector result = (*this);
		return result.invert();
	}

  bool operator == (BooleanVector const & other) {
    for (size_t i = 0; i < m_vector_size; i++) {
      if ((*this)[i] != other[i]) return false;
    }
    return true;
  }
};

class BooleanMatrix {
  size_t m_row_count;
  size_t m_column_count;
  BooleanVector * m_vector_array;

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

  BooleanVector AND_all_rows () const {
    BooleanVector result = m_vector_array[0];
    for (size_t row = 1; row < m_row_count; row++) {
      result = result & m_vector_array[row];
    }
    return result;
  }

  BooleanVector OR_all_rows () const {
    BooleanVector result = m_vector_array[0];
    for (size_t row = 1; row < m_row_count; row++) {
      result = result | m_vector_array[row];
    }
    return result;
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

};

int main (int argc, char ** argv) {
	size_t const N =5;

  char const * order1 [] = {
    "0000000",
    "0000001",
    "0000000",
    "0010000",
    "0000101",
    "0010110",
    "0000001"
  };

  char const * order2 [] = {
    "00110",
    "10000",
    "01000",
    "00010",
    "01000"
  };

  BooleanMatrix graph (order2, N, N);
	//graph.read_from_stdin();
	
	vector<size_t> ordered_sequence; 
	
	BooleanVector removed {N, 0};
	BooleanVector to_remove {N, 0};
  BooleanVector prev {N, 1};
	
	while (removed.weight() != N) {
		to_remove = ~ (removed | graph.OR_all_rows());
		removed = removed | to_remove;
    if (prev == removed) {throw logic_error("goddamn");}
    prev = removed;
		for (size_t i = 0; i < N; ++i) {
			if (to_remove[i]) {
        graph[i] = BooleanVector(N, 0);
				ordered_sequence.push_back(i + 1);
			}
		}
	}
	
	cout << "Result:" << endl;
	for (auto elem : ordered_sequence) {
		cout << elem << " ";
	} cout << endl;
	
	return 0;
}
