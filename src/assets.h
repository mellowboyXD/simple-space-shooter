#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"

#include <stdint.h>

typedef uint32_t AssetId;

void AssetsInit();
void AssetsDeinit();
AssetId AssetLoadTexture(const char *filename);
Texture2D *AssetGetTexture(AssetId id);

#endif /* ASSETS_H */
