#include "hash_map.h"
#include "debug.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static const size_t INIT_CAPACITY = 16; // must be power of 2
static const uint64_t FNV_OFFSET = 14695981039346656037u;
static const uint64_t FNV_PRIME = 1099511628211u;
static const char *HASH_ENTRY_EMPTY = "__EMPTY___";
static const float HASH_MAP_LOAD_FACTOR = 0.6f; // when 60% full, expand

// http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1
static uintptr_t _FNV1Hash(void const *data, size_t size)
{
	const uint8_t *bytes = (const uint8_t *)data;
	uint64_t hash = FNV_OFFSET;
	for (size_t i = 0; i < size; i++) {
		hash *= FNV_PRIME;
		hash ^= (uint64_t)bytes[i];
	}

	return (uintptr_t)hash;
}

static size_t _SlotIndex(uintptr_t hash, size_t capacity)
{
	assert((capacity & (capacity - 1)) == 0 && "Size must be a power of 2");
	return (size_t)(hash & (uint64_t)(capacity - 1));
}

// sets a key value pair without expanding the hash map
static bool _SetNoExpand(struct HashMapEntry *entries, size_t capacity,
			 const char *key, void *value, size_t *entriesSize)
{
	assert(entries != NULL && "Invalid argument. Entries is null");
	assert(key != NULL && "Invalid argument. Key is null");

	size_t n = strlen(key);
	uintptr_t hash = _FNV1Hash(key, n);
	size_t index = _SlotIndex(hash, capacity);
	size_t emptySlot = capacity;

	while (entries[index].key != NULL) {
		struct HashMapEntry *e = entries + index;

		if (e->key == HASH_ENTRY_EMPTY && emptySlot == capacity) {
			emptySlot = index;
		}

		if (strcmp(e->key, key) == 0) {
			entries[index].value = value;
			return true;
		}

		index = (index + 1) % capacity;
	}

	if (entriesSize != NULL) {
		key = strdup(key);
		if (!key) {
			LOG(L_ERROR, "Could not duplicate key");
			return false;
		}
		(*entriesSize)++;
	}

	if (emptySlot != capacity) {
		index = emptySlot; // reuse empty slot
	}

	entries[index].key = (char *)key;
	entries[index].value = value;
	return true;
}

static bool _ExpandMap(HashMap *map)
{
	if (map->size < HASH_MAP_LOAD_FACTOR * map->capacity) {
		// no expansion needed
		return true;
	}

	size_t newCap = map->capacity * 2;
	if (newCap < map->capacity) {
		LOG(L_ERROR, "Overflow occured.");
		return false;
	}

	struct HashMapEntry *newEntries =
		calloc(newCap, sizeof(struct HashMapEntry));
	if (!newEntries) {
		LOG(L_ERROR, "Buy more RAM");
		return false;
	}

	for (size_t i = 0; i < map->capacity; i++) {
		struct HashMapEntry entry = map->entries[i];
		if (entry.key == NULL || entry.key == HASH_ENTRY_EMPTY) {
			continue;
		}

		if (!_SetNoExpand(newEntries, newCap, entry.key, entry.value,
				  NULL)) {
			LOG(L_ERROR,
			    "An error occured while copying entries from old map to new map");
			free(newEntries);
			return false;
		}
	}

	free(map->entries);
	map->entries = newEntries;
	map->capacity = newCap;
	return true;
}

// public functions

HashMap *HashMapCreate()
{
	HashMap *map = malloc(sizeof(HashMap));
	if (!map) {
		LOG(L_ERROR, "Could not allocate space for hash map");
		return NULL; // let caller deal with error
	}
	map->capacity = INIT_CAPACITY;
	map->size = 0;

	map->entries = calloc(map->capacity, sizeof(struct HashMapEntry));
	if (!map->entries) {
		free(map);
		LOG(L_ERROR, "Could not calloc hash map entries");
		return NULL;
	}

	return map;
}

void HashMapDestroy(HashMap *map)
{
	assert(map != NULL && "Invalid argument. Hash map is null");

	for (size_t i = 0; i < map->capacity; i++) {
		if (map->entries[i].key == HASH_ENTRY_EMPTY) {
			map->entries[i].key = NULL;
		} else {
			free((void *)map->entries[i].key);
		}
	}

	free(map->entries);
	free(map);
        LOG(L_INFO, "A hash map was destroyed.");
}

bool HashMapGet(HashMap *map, const char *key, void **retval)
{
	assert(map != NULL && "Invalid argument. Hash map is null");
	assert(key != NULL && "Invalid argument. Key is null");
        assert(map->entries != NULL && "Invalid entry. Entries is null");
        assert(retval != NULL && "Return value should not be a null pointer.");

	size_t n = strlen(key);
	uintptr_t hash = _FNV1Hash(key, n);

	size_t index = _SlotIndex(hash, map->capacity);
        LOG(L_INFO, "Got index %zu for hash: %p", index, (void *)hash);
	while (map->entries[index].key != NULL) {
		if (map->entries[index].key != HASH_ENTRY_EMPTY &&
		    strcmp(map->entries[index].key, key) == 0) {
                        *retval = map->entries[index].value;
                        return true;
                }

		// linear probe to next slot
		index = (index + 1) % map->capacity;
	}

        LOG(L_INFO, "key with hash: %p is not in the map.", (void *)hash);
	return false;
}

void *HashMapGetOrError(HashMap *map, const char *key, void *errorValue)
{
        assert(map != NULL && "Invalid argument. Hash map is null.");
        assert(key != NULL && "Invalid argument. key is null.");

        void *value;
        if (!HashMapGet(map, key, &value)) {
                return errorValue; // sentinel can be null
        }

        return value;
}

bool HashMapHas(HashMap *map, const char *key)
{
	assert(map != NULL && "Invalid argument. Hash map is null");
	assert(key != NULL && "Invalid argument. Key is null");

        void *value;
	return HashMapGet(map, key, &value);
}

bool HashMapSet(HashMap *map, const char *key, void *value)
{
	assert(map != NULL && "Invalid argument. Hash map is null");
	assert(key != NULL && "Invalid argument. Key is null");

	if (!_ExpandMap(map)) { // try to expand
		LOG(L_ERROR, "Failed to expand hash map");
		return false;
	}

	if (!_SetNoExpand(map->entries, map->capacity, key, value,
			  &map->size)) {
		LOG(L_ERROR, "Could not add a new key-value pair to hash map.");
		return false;
	}

	return true;
}

bool HashMapDel(HashMap *map, const char *key)
{
	assert(map != NULL && "Invalid argument. Hash map is null");
	assert(key != NULL && "Invalid argument. Key is null");

	size_t n = strlen(key);
	uintptr_t hash = _FNV1Hash(key, n);
	size_t index = _SlotIndex(hash, map->capacity);

	while (map->entries[index].key != NULL) {
		if (map->entries[index].key != HASH_ENTRY_EMPTY &&
		    strcmp(map->entries[index].key, key) == 0) {
			free((void *)map->entries[index].key);
			map->entries[index].key = HASH_ENTRY_EMPTY;
			map->size--;
			return true;
		}

		index = (index + 1) % map->capacity;
	}

	// key not found
	return false;
}

HashMapIterator HashMapIteratorCreate(HashMap *map)
{
	assert(map != NULL && "Invalid argument. Hash map is null");
	HashMapIterator iterator = { 0 };
	iterator._map = map;
	iterator._index = 0;
	return iterator;
}

bool HashMapIteratorNext(HashMapIterator *iterator)
{
	assert(iterator != NULL && "Invalid argument. Iterator is null");
	HashMap *map = iterator->_map;
	for (; iterator->_index < map->capacity; iterator->_index++) {
		size_t index = iterator->_index;

		if (map->entries[index].key == NULL ||
		    map->entries[index].key == HASH_ENTRY_EMPTY) {
			continue;
		}

		iterator->key = map->entries[index].key;
		iterator->value = map->entries[index].value;
		iterator->_index++;
		return true;
	}

	// end of map
	iterator->key = NULL;
	iterator->value = NULL;
	return false;
}
