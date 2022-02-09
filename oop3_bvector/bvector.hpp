#include <iostream>
#include <iterator>
using std::size_t;


class BooleanVector {
public:
  class Iterator {
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = unsigned char;
		using pointer           = value_type *;
		using reference         = value_type &;
		
		Iterator (pointer ptr) : m_ptr(ptr) {}

    reference operator*() const { return *m_ptr; }
    pointer operator->() { return m_ptr; }
    Iterator& operator++() { ++m_ptr; return *this; }  
    Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
    friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
    friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };     

	private:
		pointer m_ptr;
	};

  Iterator const begin () const { return Iterator(&m_array[0]); }

	Iterator const end () const { return Iterator(&m_array[m_array_size]); }

  BooleanVector ();

  BooleanVector (size_t size, unsigned char value);

  BooleanVector (char const * const array);

  BooleanVector (BooleanVector const & other);

  ~BooleanVector ();

  BooleanVector & read_from_stdin ();

  BooleanVector & write_to_stdout ();

  BooleanVector & invert ();

  BooleanVector & invert_at (size_t index);
	
	size_t weight () const;

  bool operator [] (size_t index) const;

  BooleanVector operator & (BooleanVector & other) const;

  BooleanVector operator | (BooleanVector const & other) const;

  BooleanVector operator ^ (BooleanVector & other) const;

  BooleanVector & operator = (BooleanVector const & other);
	
	BooleanVector operator ~ () const;

  bool operator == (BooleanVector const & other);
  bool operator != (BooleanVector const & other);
	
  BooleanVector & set (size_t i);
  BooleanVector & clear (size_t i);

  BooleanVector operator << (size_t k);
  BooleanVector operator >> (size_t k);

protected:
	size_t m_vector_size;
	size_t m_array_size;
	unsigned char * m_array;
};
