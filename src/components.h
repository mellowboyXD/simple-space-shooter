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
#include <stdint.h>

#define COMPONENT_BIT(type) (1u << (type))
#define ASSERT_COMPONENT_TYPE(type)                                   \
	(assert((type) > COMPONENT_NONE && (type) < MAX_COMPONENTS && \
		"Invalid component type"))

typedef enum {
	COMPONENT_NONE,
	COMPONENT_POSITION,
	COMPONENT_VELOCITY,
	COMPONENT_HITBOX,
	COMPONENT_RENDER,
	MAX_COMPONENTS
} ComponentType;

typedef struct {
	Vector2 pos;
} Position;

typedef struct {
	Vector2 vel;
} Velocity;

typedef struct {
	float width;
	float height;
} Hitbox;

typedef enum { RENDER_COLOR, RENDER_SPRITE } RenderMode;

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
	RenderMode renderMode;
	Color renderColor; // used this in mode RENDER_COLOR
	int textureId; // use this in mode RENDER_SPRITE
	Rectangle frame; // use this in mode RENDER_SPRITE
} Render;

#endif /* COMPONENTS_H */
