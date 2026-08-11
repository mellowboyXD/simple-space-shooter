#include "hash_table.h"
#include <stdint.h>

// http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1
static uintptr_t _fnv1_hash(void const *data, size_t size)
{
	const uint8_t *bytes = (const uint8_t *)data;
	uint64_t hash = 14695981039346656037u;
	for (size_t i = 0; i < size; i++) {
		hash *= 1099511628211u;
		hash ^= (uint64_t)bytes[i];
	}

	return (uintptr_t)hash;
}

static uintptr_t *_slot_hash(void const *slot, struct _HashTableLayout l)
{
	return (uintptr_t *)((uint8_t *)slot + l._hashOffset);
}

static void *_slot_key(void const *slot, struct _HashTableLayout l)
{
        return ((uint8_t *)slot + l._keyOffset);
}

void *_HashTableFind(struct _AbstractHashTable ht, void *key,
		     struct _HashTableLayout l)
{
	return NULL;
}
