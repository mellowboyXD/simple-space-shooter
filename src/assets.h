/**
 * If the refCount hits zero, I need to get rid of that asset.
 * Where should I even check the refCount?
 * When should the refCount increment and when should it decrement?
 *
 * flow: bullet creation example
 * 1. bullet entity is created
 * 2. requests to load the bullet asset
 *      chances are the asset is already sitting in texture pool
 *      how do I make the connection? the filename. 
 *      Store filename-assetId key-value pair in a hash table
 * 3. If asset is in the pool, increment its reference count & return its id.
 * 4. If asset is not in the pool, I add it to the pool & return the newly 
 *      generated id.
 *
 * flow: bullet destruction example
 * 1. bullet is out of play and needs to be destroyed
 * 2. If it had an asset that is in the pool, i decrement its refCount
 * 3. If the refCount == 0, I unload it from the pool & memory
 */
#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"

#include <stddef.h>
#include <stdint.h>

typedef size_t AssetId;

// max allowed assets at all times
static constexpr size_t MAX_TEXTURES = 256;

// sentinel value to designate an invalid index/AssetId (basically same thing)
static constexpr AssetId INVALID_ID_OR_INDEX = MAX_TEXTURES + 1;

void AssetsInit();
void AssetsDeinit();
AssetId AssetsLoadTexture(const char *filename);
Texture2D AssetsGetTexture2D(AssetId id);
void AssetUnloadTexture(AssetId id);
void AssetLogInfo(); // logs useful details about the asset pool. use in debug
void AssetLogRefCount(AssetId id);

#endif /* ASSETS_H */
