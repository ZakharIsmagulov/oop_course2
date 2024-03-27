#pragma once
#include <string>

template<class T> class Container {
struct Cell;

public:
	class Iterator {
	public:
		Iterator(Cell *cell);
		Iterator &operator=(Iterator const &other);
		Iterator &operator++();
		Iterator operator++(int i);
		T &operator*() const;
		T *operator&() const;
		T *operator->() const;
		bool operator==(Iterator const &other) const;
		bool operator!=(Iterator const &other) const;
	private:
		Cell *m_current;
	};

	Container();
	Container(T const &value);
	~Container();

	void push_front(T const &value);
	void push_back(T const &value);
	void del_head();
	void del_tail();
	T &get_head() const;
	void print_head() const;
	T &get_tail() const;
	void print_tail() const;
	Iterator begin() const;
	Iterator end() const;
	int get_count() const;
	bool const is_empty() const;
	static void swap(Container<T> &cont1, Container<T> &cont2);
	void reverse();
	void clear();

private:
	struct Cell {
		Cell(T const &value);
		T m_value;
		Cell *m_prev;
		Cell *m_next;
	};

	Cell *m_head;
	Cell *m_tail;
	int m_count;
};

template<class T> Container<T>::Cell::Cell(T const &value) {
	m_value = value;
	m_next = nullptr;
	m_prev = nullptr;
}

template<class T> Container<T>::Iterator::Iterator(Cell *cell) {
	m_current = cell;
}

template<class T> typename Container<T>::Iterator &Container<T>::Iterator::operator=(Container<T>::Iterator const &other) {
	m_current = other.m_current;
	return *this;
}

template<class T> typename Container<T>::Iterator &Container<T>::Iterator::operator++() {
	m_current = m_current->m_next;
	return *this;
}

template<class T> typename Container<T>::Iterator Container<T>::Iterator::operator++(int i) {
	Iterator temp(m_current);
	m_current = m_current->m_next;
	return temp;
}

template<class T> T &Container<T>::Iterator::operator*() const {
	return m_current->m_value;
}

template<class T> T *Container<T>::Iterator::operator&() const {
	return &(m_current->data);
}

template<class T> T *Container<T>::Iterator::operator->() const {
	return &(m_current->data);
}

template<class T> bool Container<T>::Iterator::operator==(Container<T>::Iterator const &other) const {
	return m_current == other.m_current;
}

template<class T> bool Container<T>::Iterator::operator!=(Container<T>::Iterator const &other) const {
	return m_current != other.m_current;
}

template<class T> Container<T>::Container() {
	m_head = nullptr;
	m_tail = nullptr;
	m_count = 0;
}

template<class T> Container<T>::Container(T const &value) {
	m_head = new Cell<T>(value);
	m_tail = m_head;
	m_count = 1;
}

template<class T> Container<T>::~Container() {
	clear();
}

template<class T> void Container<T>::push_front(T const &value) {
	if (!m_head) {
		m_head = new Cell(value);
		m_head->m_prev = nullptr;
		m_head->m_next = nullptr;
		m_tail = m_head;
	}
	else {
		Cell *temp = new Cell(value);
		m_head->m_prev = temp;
		temp->m_next = m_head;
		m_head = temp;
	}
	++m_count;
}

template<class T> void Container<T>::push_back(T const &value) {
	if (!m_head) {
		m_head = new Cell(value);
		m_head->m_prev = nullptr;
		m_head->m_next = nullptr;
		m_tail = m_head;
	}
	else {
		Cell *temp = new Cell(value);
		m_tail->m_next = temp;
		temp->m_prev = m_tail;
		m_tail = temp;
	}
	++m_count;
}

template<class T> void Container<T>::del_head() {
	if (!m_head) {
		throw std::out_of_range("Error: container is empty");
	}
	else if (!m_head->m_next) {
		delete m_head;
		m_head = nullptr;
		m_tail = nullptr;
		--m_count;
	}
	else {
		Cell *temp = m_head->m_next;
		temp->m_prev = nullptr;
		delete m_head;
		m_head = temp;
		--m_count;
	}
}

template<class T> void Container<T>::del_tail() {
	if (!m_head) {
		throw std::out_of_range("Error: container is empty");
	}
	else if (!m_head->m_next) {
		delete m_head;
		m_head = nullptr;
		m_tail = nullptr;
		--m_count;
	}
	else {
		Cell *temp = m_tail->m_prev;
		temp->m_next = nullptr;
		delete m_tail;
		m_tail = temp;
		--m_count;
	}
}

template<class T> T &Container<T>::get_head() const {
	if (m_head) {
		return m_head->m_value;
	}
	else {
		throw std::out_of_range("Error: container is empty");
	}
}

template<class T> void Container<T>::print_head() const {
	if (!m_head)
		throw std::out_of_range("Error: container is empty");
	std::cout << *m_head << '\n';
}

template<class T> T &Container<T>::get_tail() const {
	if (m_head) {
		return m_tail->m_value;
	}
	else {
		throw std::out_of_range("Error: container is empty");
	}
}

template<class T> void Container<T>::print_tail() const {
	if (!m_head)
		throw std::out_of_range("Error: container is empty");
	std::cout << *m_tail << '\n';
}

template<class T> typename Container<T>::Iterator Container<T>::begin() const {
	if (m_head) {
		return Iterator(m_head);
	}
	else {
		throw std::out_of_range("Error: container is empty");
	}
}

template<class T> typename Container<T>::Iterator Container<T>::end() const {
	if (m_head) {
		return Iterator(nullptr);
	}
	else {
		throw std::out_of_range("Error: container is empty");
	}
}

template<class T> int Container<T>::get_count() const {
	return m_count;
}

template<class T> bool const Container<T>::is_empty() const {
	return m_count == 0;
}

template<class T> void Container<T>::swap(Container<T> &cont1, Container<T> &cont2) {
	Cell *temp_head1 = cont1.m_head;
	Cell *temp_tail1 = cont1.m_tail;
	int temp_count1 = cont1.m_count;
	cont1.m_head = cont2.m_head;
	cont1.m_tail = cont2.m_tail;
	cont1.m_count = cont2.m_count;
	cont2.m_head = temp_head1;
	cont2.m_tail = temp_tail1;
	cont2.m_count = temp_count1;
}

template<class T> void Container<T>::reverse() {
	if (m_head->m_next) {
		Cell *cur = m_head;
		Cell *temp_next = cur->m_next;
		while (true) {
			cur->m_next = cur->m_prev;
			cur->m_prev = temp_next;
			cur = temp_next;
			if (cur != nullptr) {
				temp_next = cur->m_next;
			}
			else {
				break;
			}
		}
		Cell *temp_head = m_head;
		m_head = m_tail;
		m_tail = temp_head;
	}
}

template<class T> void Container<T>::clear() {
	while (m_count != 0) {
		del_head();
	}
}
