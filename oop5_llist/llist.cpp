/*

Необходимые методы класса:
  1d. конструктор по умолчанию;
  1p. конструктор с параметрами;
  1a. конструктор из массива int;
  1c. конструктор копирования;
  2. деструктор;
  3i. ввод из консоли;
  3o. вывод в консоль;
  4. поиск элемента по ключу (возвращает указатель на элемент или nullptr, если элемента нет в списке);
  5. добавление элемента (в голову);
  6. удаление элемента (из хвоста);

Необходимые перегрузки:
  7. присваивание (=);
  8. получение ссылки на ключ элемента ([ ]);
  9. сравнение (==, !=);

В данной лабораторной требуется реализовать списочную структуру хранения целочисленных данных посредством композиции классов. Список может быть как однонаправленным, так и двунаправленным, по вашему усмотрению (для двунаправленного списка, очевидно, помимо указателя на голову требуется хранить указатель на хвост).
Для реализации необходимо создать два класса: Node (узел списка) и List (сам список).

*/

#include <iostream>
#include <stdexcept>

using std::size_t, std::cin, std::cout, std::endl, std::out_of_range, std::length_error;

class llist {
private:
	struct node {
		node * next;
		int value;
	};
	node * m_head;

public:
	//1d
	llist () {
		m_head = nullptr;
	}
	
	//1p
	llist (size_t const size, int const val) {
		if (size == 0) {
			m_head = nullptr;
			return;
		}
		else {
			m_head = new node;
			m_head->value = val;
		}
		node * current_node = m_head;
		for (size_t i = 1; i < size; i++) {
			current_node->next = new node;
			current_node = current_node->next;
			current_node->value = val;
		}
		current_node->next = nullptr;
	}
	
	//1a
	llist (int const array [], size_t const size) {
		if (size == 0) {
			m_head = nullptr;
			return;
		}
		m_head = new node; m_head->value = *array;
		node * current_node = m_head;
		for (size_t i = 1; i < size; i++) {
			current_node->next = new node;
			current_node = current_node->next;
			current_node->value = array[i];
		}
		current_node->next = nullptr;
	}
	
	//1c
	llist (llist const & other) {
		if (other.m_head == nullptr) {
			m_head = nullptr;
			return;
		}

		node * this_node = m_head = new node;
		node * other_node = other.m_head;

		while (true) {
			this_node->value = other_node->value;
			other_node = other_node->next;

			if (other_node == nullptr) {
				this_node->next = nullptr;
				break;
			}
			else {
				this_node = this_node->next = new node; 
			}
		}
	}
	
	llist (llist const && other) {
		m_head = other.m_head;
	}

	//2
	~llist () {
		node * next;
		for (node * this_node = m_head; this_node != nullptr; this_node = next) {
			next = this_node->next;
			delete this_node;
		}
	}
	
	//3i
	llist & read_from_stdin () {
		for (node * current_node = m_head; current_node != nullptr; current_node = current_node->next) {
			cin >> current_node->value;
		}
		return *this;
	}
	
	//3o
	llist & print_to_stdout () {
		for (node * current_node = m_head; current_node != nullptr; current_node = current_node->next) {
			cout << current_node->value << " ";
		} cout << endl;
		return *this;
	}
	
	//4
	node * find (int const val) const {
		for (node * current_node = m_head; current_node != nullptr; current_node = current_node->next) {
			if (current_node->value == val) { return current_node; }
		}
		return nullptr;
	}
	
	//5
	llist & insert_to_head (int const val) {
		node * next = m_head;
		m_head = new node;
		*m_head = {next, val};

		return *this;
	}
	
	llist & insert_to_tail (int const val) {
		if (m_head == nullptr) {m_head = new node {nullptr, val};}
		node * step = m_head;
		while (step->next != nullptr) {
			step = step->next;
		}
		step->next = new node {nullptr, val};
		return *this;
	}

	int max () const {
		if (m_head == nullptr) throw length_error ("aa");
		int max = m_head->value;
		for (node * step = m_head->next; step != nullptr; step = step->next) {
			if (step->value > max) max = step->value;
		}
		return max;
	}

	int min () const {
		if (m_head == nullptr) throw length_error ("aa");
		int min = m_head->value;
		for (node * step = m_head->next; step != nullptr; step = step->next) {
			if (step->value < min) min = step->value;
		}
		return min;
	}

	bool is_empty () const { return m_head == nullptr; }

	llist & clear () {
		node * next;
		for (node * this_node = m_head; this_node != nullptr; this_node = next) {
			next = this_node->next;
			delete this_node;
		}
		return *this;
	}

	llist&&  operator + (llist const & other) const {
		llist lhs = *this;

		node * step = other.m_head;
		while (step != nullptr) {
			lhs.insert_to_tail(step->value);
			step = step->next;
		}
		
		return std::move(lhs);
	}

	//6
	llist & delete_from_tail () {
		if (m_head == nullptr) {
			return *this;
		}
		else if (m_head->next == nullptr) {
			delete m_head;
			m_head = nullptr;
			return *this;
		}
		else {
			node * current_node = m_head;
			for (; current_node->next->next != nullptr; current_node = current_node->next);
			delete current_node->next;
			current_node->next = nullptr;
		}

		return *this;
	}

	llist & delete_from_head () {
		if (m_head == nullptr) {
			return *this;
		}
		node * next = m_head->next;
		delete m_head;
		m_head = next;
		return *this;
	}
	
	//7 copy assignment
	llist & operator = (llist const & other) {
		if (this == &other) return *this;
		
		node * other_node = other.m_head;
		node * this_node = m_head;
		node * this_prev;
		
		// other != 0, this != 0
		while (other_node != nullptr and this_node != nullptr) {
			this_node->value = other_node->value;
			this_prev = this_node;
			this_node = this_node->next;
			other_node = other_node->next;
		}

		// other == 0, this != 0; other == 0, this == 0
		if (other_node == nullptr) {
			while (this_node != nullptr) {
				node * next = this_node->next;
				delete this_node;
				this_node = next;
			}
			this_prev->next = nullptr;
		}
		// other != 0, this == 0
		else {
			if (m_head == nullptr) {
				this_node = m_head = new node;
				this_node->value = other_node->value;
				this_prev = this_node;
				other_node = other_node->next;
			}
			
			this_prev->next = this_node = new node;

			while (true) {
				this_node->value = other_node->value;
				other_node = other_node->next;

				if (other_node == nullptr) {
					this_node->next = nullptr;
					break;
				}
				else {
					this_node = this_node->next = new node; 
				}
			}
		}
		
		return *this;
	}
	
	//8
	int & operator [] (size_t const index) const {
		node * this_node = m_head;
		for (size_t i = 0; this_node != nullptr and i != index; i++, this_node = this_node->next);
		if (this_node == nullptr) throw out_of_range ("Cannot index past list's end");
		return this_node->value;
	}
	
	//9
	bool operator == (llist const & other) const {
		node * this_node = m_head;
		node * other_node = other.m_head;
		while (this_node != nullptr and other_node != nullptr) {
			if (this_node->value != other_node->value) return false;
			this_node = this_node->next;
			other_node = other_node->next;
		}
		
		if (this_node == other_node) {
			return true;
		}
		else {
			return false;
		}
	}
	
	bool operator != (llist const & other) const {
		return not (*this == other);
	}

};

int main () {
	int b;
	int const array2 [] = {9, 6, 3, 2, 1, 5};
  llist list_a; //1d
	llist list_b (4, 9); //1p
	llist list_c (array2, 6); //1a
	llist list_d = list_b; //1c
	list_c.print_to_stdout(); //3o
	list_a = list_c; //7
	//list_a.read_from_stdin().print_to_stdout(); // 3i, 3o
	cout << (list_a == list_c) << endl; //9
	//cout << *(list_a.find(0)) << endl; //4
	
	list_c.delete_from_tail().delete_from_tail().print_to_stdout(); //6, 3o
	list_d.insert_to_head(5555).print_to_stdout(); //5, 3o
	cout << list_c[3] << endl; //8
	llist ww = list_c + list_d;
}