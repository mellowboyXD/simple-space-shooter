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

// returns a hash map structure
HashMap *HashMapCreate();

// returns the value associated with a key, else NULL
void *HashMapGet(HashMap *map, const char *key);

// adds a key-value pair to the map
bool HashMapSet(HashMap *map, const char *key, void *value);

// removes a key from the map by setting it key to NULL
// delete will just mark the key of an entry as "EMPTY"
bool HashMapDelete(HashMap *map, const char *key);

// free all resources
void HashMapDestroy(HashMap *map);

#endif // HASH_MAP_H
