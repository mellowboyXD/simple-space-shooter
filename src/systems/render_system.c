#include "render_system.h"
#include "assets.h"
#include "coordinator.h"
#include "raylib.h"

#include "utils.h"
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

static void _RenderHitbox(const Position *const p, const Hitbox *const h,
			  const Render *const r)
{
	Vector2 pos = GetHitboxPos(p, r, h, gameCameraOffset);

	DrawRectangleLines(pos.x, pos.y, h->width, h->height, RED);
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
			_RenderHitbox(p, h, r);
#endif // DEBUG
		}
	}
}
