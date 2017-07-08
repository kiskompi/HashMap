#pragma once
#include <string>
#include <sstream>
#include <future>
#include <vector>

#include "NoEntryException.h"

template <typename Key, typename Val>
class HashMap
{
	/// This class contains a key-value pair and a pointer to the next node with the same key.
	struct Node {
		bool	initialized;		/// does it have a valid stored value
		bool	has_next;			/// does it have an initialized next node
		Key		key;					/// Hash key
		Val		val;					/// stored value
		Node*	next;	/// next node in the bucket

		Node();
		Node(Key&, Val&);
		~Node();

		/// return the value
		inline Val get() { return val; }
	};
	
public:
	// Iterators
	typedef std::allocator<Node> allocator_type;
	typedef typename std::allocator<Node>::value_type			value_type;
	typedef typename std::allocator<Node>::reference			reference;
	typedef typename std::allocator<Node>::const_reference		const_reference;
	typedef typename std::allocator<Node>::difference_type		difference_type;
	typedef typename std::allocator<Node>::size_type			size_type;

	class iterator  {
		Node* pointed;
	public:
		typedef typename std::allocator<Node>::difference_type	difference_type;
		typedef typename std::allocator<Node>::value_type		value_type;
		typedef typename std::allocator<Node>::reference		reference;
		typedef typename std::allocator<Node>::pointer			pointer;
		typedef std::random_access_iterator_tag					iterator_category;

		iterator();
		iterator(Node&);
		iterator(const iterator&);
		~iterator();

		iterator& operator=(const iterator&);
		bool operator==(const iterator&) const;
		bool operator!=(const iterator&) const;
		//bool operator<(const iterator&) const; //optional
		//bool operator>(const iterator&) const; //optional
		//bool operator<=(const iterator&) const; //optional
		//bool operator>=(const iterator&) const; //optional

		iterator& operator++();
		//iterator operator++(int); //optional
		//iterator& operator--(); //optional
		//iterator operator--(int); //optional
		//iterator& operator+=(size_type); //optional
		//iterator operator+(size_type) const; //optional
		//friend iterator operator+(size_type, const iterator&); //optional
		//iterator& operator-=(size_type); //optional            
		//iterator operator-(size_type) const; //optional
		//difference_type operator-(iterator) const; //optional

		reference operator*() const;
		pointer operator->() const;
		//reference operator[](size_type) const; //optional
	};
	class const_iterator {
		Node* pointed;
	public:
		typedef typename std::allocator<Node>::difference_type	difference_type;
		typedef typename std::allocator<Node>::value_type		value_type;
		typedef typename const std::allocator<Node>::reference	reference;
		typedef typename const std::allocator<Node>::pointer	pointer;
		typedef std::random_access_iterator_tag					iterator_category;

		const_iterator();
		const_iterator(Node*);
		const_iterator(const const_iterator&);
		const_iterator(const iterator&);
		~const_iterator();

		const_iterator& operator=(const const_iterator&);
		bool operator==(const const_iterator&) const;
		bool operator!=(const const_iterator&) const;
		//bool operator<(const const_iterator&) const; //optional
		//bool operator>(const const_iterator&) const; //optional
		//bool operator<=(const const_iterator&) const; //optional
		//bool operator>=(const const_iterator&) const; //optional

		//const_iterator& operator++();
		//const_iterator operator++(int); //optional
		//const_iterator& operator--(); //optional
		//const_iterator operator--(int); //optional
		//const_iterator& operator+=(size_type); //optional
		//const_iterator operator+(size_type) const; //optional
		//friend const_iterator operator+(size_type, const const_iterator&); //optional
		//const_iterator& operator-=(size_type); //optional            
		//const_iterator operator-(size_type) const; //optional
		//difference_type operator-(const_iterator) const; //optional

		reference operator*() const;
		pointer operator->() const;
		reference operator[](size_type) const; //optional
	};

private:
	//private fields
	size_t	MAX_LENGTH = 2000;
	Node*	table = new Node[MAX_LENGTH];

	iterator* _begin;
	static iterator* _end;
	iterator* _current;

public:
	// Iterator functions
	HashMap& operator=(const HashMap&);
	bool operator==(const HashMap&) const;
	bool operator!=(const HashMap&) const;
	//bool operator<(const X&) const; //optional
	//bool operator>(const X&) const; //optional
	//bool operator<=(const X&) const; //optional
	//bool operator>=(const X&) const; //optional

	iterator begin();
	const_iterator begin() const;
	const_iterator cbegin() const;
	iterator end();
	const_iterator end() const;
	const_iterator cend() const;




	// private methods
	/// Hash functions
	unsigned long int hash(Key) const;
	unsigned int _count(unsigned int begin, unsigned int end) const;
	void _destroy_fragment(unsigned int begin, unsigned int end);
	unsigned long int to_uint(Key) const;

	// HashFunc hash;

	/// Puts a new object to the hash map
	void put(Key, Val);
	/// Returns the object with the given key from the hash map
	Val* get(const Key&) const;
	/// Deletes an object from the hash map
	void del(Key&);
	/// Returns the number of stored object
	unsigned int size() const;
	/// Returns True if the hash map is empty, False otherwise
	bool empty();
	///Getter
	Node* get_table() const;

	HashMap();
	HashMap(const HashMap&);
	~HashMap();
};

// Node functions

template<typename Key, typename Val>
HashMap<Key, Val>::Node::Node()
{
	// creates an empty placeholder Node (maybe later can 
	// be reimplemented using allocator)
	initialized = false;
	has_next = false;
	key = NULL;
	next = nullptr;
}

template<typename Key, typename Val>
HashMap<Key, Val>::Node::Node(Key& in_key, Val& in_val)
{
	initialized = true;
	has_next = false;
	key = in_key;
	val = in_val;
	next = nullptr;
}

template<typename Key, typename Val>
HashMap<Key, Val>::Node::~Node()
{

}

// HashMap functions
template<typename Key, typename Val>
HashMap<Key, Val>::HashMap()
{
	// creates an array of size MAX_LENGTH filled with empty
	// (uninitialized) Nodes
	for (auto i = 0; i < MAX_LENGTH; ++i)
		table[i] = Node();
	this->_begin = new iterator(table[0]);
	this->_end = _begin;
	this->_current = _end;
}

template<typename Key, typename Val>
HashMap<Key, Val>::HashMap(const HashMap& other)
{
	// Creates shallow copy of the table
	this->table = other.table;
	this->MAX_LENGTH = other.MAX_LENGTH;
	this->_end = other->_end;
	this->_begin = &table[0];
	this->_current = other->_current;
}

template<typename Key, typename Val>
HashMap<Key, Val>::~HashMap()
{
	unsigned int nthreads = std::thread::hardware_concurrency();
	std::vector<std::thread> counters;

	// destroys the buckets in paralell threads (# of logical cores)
	for (unsigned int i = 0; i < nthreads; i++)
	{
		size_t a = (MAX_LENGTH / nthreads*i);
		size_t b = (MAX_LENGTH / nthreads*(i + 1));

		counters.push_back(std::thread(&HashMap<Key, Val>::_destroy_fragment, this, a, b));
	}

	for (size_t i = 0; i < nthreads; i++)
	{
		counters[i].join();
	}

}
template<typename Key, typename Val>
inline unsigned long int HashMap<Key, Val>::hash(const Key key) const
{
	// casts the Key to unsigned int and counts the hash on the value
	return to_uint(key) % MAX_LENGTH;
}

template<typename Key, typename Val>
void HashMap<Key, Val>::put(Key key, Val value)
{
	int key_hash = this->hash(key);

	if (!table[key_hash].initialized) {
		// ha még nincs Node ilyen key-el tárolva, berakjuk elsőnek
		this->table[key_hash] = Node(key, value);
	}
	else {
		// ha már van, akkor belinkeljük a Node lista utolsó helyére
		Node *act = &table[key_hash];
		while (act->has_next) {
			//std::cout <<act->val<< "->" << act->next->val <<" ";
			act = act->next;
		}
		act->next = new Node(key, value);
		act->has_next = true;
		// std::cout << "LINKING " << act->next->val << " TO " << act->val << " IN " << key_hash << std::endl;
	}

}

template<typename Key, typename Val>
unsigned int HashMap<Key, Val>::_count(unsigned int begin, unsigned int end) const {
	// counts the nodes in a part of the table (between indexes: begin and end)
	unsigned int count = 0;
	for (size_t i = begin; i < end; i++)
	{
		if (table[i].initialized) {
			Node *act = &table[i];
			count++;
			while (act->has_next) {
				// std::cout << act->val << "->" << act->next->val << " ";
				act = act->next;
				count++;
			}
		}
	}

	return count;
}

template<typename Key, typename Val>
unsigned int HashMap<Key, Val>::size() const {
	// counts the number of initialized Nodes in the HashMap. Runs as many paralell threads as the # of logical cores.
	unsigned int nthreads = std::thread::hardware_concurrency();

	// std::thread counters[nthreads];

	std::future<unsigned int> *counters = new std::future<unsigned int>[nthreads];

	for (unsigned int i = 0; i < nthreads; i++)
	{
		size_t a = (MAX_LENGTH / nthreads*i);
		size_t b = (MAX_LENGTH / nthreads*(i + 1));

		//std::cout << a << ", " << b << std::endl;
		counters[i] = std::async(std::launch::async, &HashMap<Key, Val>::_count, this, a, b);
		//std::thread(_count, MAX_LENGTH/nthreads*i, MAX_LENGTH / nthreads*(i+1));
	}

	for (size_t i = 0; i < nthreads; i++)
	{
		counters[i].wait();
	}

	unsigned int count = 0;
	for (unsigned int i = 0; i < nthreads; i++)
	{

		unsigned int c = counters[i].get();
		// std::cout << c <<std::endl;
		count += c;
	}
	return count;
}


template<typename Key, typename Val>
Val* HashMap<Key, Val>::get(const Key & key) const
{
	unsigned int key_hash = hash(key);
	if (table[key_hash].initialized) {
		// returns the element if it is the only element in the bucket
		if (!table[key_hash].has_next) {
			return &table[key_hash].val;
		}
		// if there are multiple elements in the bucket it starts a linear search for the appropriate key
		Node *act = &table[key_hash];
		while (act->has_next) {
			act = act->next;
			if (act->key == key)
				return &(act->val);
		}
		std::cout << "No entry found" << std::endl;

	}
	else
	{
		std::cout << "No entry found" << std::endl;
	}
	return nullptr;

}

template<typename Key, typename Val>
inline void HashMap<Key, Val>::del(Key & in_key)
{
	unsigned int key_hash = hash(in_key);
	if (!table[key_hash].has_next && table[key_hash].key == in_key) {
		table[key_hash].initialized = false;
	}


	Node *prev = &table[key_hash];
	while (table[key_hash].has_next) {
		Node *act = table[key_hash].next;
		if (act->key == in_key) {
			act->initialized = false;
			prev->has_next = false;
		}
		Node *prev = act;
	}
}

template<typename Key, typename Val>
void HashMap<Key, Val>::_destroy_fragment(unsigned int begin, unsigned int end) {

	for (size_t i = begin; i < end; i++)
	{
		if (table[i].initialized) {
			Node *current = table[i].next;
			while (current != 0) {
				Node* next = current->next;
				delete current;
				current = next;
				std::cout << "DN";
			}
		}
	}

}

template<typename Key, typename Val>
inline unsigned long int HashMap<Key, Val>::to_uint(Key key) const
{
	return static_cast<int>(key);
}

template<typename Key, typename Val>
typename HashMap<Key, Val>::Node* HashMap<Key, Val>::get_table() const {
	return this->table;
}

// HashMap operators


template<typename Key, typename Val>
HashMap<Key, Val>& HashMap<Key, Val>::operator=(const HashMap<Key, Val>& other){
	// TODO HashMap<Key, Val>::operator=
	for (size_t i = 0; i < MAX_LENGTH; i++)
	{
		int c = 0;
		Node *current = &other.table[i];

		// Assume the list contains at least 1 student.
		Node *copy = new Node;
		copy->initialized = current->initialized;
		copy->has_next = current->has_next;
		copy->key = current->key;
		copy->val = current->val;
		copy->next = current->next;

		// Keep track of first element of the copy.
		Node *const head = copy;

		// 1st element already copied.
		if (current->has_next)
			current = current->next;

		while (current->has_next) {
			// Allocate the next node and advance `copy` to the element being copied.
			copy = copy->next = new Node;

			// Copy the node contents; don't share references to students.
			copy->initialized = current->initialized;
			copy->has_next = current->has_next;
			copy->key = current->key;
			copy->val = current->val;

			std::cout << i << " / " << c++ << " : " << copy->initialized << ", " << copy->has_next << std::endl;

			// No next element (yet).
			copy->next = nullptr;

			// Advance 'current' to the next element
			current = current->next;
		}

		// Return pointer to first (not last) element.
		this->table[i] = *head;

	}
	std::cin.get();
	return *this;
}

template<typename Key, typename Val>
bool HashMap<Key, Val>::operator==(const HashMap<Key, Val>&) const {
	// TODO HashMap<Key, Val>::operator==
	return false;
}

template<typename Key, typename Val>
bool HashMap<Key, Val>::operator!=(const HashMap<Key, Val>& other) const{
	// TODO HashMap<Key, Val>::operator!=
	return !(*this==other)
}

// HashMap iterator functions

template<typename Key, typename Val>
typename HashMap<Key, Val>::iterator HashMap<Key, Val>::begin() { 
	// TODO HashMap<Key, Val>::begin()
	return *_begin;
}

template<typename Key, typename Val>
typename HashMap<Key, Val>::const_iterator HashMap<Key, Val>::begin() const { 
	// TODO HashMap<Key, Val>::begin()
	return _begin;
}

template<typename Key, typename Val>
typename HashMap<Key, Val>::const_iterator HashMap<Key, Val>::cbegin() const { 
	// TODO HashMap<Key, Val>::cbegin()
	return const_iterator(_begin);
}

template<typename Key, typename Val>
typename HashMap<Key, Val>::iterator HashMap<Key, Val>::end(){ 
	// TODO HashMap<Key, Val>::end()
	return *_end;
}

template<typename Key, typename Val>
typename HashMap<Key, Val>::const_iterator HashMap<Key, Val>::end() const { 
	// TODO HashMap<Key, Val>::end()
	return const_iterator(_end);
}

template<typename Key, typename Val>
typename HashMap<Key, Val>::const_iterator HashMap<Key, Val>::cend() const { 
	// TODO HashMap<Key, Val>::cend()
	return const_iterator(_end);
}

template<typename Key, typename Val>
HashMap<Key, Val>::iterator::iterator(){
	// TODO HashMap<Key, Val>::iterator::iterator()
}

template<typename Key, typename Val>
HashMap<Key, Val>::iterator::iterator(Node &_pointed) {
	// TODO HashMap<Key, Val>::iterator::iterator()
	// passes the pointed address
	this->pointed = &_pointed;
}

template<typename Key, typename Val>
HashMap<Key, Val>::iterator::iterator(const iterator& other) {
	// TODO HashMap<Key, Val>::iterator::iterator(const iterator&)
	this->pointed = other.pointed;
}

template<typename Key, typename Val>
HashMap<Key, Val>::iterator::~iterator() {
	// TODO HashMap<Key, Val>::iterator::~iterator()
}

template<typename Key, typename Val>
typename HashMap<Key, Val>::iterator& HashMap<Key, Val>::iterator::operator=(const iterator& other) {
	// TODO HashMap<Key, Val>::iterator::operator=(const iterator&)
	this->pointed = other->pointed;
}

template<typename Key, typename Val>
bool HashMap<Key, Val>::iterator::operator==(const iterator& other) const {
	// TODO HashMap<Key, Val>::iterator::operator==(const iterator&)
	// compares the pointed address, not the key-value pair of the pointed Node
	return this->pointed == other->pointed;
}

template<typename Key, typename Val>
bool HashMap<Key, Val>::iterator::operator!=(const iterator& other) const {
	// TODO HashMap<Key, Val>::iterator::operator!=(const iterator&)
	// compares the pointed address, not the key-value pair of the pointed Node
	return (this->pointed 
		!= other.pointed);
}

template<typename Key, typename Val>
typename HashMap<Key, Val>::iterator& HashMap<Key, Val>::iterator::operator++() {
	// TODO HashMap<Key, Val>::iterator::operator++()
	this->pointed = this->pointed->next;
	return *this;
}

template<typename Key, typename Val>
typename HashMap<Key, Val>::reference HashMap<Key, Val>::iterator::operator*() const {
	// TODO HashMap<Key, Val>::iterator::operator*()
	return *pointed;
}

template<typename Key, typename Val>
typename HashMap<Key, Val>::iterator::pointer HashMap<Key, Val>::iterator::operator->() const {
	// TODO HashMap<Key, Val>::iterator::operator->()
	return &(*pointed);
}