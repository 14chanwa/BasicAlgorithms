/*
* Created by 14chanwa on 2017.03.08
*/

/*
* Heaps
* Implements min (resp. max) heaps in C++. 
* These classes support extraction of the min (resp. max) element in O(1) time, insertion, deletion in O(log(n)) time.
*/

#ifndef HEAPS_H
#define HEAPS_H

#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

/*
* CLASS DECLARATION
*/

/*
* Virtual parent class. Defines methods to be implemented by Min and Max versions
*/
template <typename T>
class Heap {
public:
	inline Heap() {}

	// Basic operations
	T peek();						// returns the summit of the heap
	virtual void push(T) = 0;				// inserts a new element in the heap
	virtual T pop() = 0;					// removes the summit of the heap
	inline std::vector<T>* get_elements();			// copies elements to a new vector

	// Creation
	virtual void heapify(std::vector<T>&) = 0;		// given a vector of elements, creates a valid heap

	// Inspection
	inline int size() { return m_elements.size(); }		// returns the size of the heap
	inline bool empty() { return m_elements.empty(); }	// checks whether the heap is empty
	virtual bool check() = 0;				// checks the validity of the heap

	void print_heap();					// prints the heap in the console (as a tree form)

protected:
	std::vector<T> m_elements;				// the elements of the heap. Convention: array numbering starts at 1
	void switch_indexes(int, int);				// given 2 indexes, swap the corresponding elements
	T get_element(int);					// given an index, returns the corresponding element
};

/*
* Min version of the Heap (implements Extract-min ; each parent is greater than its children)
*/
template<typename T>
class Heap_Min : public Heap<T> {
public:
	void push(T);
	T pop();
	void heapify(std::vector<T>&);
	bool check();
};

/*
* Max version of the Heap (implements Extract-max ; each parent is smaller than its children)
*/
template<typename T>
class Heap_Max : public Heap<T> {
public:
	void push(T);
	T pop();
	void heapify(std::vector<T>&);
	bool check();
};




/*
* METHOD IMPLEMENTATION
*/

template<typename T>
T Heap<T>::peek() {
	if (m_elements.empty()) {
		return 0;
	}
	else {
		return m_elements[0];
	}
}

template<typename T>
void Heap<T>::switch_indexes(int i, int j) {
	T _tmp = m_elements[i - 1];
	m_elements[i - 1] = m_elements[j - 1];
	m_elements[j - 1] = _tmp;
}

template<typename T>
void Heap<T>::print_heap() {
	int _current_power = 0;
	int _last_checked_index = 0;
	while (_last_checked_index < m_elements.size() - 1) {
		for (int i = std::pow(2, _current_power) - 1; i < std::min<int>(std::pow(2, _current_power + 1) - 1, m_elements.size()); i++) {
			_last_checked_index = i;
			std::cout << m_elements[i] << "\t";
		}
		std::cout << std::endl;
		_current_power++;
	}
}

template<typename T>
std::vector<T>* Heap<T>::get_elements() {
	std::vector<T>* _elements_copy;
	std::copy(m_elements.begin(), m_elements.end(), _elements_copy->begin());
	return _elements_copy;
}

template<typename T>
T Heap<T>::get_element(int _index) {
	return m_elements[_index - 1];
}

template<typename T>
void Heap_Min<T>::push(T _t) {
	// Insert the element at the end of the vector
	m_elements.push_back(_t);
	// Fix the heap
	int current_index = m_elements.size(); // convention: array numbering starts with 1
	while (current_index > 1) {
		int next_index = current_index / 2;
		if (get_element(current_index) < get_element(next_index)) {
			switch_indexes(current_index, next_index);
		}
		current_index = next_index;
	}
}

template<typename T>
T Heap_Min<T>::pop() {
	if (m_elements.empty()) {
		return 0;
	}
	// Exchange first and last element of the array
	// Delete the last element of the array
	T _first_element = get_element(1);
	switch_indexes(1, m_elements.size());
	m_elements.pop_back();
	// Fix the heap
	int current_index = 1; // convention: array numbering starts with 1

	T _tmp;
	while (current_index * 2 <= m_elements.size()) {
		// If 2 children
		if (current_index * 2 + 1 <= m_elements.size()) {
			_tmp = std::min(get_element(current_index * 2), get_element(current_index * 2 + 1));
			// Check if need switch
			if (get_element(current_index) > _tmp) {
				// Switch the smallest element
				if (get_element(current_index * 2) < get_element(current_index * 2 + 1)) {
					switch_indexes(current_index, current_index * 2);
					current_index = current_index * 2;
				}
				else {
					switch_indexes(current_index, current_index * 2 + 1);
					current_index = current_index * 2 + 1;
				}
			}
			else {
				break;
			}
		}
		// If one child
		else {
			if (get_element(current_index * 2) < get_element(current_index)) {
				switch_indexes(current_index, current_index * 2);
				current_index = current_index * 2;
			}
			break;
		}

	}
	return _first_element;
}

template<typename T>
void Heap_Min<T>::heapify(std::vector<T>& _vec_t) {
	std::sort(_vec_t.begin(), _vec_t.end());
	for (std::vector<T>::iterator it = _vec_t.begin(); it < _vec_t.end(); it++) {
		m_elements.push_back(*it);
	}
}

template<typename T>
bool Heap_Min<T>::check() {
	for (int i = 1; i < m_elements.size() + 1; i++) {
		if (2 * i <= m_elements.size()) {
			if (get_element(i) > get_element(i * 2)) {
				std::cout << i << ", " << 2 * i << ", size: " << m_elements.size() << std::endl;
				return false;
			}
		}
		if (2 * i + 1 <= m_elements.size()) {
			if (get_element(i) > get_element(i * 2 + 1)) {
				std::cout << i << ", " << 2 * i + 1 << ", size: " << m_elements.size() << std::endl;
				return false;
			}
		}
	}
	return true;
}

template<typename T>
void Heap_Max<T>::push(T _t) {
	// Insert the element at the end of the vector
	m_elements.push_back(_t);
	// Fix the heap
	int current_index = m_elements.size(); // convention: array numbering starts with 1
	while (current_index > 1) {
		int next_index = current_index / 2;
		if (get_element(current_index) > get_element(next_index)) {
			switch_indexes(current_index, next_index);
		}
		current_index = next_index;
	}
}

template<typename T>
T Heap_Max<T>::pop() {
	if (m_elements.empty()) {
		return 0;
	}
	// Exchange first and last element of the array
	// Delete the last element of the array
	T _first_element = get_element(1);
	switch_indexes(1, m_elements.size());
	m_elements.pop_back();
	// Fix the heap
	int current_index = 1; // convention: array numbering starts with 1

	T _tmp;
	while (current_index * 2 <= m_elements.size()) {
		// If 2 children
		if (current_index * 2 + 1 <= m_elements.size()) {
			_tmp = std::max(get_element(current_index * 2), get_element(current_index * 2 + 1));
			// Check if need switch
			if (get_element(current_index) < _tmp) {
				// Switch the biggest element
				if (get_element(current_index * 2) > get_element(current_index * 2 + 1)) {
					switch_indexes(current_index, current_index * 2);
					current_index = current_index * 2;
				}
				else {
					switch_indexes(current_index, current_index * 2 + 1);
					current_index = current_index * 2 + 1;
				}
			}
			else {
				break;
			}
			// If one child
		}
		else {
			if (get_element(current_index * 2) > get_element(current_index)) {
				switch_indexes(current_index, current_index * 2);
				current_index = current_index * 2;
			}
			break;
		}

	}
	return _first_element;
}

template<typename T>
void Heap_Max<T>::heapify(std::vector<T>& _vec_t) {
	std::sort(_vec_t.begin(), _vec_t.end());
	for (std::vector<T>::reverse_iterator rit = _vec_t.rbegin(); rit < _vec_t.rend(); rit++) {
		m_elements.push_back(*rit);
	}
}

template<typename T>
bool Heap_Max<T>::check() {
	for (int i = 1; i < m_elements.size() + 1; i++) {
		if (2 * i <= m_elements.size()) {
			if (get_element(i) < get_element(i * 2)) {
				std::cout << i << ", " << 2 * i << ", size: " << m_elements.size() << std::endl;
				return false;
			}
		}
		if (2 * i + 1 <= m_elements.size()) {
			if (get_element(i) < get_element(i * 2 + 1)) {
				std::cout << i << ", " << 2 * i + 1 << ", size: " << m_elements.size() << std::endl;
				return false;
			}
		}
	}
	return true;
}

#endif
