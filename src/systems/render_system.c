#include "render_system.h"
#include "assets.h"
#include "coordinator.h"
#include "raylib.h"

#include <assert.h>

extern const Vector2 gameCameraOffset; // declared by game.c

static void _RenderInColorMode(Position *p, Hitbox *h, Color color)
{
	// apply game offset
	DrawRectangle(p->x + gameCameraOffset.x, p->y + gameCameraOffset.y,
		      h->width, h->height, color);
}

static void _RenderInSpriteMode(Position *p, AssetId textureId, Rectangle frame)
{
	Texture2D texture = AssetsGetTexture2D(textureId);

	Vector2 screenPos = { p->x + gameCameraOffset.x,
			      p->y + gameCameraOffset.y };
	DrawTextureRec(texture, frame, screenPos, WHITE);
}

static void _RenderHitbox(Position *p, Hitbox *h, Rectangle frame)
{
	float px = p->x + gameCameraOffset.x;
	float py = p->y + gameCameraOffset.y;

	float cx = frame.width / 2;
	float cy = frame.height / 2;

	float x = px + (cx - h->width / 2);
	float y = py + (cy - h->height / 2);

	DrawRectangleLines(x, y, h->width, h->height, RED);
}

RenderSystem *RenderSystemCreate()
{
	RenderSystem *renderSystem = CoordinatorRegisterSystem(
		RENDER_SYSTEM_TYPE, RenderSystemUpdate);
	Signature signature = COMPONENT_BIT(COMPONENT_RENDER) |
			      COMPONENT_BIT(COMPONENT_POSITION) |
			      COMPONENT_BIT(COMPONENT_HITBOX);

	CoordinatorSetSystemSignature(RENDER_SYSTEM_TYPE, signature);

	return renderSystem;
}

void RenderSystemUpdate(RenderSystem *self, [[maybe_unused]] float dt)
{
	for (size_t i = 0; i < self->count; i++) {
		Entity entity = self->entities[i];

		Position *p =
			GET_COMPONENT(Position, entity, COMPONENT_POSITION);
		Hitbox *h = GET_COMPONENT(Hitbox, entity, COMPONENT_HITBOX);
		Render *r = GET_COMPONENT(Render, entity, COMPONENT_RENDER);

		if (r->renderMode == RENDER_COLOR) {
			_RenderInColorMode(p, h, r->renderColor);
		} else if (r->renderMode == RENDER_SPRITE) {
			AssetId textureId = r->textureId;
			Rectangle frame = r->frame;
			_RenderInSpriteMode(p, textureId, frame);
#ifdef DEBUG // draw hitbox in debug mode
			_RenderHitbox(p, h, frame);
#endif // DEBUG
		}
	}
}
