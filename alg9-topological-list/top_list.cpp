#include <iostream>
#include <stdexcept>
#include <iterator>
#include <vector>
#include <array>

using namespace std;

template <typename content>
class list {
public:
	struct node {
		content contents;
		node * next;
	};

	class Iterator {
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = content;
		using pointer           = content *;
		using reference         = content &;
		
		Iterator (node * ptr) : m_ptr(ptr) {}

    reference operator*() const { return m_ptr->contents; }
    pointer operator->() { return & m_ptr->contents; }
    Iterator& operator++() { m_ptr = m_ptr->next; return *this; }  
    Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
    friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
    friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };     

	private:
		node * m_ptr;
	};

	list () { head = nullptr; }
	
	list (content c) {
		head = new node {c, head};
	}

	list (list <content> const && other) {
		head = other.head;
	}

	list (list <content> const & other) {
		head = nullptr;
		node * prev = head;
		for (auto & c : other) {
			head = new node {c, prev};
			prev = head;
		}
	}

	~ list () {
		node * curr = head;
		node * next = head;
		while (curr != nullptr) {
			next = curr->next;
			delete curr;
			curr = next;
		}
	}

	list & push_frontc (content const c) {
		head = new node {c, head};
		return *this;
	}

	list & push_frontm (content const && c) {
		head = new node {c, head};
		return *this;
	}

	list & pop_front() {
		if (head == nullptr) throw out_of_range ("damn");
		node * next = head->next;
		delete head;
		head = next;
		return *this;
	}

	content & operator [] (std::size_t index) {
		node * it = head;
		node * prev = it;
		for (; it != nullptr and index != 0; ) { prev = it; it = it->next; --index; }
		if (it == nullptr) throw out_of_range ("darn it");
		return prev->contents;
	}

	bool is_empty () const { return head == nullptr; }

	Iterator const begin () const { return Iterator(head); }

	Iterator const end () const { return Iterator(nullptr); }

	friend ostream & operator << (ostream & os, list const & me) {
		for (auto & it : me) os << it << " ";
		return os;
	}
	
private:
	node * head;
};

struct vertex {
	unsigned id;
	unsigned n_incoming_edges;
	list <vertex *> out_edges;
};

int main () {
	list <vertex> vertex_list;
	vector <unsigned> solution;
	
	//auto has_this_id [] (vertex & v, unsigned id) noexcept {return v.id == id;};
	
	while (true) {
		vertex * pb;
		unsigned a, b;

		cin >> a >> b;

		if (a == 0) break;
		
		bool found = false;
		for (auto & v : vertex_list) {
			if (v.id == b) {
				found = true;
				pb = & v;
				++v.n_incoming_edges;
				break;
			}
		}

		if (not found) {
			vertex_list.push_frontm(move(vertex{b, 1, list<vertex *>()}));
			pb = & vertex_list[0];
		}

		found = false;

		for (auto & v : vertex_list) {
			if (v.id == a) {
				found = true;
				v.out_edges.push_frontc(pb);
			}
		}

		if (not found) {
			vertex_list.push_frontm(move(vertex{a, 0, list<vertex *> (pb)}));
		}
	}

	list <vertex> aa;

	for (auto & v : vertex_list) {
		if (v.n_incoming_edges == 0) {
			aa.push_frontc(v);
		}
	}

	while (not aa.is_empty()) {
		vertex p = *aa.begin();
		aa.pop_front();
		for (auto & v : p.out_edges) {
			if (--v->n_incoming_edges == 0) {
				aa.push_frontc(*v);
			}
		}
		solution.push_back(p.id);
	}
}