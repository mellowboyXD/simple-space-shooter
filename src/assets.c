/**
 * Implementation:
 *      Have a hash table that stores filename (as key) and AssetId (as value).
 *      When the user requests to load an asset, we find the key (filename) in 
 *      the hash table. If its there, we use the AssetId as an index into 
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
#include "hash_map.h"
#include "utils.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct _AssetTexture {
	char *filename; // the asset filename, used for fast ht lookups
	Texture2D texture;
	size_t refCount; // reference count
};

static struct HashMap *map = NULL;

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
	if (!HashMapDel(map, (const char *)key)) {
		LOG(L_ERROR, "Failed to remove %s from the hash map", key);
		free(key);
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

	LOG(L_INFO, "Asset '%s' was completely trashed.", key);

	// free strdup'ed filename (see AssetsLoadTexture)
	free(key);
}

static AssetId _GetNewAssetId()
{
        ASSERT_STATIC_INITIALIZED;

        for (AssetId i = 0; i < MAX_TEXTURES; i++) {
                if (_assetIdToIndexMap[i] == INVALID_ID_OR_INDEX)
                        return i;
        }

        LOG(L_ERROR, "Could not find a valid asset id. Max textures reached.");
        return INVALID_ID_OR_INDEX;
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

	// initialize the hash table
	map = HashMapCreate();

	LOG(L_INFO, "Asset was initialized successfully.");
	initCalled = true;
}

/**
 * Frees up the VRAM by unloading all the assets.
 */
void AssetsDeinit()
{
	ASSERT_STATIC_INITIALIZED;
	for (AssetId id = 0; id < MAX_TEXTURES; id++) {
		while (_assetIdToIndexMap[id] != INVALID_ID_OR_INDEX &&
		       _texturesPool[_assetIdToIndexMap[id]].refCount > 0) {

			AssetUnloadTexture(id);
		}
	}
	_texturesCount = 0;
	HashMapDestroy(map);
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

	LOG(L_INFO, "Checking if texture is in cache");
	id = (AssetId)((uintptr_t)HashMapGetOrError(
		map, filename, (void *)((uintptr_t)INVALID_ID_OR_INDEX)));
	if (id != INVALID_ID_OR_INDEX) {
		index = _assetIdToIndexMap[id];
		_texturesPool[index].refCount++;
		return id;
	}

	LOG(L_INFO, "Texture not in cache. Loading manually.");
	// it does not exist, so load it into the pool
	assert(_texturesCount < MAX_TEXTURES && "Max textures reached.");
	index = _texturesCount++;
	id = _GetNewAssetId();

	assert(id < MAX_TEXTURES && "Invalid Asset Id. Out of range.");

	_texturesPool[index].texture = LoadTexture(filename);
	_texturesPool[index].refCount = 1;
	_texturesPool[index].filename = strdup(filename); // need to free

	// update the maps
	_assetIdToIndexMap[id] = index;
	_indexToAssetIdMap[index] = id;

	// add the entry to the hash table
	if (!HashMapSet(map, filename, (void *)((uintptr_t)id))) {
		LOG(L_ERROR, "Could not add id to hash table.");
		exit(EXIT_FAILURE);
	}

	return id;
}

/**
 * Unloads a texture using the asset id.
 */
void AssetUnloadTexture(AssetId id)
{
	ASSERT_STATIC_INITIALIZED;

	assert(id < MAX_TEXTURES && "Invalid asset id.");

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

void AssetLogInfo()
{
	ASSERT_STATIC_INITIALIZED;
	LOG(L_INFO, "Asset pool count: %zu", _texturesCount);
}

void AssetLogRefCount(AssetId id)
{
	ASSERT_STATIC_INITIALIZED;
	assert(id < MAX_TEXTURES && "Invalid asset id. Out of range.");

	size_t index = _assetIdToIndexMap[id];
	if (index == INVALID_ID_OR_INDEX) {
		LOG(L_ERROR, "Requested asset id (%zu) not found.", id);
		return;
	}

	struct _AssetTexture texture = _texturesPool[index];

	LOG(L_INFO, "Ref count of asset('%s') with id %zu:- %zu",
	    texture.filename, id, texture.refCount);
}
