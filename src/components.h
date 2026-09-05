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
 *
 * Tags:
 *      A tag is just component that makes it easy to look up a specific
 *      entity.
 */
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "assets.h"
#include "raylib.h"
#include <stdint.h>

#define COMPONENT_BIT(type) (1u << (type))
#define ASSERT_COMPONENT_TYPE(type)                                   \
	(assert((type) > COMPONENT_NONE && (type) < MAX_COMPONENTS && \
		"Invalid component type"))

// convention/standard to follow: tags are placed after TAG_PLAYER which is
// always the very first tag the system knows about. A tag is basically a
// component.
typedef enum {
	COMPONENT_NONE,
	COMPONENT_POSITION,
	COMPONENT_VELOCITY,
	COMPONENT_PLAYER_MODIFIERS,
	COMPONENT_WEAPON_MODIFIER,
	COMPONENT_HITBOX,
	COMPONENT_RENDER,
	COMPONENT_UI_MOUSE_INPUT_STATE,
	COMPONENT_UI_CALLBACK,
	COMPONENT_PLAYER_INPUT,
	TAG_PLAYER,
	TAG_BULLET,
	TAG_FOR_CLEANUP,
	MAX_COMPONENTS
} ComponentType;

#define POSITION(x, y) ((Position){ (x), (y) })
typedef Vector2 Position;

#define VELOCITY(dx, dy) ((Velocity){ (dx), (dy) })
typedef struct {
	float dx;
	float dy;
} Velocity;

#define PLAYER_MODIFIERS(s) ((PlayerModifiers){ (s) })
typedef struct {
	float speed;
} PlayerModifiers;

#define HITBOX(width, height) ((Hitbox){ (width), (height) })
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
#define RENDER_C(color) ((Render){ RENDER_COLOR, (color), -1, { 0, 0, 0, 0 } })
#define RENDER_S(textureId, frame) \
	((Render){ RENDER_SPRITE, WHITE, (textureId), (frame) })
typedef struct {
	RenderMode renderMode;
	Color renderColor; // used this in mode RENDER_COLOR
	AssetId textureId; // use this in mode RENDER_SPRITE
	Rectangle frame; // use this in mode RENDER_SPRITE
} Render;

typedef enum {
	AUTO_CANNON_WEAPON,
	SPACE_GUN_WEAPON,
	ZAPPER_WEAPON,
	ROCKET_WEAPON
} WeaponType;

#define WEAPON(f, t, bs) ((WeaponModifier){ 0, (f), (t), (bs) })
typedef struct {
	float _cooldown; // time before next fire. Used internally to dictate when next to shoot
	float fireRate;
	WeaponType type;
        float bulletSpeed;
} WeaponModifier;

#define TAG_OBJ ((Tag){ 0 })
typedef struct {
	char __dummy;
} Tag;

#define PLAYER_INPUT_INIT ((PlayerInput){ 0 })
typedef struct {
	bool left;
	bool right;
	bool up;
	bool down;
	bool fire;
} PlayerInput;

#endif /* COMPONENTS_H */
