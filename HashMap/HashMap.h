#pragma once
#include <string>
#include <sstream>

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
	size_t size();
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
			std::cout <<act->val<< "->" << act->next->val <<" ";
			act = act->next;
		}
		act->next = new Node(key, value);
		act->has_next = true;
		// std::cout << "LINKING " << act->next->val << " TO " << act->val << " IN " << key_hash << std::endl;
	}

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
inline size_t HashMap<Key, Val>::size()
{
	return size_t();
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
}

template<typename Key, typename Val>
inline unsigned long int HashMap<Key, Val>::to_uint(Key key) const
{
	// std::cout << static_cast<int>(key)<<std::endl;
	return static_cast<int>(key);
}
