#include "hash_map.h"
#include "debug.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	const char *key; // key is set to null if empty
	void *value;
} HashMapEntry;

struct HashMap {
	HashMapEntry *entries;
	size_t size; // number of entries in map
	size_t capacity; // max number of entries
};

static constexpr size_t INIT_CAPACITY = 16; // must be power of 2
static constexpr uint64_t FNV_OFFSET = 14695981039346656037u;
static constexpr uint64_t FNV_PRIME = 1099511628211u;
static const char *HASH_ENTRY_EMPTY = "__EMPTY___";

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

static size_t _SlotIndex(uintptr_t hash, size_t size)
{
	assert((size & (size - 1)) == 0 && "Size must be a power of 2");
	return (size_t)(hash & (uint64_t)(size - 1));
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

	map->entries = calloc(map->capacity, sizeof(HashMapEntry));
	if (!map->entries) {
		free(map);
		LOG(L_ERROR, "Could not calloc hash map entries");
		return NULL;
	}

	return map;
}

void HashMapDestroy(HashMap *map)
{
	for (size_t i = 0; i < map->capacity; i++) {
		free((void *)map->entries[i].key);
	}

	free(map->entries);
	free(map);
}

void *HashMapGet(HashMap *map, const char *key)
{
	size_t n = strlen(key);
	assert(n > 0 && "Invalid key");
	uintptr_t hash = _FNV1Hash(key, n);

	size_t index = _SlotIndex(hash, map->capacity);
	while (map->entries[index].key != NULL) {
		if (strncmp(map->entries[index].key, key, n) == 0)
			return map->entries[index].value;

		// linear probe to next slot
		index = (index + 1) % map->capacity;
	}

	return NULL;
}
