#include "assets.h"
#include "debug.h"
#include "utils.h"

#include <assert.h>
#include <string.h>

static constexpr size_t MAX_TEXTURES = 256;
static Texture2D _texturesPool[MAX_TEXTURES];
static size_t _texturesCount = 0;

static bool initCalled = false;

void AssetsInit()
{
        assert(!initCalled && "Asset pool already initialized.");
        _texturesCount = 0;

        // zero-initialize the pool
        memset(&_texturesPool, 0, sizeof(_texturesPool));
        LOG(L_INFO, "Asset was initialized successfully.");
        initCalled = true;
}

void AssetsDeinit()
{
        ASSERT_STATIC_INITIALIZED;
        for (AssetId i = 0; i < _texturesCount; i++) {
                UnloadTexture(_texturesPool[i]);
        }
        _texturesCount = 0;
        LOG(L_INFO, "Assets were deinitialized successfully.");
}

AssetId AssetsLoadTexture(const char *filename)
{
        ASSERT_STATIC_INITIALIZED;
        assert(_texturesCount < MAX_TEXTURES && "Max textures reached.");
        AssetId id = (AssetId)_texturesCount++;
        _texturesPool[id] = LoadTexture(filename);
        return id;
}

Texture2D *AssetsGetTexture(AssetId id)
{
        ASSERT_STATIC_INITIALIZED;
        assert(id < MAX_TEXTURES && "Invalid texture.");

        return _texturesPool + id;
}
