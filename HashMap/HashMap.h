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
		bool initialized = false;		/// does it have a valid stored value
		bool has_next = false;			/// does it have an initialized next node
		Key		key;					/// Hash key
		Val		val;					/// stored value
		Node*	next;	/// next node in the bucket

		Node();
		Node(Key&, Val&);
		~Node();
	};

	//private fields
	const size_t MAX_LENGTH = 2000;
	Node* table = new Node[MAX_LENGTH];

	// private methodss
	/// Hash functions
	unsigned long int hash(Key) const;
	unsigned int _count(unsigned int begin, unsigned int end) const;
	void _destroy_fragment(unsigned int begin, unsigned int end);
	unsigned long int to_uint(Key) const;
	
public:
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

	HashMap();
	~HashMap();
};



template<typename Key, typename Val>
HashMap<Key, Val>::Node::Node()
{
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


template<typename Key, typename Val>
inline unsigned long int HashMap<Key, Val>::hash(const Key key) const
{
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
HashMap<Key, Val>::HashMap()
{
	for (auto i = 0; i < MAX_LENGTH; ++i)
		table[i] = Node();
}


template<typename Key, typename Val>
HashMap<Key, Val>::~HashMap()
{
	unsigned int nthreads = std::thread::hardware_concurrency();
	std::vector<std::thread> counters;

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
void HashMap<Key, Val>::_destroy_fragment(unsigned int begin, unsigned int end) {

	for (size_t i = begin; i < end; i++)
	{
		if (table[i].initialized) {
			Node *current = table[i].next;
			int c = 0;

			while (current != 0) {
				Node* next = current->next;
				delete current;
				current = next;
				std::cout << "DN";
			}

			//while (act->has_next) {
			//	// std::cout << act->val << "->" << act->next->val << " ";
			//	if (c % 2 == 0 && c != 0) {
			//		to_del = act;
			//		if (to_del != nullptr) {
			//			delete to_del->next;
			//		}
			//	}
			//		
			//	act = act->next;
			//	c++;
			//}
		}
	}

}

template<typename Key, typename Val>
inline unsigned long int HashMap<Key, Val>::to_uint(Key key) const
{
	// std::cout << static_cast<int>(key)<<std::endl;
	return static_cast<int>(key);
}
