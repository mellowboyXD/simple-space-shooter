/**
 * A simple hash table implementation.
 * Heavily inspired by this: https://github.com/tsoding/ht.h/blob/main/ht.h
 * Note: makes use of `typeof` operator which was introduced in c23.
 *
 * Simple operations include:
 * - Find
 * - Insert
 * - Delete
 */
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/**
 * Idea borrowed from Tsoding's hash table: https://github.com/tsoding/ht.h/blob/main/ht.h
 */

#include <stdint.h>

#define HashTable(Tk, Tv)         \
	struct {                  \
		Tk *_keyPtr;      \
		size_t _size;     \
		size_t _capacity; \
		Tv _defaultValue; \
	}

#define _HashTableWordAlign(size)                               \
	(((size) + sizeof(uintptr_t) - 1) / sizeof(uintptr_t) * \
	 sizeof(uintptr_t))

#define _HashTableLayout(ht)                                                   \
	((struct _Layout){                                                     \
		._slotSize =                                                   \
			_HashTableWordAlign(sizeof((ht)->_defaultValue)) +     \
			_HashTableWordAlign(sizeof(*(ht)->_keyPtr)) +          \
			sizeof(uintptr_t),                                     \
		._keySize = sizeof(*(ht)->_keyPtr),                            \
		._valueSize = sizeof((ht)->_defaultValue),                     \
		._keyOffset = _HashTableWordAlign(sizeof((ht)->defaultValue)), \
		._hashOffset =                                                 \
			_HashTableWordAlign(sizeof((ht)->defaultValue)) +      \
			_HashTableWordAlign(sizeof(*(ht)->_keyPtr)) })

struct _HashTableLayout {
	size_t _slotSize;
	size_t _keySize;
	size_t _valueSize;
	size_t _keyOffset;
	size_t _hashOffset;
};

struct _AbstractHashTable {
	void *_slots; // stores value, key and hash contiguously
	size_t _size;
	size_t _capacity;
	uint8_t defaultValue; // placeholder type
};

// returns the value associated with a key if found, NULL otherwise
#define HashTableFind(ht, key)                         \
	(typeof((ht)->_defaultValue) *)_HashTableFind( \
		(_AbstractHashTable *)(ht),            \
		(typeof (*(ht)->_keyPtr)[]){ key }, _HashTableLayout(ht))
void *_HashTableFind(struct _AbstractHashTable ht, void *key,
		     struct _HashTableLayout l);

// inserts a new key-value pair into the table. returns true on success, false 
// otherwise. Updates existing key if it exists
#define HashTableInsert(ht, key, value)                                        \
	(typeof((ht)->defaultValue) *)_HashTableInsert(                        \
		(_AbstractHashTable *)(ht), (typeof (*(ht)->keyPtr)[]){ key }, \
		(typeof((ht)->defaultValue)[]){ value }, _HashTableLayout(ht))
bool _HashTableInsert(struct _AbstractHashTable ht, void *key, void *value,
		      struct _HashTableLayout l);

// removes an entry from the hash table. returns true on success, false otherwise
#define HashTableDelete(ht, key)                                               \
	(typeof((ht)->defaultValue) *)_HashTableDelete(                        \
		(_AbstractHashTable *)(ht), (typeof (*(ht)->keyPtr)[]){ key }, \
		_HashTableLayout(ht))
bool _HashTableDelete(struct _AbstractHashTable ht, void *key,
		      struct _HashTableLayout l);

#endif // HASH_TABLE_H
