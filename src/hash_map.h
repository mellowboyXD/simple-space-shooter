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
#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stddef.h>
#include <stdint.h>

typedef struct HashMap HashMap;
typedef struct HashMapIterator HashMapIterator;

struct HashMapEntry {
	const char *key; // key is set to "__EMPTY___" if empty
	void *value;
};

struct HashMap {
	struct HashMapEntry *entries;
	size_t size; // number of entries in map
	size_t capacity; // max number of entries
};

struct HashMapIterator {
	const char *key;
	void *value;

	HashMap *_map;
	size_t _index; // index of next entry
};


// returns a hash map structure
HashMap *HashMapCreate();

// free all resources
// note: does not free value. It is the responsibility of caller to free its value
void HashMapDestroy(HashMap *map);

// returns the value associated with a key, else NULL
void *HashMapGet(HashMap *map, const char *key);

// returns the value associated with a key or errorValue if not found
void *HashMapGetOrError(HashMap *map, const char *key, void *errorValue);

// adds a key-value pair to the map
bool HashMapSet(HashMap *map, const char *key, void *value);

// returns true if key is found, false otherwise
bool HashMapHas(HashMap *map, const char *key);

// removes a key from the map by setting it key to NULL
// delete will just mark the key of an entry as "EMPTY"
bool HashMapDel(HashMap *map, const char *key);

HashMapIterator HashMapIteratorCreate(HashMap *map);

bool HashMapIteratorNext(HashMapIterator *iterator);

#endif // HASH_MAP_H
