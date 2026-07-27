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

static void _RenderInSpriteMode(Position *p, Hitbox *h, AssetId textureId,
				Rectangle frame)
{
	Texture2D *texture = AssetsGetTexture(textureId);
	assert(texture && "Could not retrieve the texture.");

	Vector2 screenPos = { p->x + gameCameraOffset.x,
			      p->y + gameCameraOffset.y };
	DrawTextureEx(*texture, screenPos, 0.0f, 1.0f, WHITE);
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
			;
			_RenderInSpriteMode(p, h, textureId, frame);
		}
	}
}
