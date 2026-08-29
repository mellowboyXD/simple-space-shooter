#include "utils.h"

// Returns the position of the hitbox relative to position and the camera.
Vector2 GetHitboxPos(const Position *const pos, const Render *const r,
		     const Hitbox *const hb, const Vector2 gameCameraOffset)
{
	float px = pos->x + gameCameraOffset.x;
	float py = pos->y + gameCameraOffset.y;
	if (r->renderMode == RENDER_COLOR)
		return (Vector2){ px, py };

	Rectangle frame = r->frame;
	float cx = frame.width / 2.0f;
	float cy = frame.height / 2.0f;

	float x = px + (cx - hb->width / 2.0f);
	float y = py + (cy - hb->height / 2.0f);

	return (Vector2){ x, y };
}
