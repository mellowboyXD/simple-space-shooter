#ifndef UTILS_H
#define UTILS_H

#include "components.h"
#include "raylib.h"

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(*arr))

#define ASSERT_STATIC_INITIALIZED assert(initCalled && "Init was not called.")

#define RAISE_UNIMPLEMENTED                                 \
	LOG(L_WARN, "[TODO]: NOT IMPLEMENTED FUNCTION!\n"); \
	exit(EXIT_FAILURE)

Vector2 GetHitboxPos(const Position *const pos, const Render *const r,
		     const Hitbox *const hb, const Vector2 gameCameraOffset);

float clampf(float min, float value, float max);

#endif // UTILS_H
