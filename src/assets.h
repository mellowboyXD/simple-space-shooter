#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"

#include <stdint.h>

typedef uint32_t AssetId;

void AssetsInit();
void AssetsDeinit();
AssetId AssetsLoadTexture(const char *filename);
Texture2D *AssetsGetTexture(AssetId id);

#endif /* ASSETS_H */
