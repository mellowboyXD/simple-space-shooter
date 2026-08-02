/**
 * Implementation:
 *      Have a hash table that stores filename (as key) and AssetId (as value).
 *      When the user requests to load an asset, we find the key (filename) in 
 *      the hash table. If its there, we use to AssetId as an index into 
 *      texture pool, increment the refCount and return the AssetId.
 *      Otherswise, we add the texture to the back of the pool, set the
 *      refCount to 0 and return the index as AssetId.
 *
 *      When entities are destroyed, the asset associated with it should die.
 *      Decrement it refCount until it hits zero. In that case we remove its 
 *      entry in the hash table.
 *      To maintain a packed array, we have 2 parrallel arrays that act like 
 *      maps. Need to update the maps during loading and unloading of an asset.
 *      As such, we move the last element of the pool to the index of the 
 *      removed asset and update the maps accordingly.
 */
#include "assets.h"
#include "debug.h"
#include "utils.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define strdup _strdup

// TODO: Replace with actuall hash table implementation
struct HashTable {
	char *key;
	AssetId value;
};

/**
 * Finds the value associated with key in ht.
 * Returns errorValue if key is not found in ht.
 */
AssetId HashTableGetOrError(struct HashTable ht, const char *key,
			    AssetId errorValue)
{
        RAISE_UNIMPLEMENTED;
}
/**
 * Adds a new key-value pair to the hash table
 */
void HashTableAdd(struct HashTable ht, const char *key, AssetId value)
{
        RAISE_UNIMPLEMENTED;
}

/**
 * Removes a key from the hash table.
 * Returns true on success, false otherwise
 */
bool HashTableDelete(struct HashTable ht, const char *key)
{
        RAISE_UNIMPLEMENTED;
}

struct _AssetTexture {
	char *filename; // the asset filename, used for fast ht lookups
	Texture2D texture;
	size_t refCount; // reference count
};

// max allowed assets at all times
static constexpr size_t MAX_TEXTURES = 256;

// sentinel value to designate an invalid index/AssetId (basically same thing)
static constexpr AssetId INVALID_ID_OR_INDEX = MAX_TEXTURES + 1;

static struct HashTable ht = { 0 };

static struct _AssetTexture _texturesPool[MAX_TEXTURES];
static size_t _texturesCount = 0;
static size_t _assetIdToIndexMap[MAX_TEXTURES];
static AssetId _indexToAssetIdMap[MAX_TEXTURES];

static bool initCalled = false;

/**
 * Helper function to remove a texture from the texture pool and reordering
 * the pool to maintain packed array.
 */
static void _TrashAssetTexture(AssetId id)
{
	ASSERT_STATIC_INITIALIZED;
	assert(id < MAX_TEXTURES && "Invalid AssetId. Out of range.");

	size_t index = _assetIdToIndexMap[id];
	assert(index != INVALID_ID_OR_INDEX && "Invalid index from AssetId.");

	// key(filename) is freed later
	char *key = _texturesPool[index].filename;
	if (!HashTableDelete(ht, (const char *)key)) {
		LOG(L_ERROR, "Failed to remove %s from the hash table", key);
		exit(EXIT_FAILURE);
	}

	UnloadTexture(_texturesPool[index].texture);

	size_t indexOfLast = --_texturesCount;
	AssetId idOfLast = _indexToAssetIdMap[indexOfLast];

	if (idOfLast != id) {
		// move last texture at this index
		_texturesPool[index] = _texturesPool[indexOfLast];

		// update maps
		_indexToAssetIdMap[index] = idOfLast;
		_assetIdToIndexMap[idOfLast] = index;
	}

	// invalidate old ids & indexOfLast
	_assetIdToIndexMap[id] = INVALID_ID_OR_INDEX;
	_indexToAssetIdMap[indexOfLast] = INVALID_ID_OR_INDEX;
	_texturesPool[indexOfLast] = (struct _AssetTexture){ 0 };

	// free strdup'ed filename (see AssetsLoadTexture)
	free(key);
}

void AssetsInit()
{
	assert(!initCalled && "Asset pool already initialized.");
	_texturesCount = 0;

	// zero-initialize the pool
	memset(&_texturesPool, 0, sizeof(_texturesPool));

	// intialize the maps
	for (AssetId i = 0; i < MAX_TEXTURES; i++) {
		_assetIdToIndexMap[i] = INVALID_ID_OR_INDEX;
		_indexToAssetIdMap[i] = INVALID_ID_OR_INDEX;
	}
	LOG(L_INFO, "Asset was initialized successfully.");
	initCalled = true;
}

/**
 * Frees up the VRAM by unloading all the assets.
 */
void AssetsDeinit()
{
	ASSERT_STATIC_INITIALIZED;
	for (AssetId i = 0; i < _texturesCount; i++) {
		AssetUnloadTexture(i);
	}
	_texturesCount = 0;
	LOG(L_INFO, "Assets were deinitialized successfully.");
}

/**
 * Loads a texture into the texture pool and returns the associated id.
 */
AssetId AssetsLoadTexture(const char *filename)
{
	ASSERT_STATIC_INITIALIZED;

	AssetId id;
	size_t index;

	// TODO: Hash table lookup
	id = HashTableGetOrError(ht, filename, INVALID_ID_OR_INDEX);
	if (id != INVALID_ID_OR_INDEX) {
		size_t index = _assetIdToIndexMap[id];
		_texturesPool[index].refCount++;
		return id;
	}

	// it does not exist, so load it into the pool
	assert(_texturesCount < MAX_TEXTURES && "Max textures reached.");
	index = _texturesCount++;
	_texturesPool[index].texture = LoadTexture(filename);
	_texturesPool[index].refCount = 1;
	_texturesPool[index].filename = strdup(filename); // need to free

	// add the entry to the hash table
	HashTableAdd(ht, filename, index);

	// update the maps
	_assetIdToIndexMap[id] = index;
	_indexToAssetIdMap[index] = id;

	return id;
}

/**
 * Unloads a texture using the asset id.
 */
void AssetUnloadTexture(AssetId id)
{
	ASSERT_STATIC_INITIALIZED;

	assert(id < _texturesCount && "Invalid asset id.");

	// check if texture is present
	// if not present return
	size_t index = _assetIdToIndexMap[id];
	if (index == INVALID_ID_OR_INDEX)
		return; // not in texture pool

	// decrement refCount
	const size_t refCount = --_texturesPool[index].refCount;
	if (refCount > 0) // this asset is still in-use somewhere
		return;

	// else get rid of the asset
	_TrashAssetTexture(id);
}

Texture2D AssetsGetTexture2D(AssetId id)
{
	ASSERT_STATIC_INITIALIZED;
	assert(id < MAX_TEXTURES && "Invalid texture.");

	size_t index = _assetIdToIndexMap[id];
	assert(index != INVALID_ID_OR_INDEX &&
	       "Trying to access an invalid texture.");

	return _texturesPool[index].texture;
}
