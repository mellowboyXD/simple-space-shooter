/**
 * Components - part of Entity-Component-System paradigm
 * Components are **pure data containers**.
 * 
 * Things to keep in mind when adding new components:
 *
 * 1. Prefer primitive data types keeping components flat and simple
 * 2. No logic in components
 * 3. Use tag components freely
 * 4. Keep serialization in mind
 */
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "raylib.h"

typedef struct {
	float x;
	float y;
} Position;

typedef struct {
	float velX;
	float velY;
} Velocity;

typedef struct {
	float width;
	float height;
} Hitbox;

/* Texture2D's should be loaded into a cache and the id is passed between 
 * entities.
 *
 * Example:
 * ```c
 * typedef struct {
 *      Texture2D textures[MAX_TEXTURES];
 *      int size;
 * } TextureCache;
 * ```
 */
typedef struct {
	Color renderColor;  // used this in mode RENDER_COLOR
	int textureId;      // use this in mode RENDER_SPRITE
	Rectangle frame;    // use this in mode RENDER_SPRITE
} Render;

#endif /* COMPONENTS_H */
