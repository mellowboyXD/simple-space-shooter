#include "render_system.h"
#include "assets.h"
#include "components.h"
#include "coordinator.h"
#include "entity_manager.h"
#include "raylib.h"

#include "utils.h"
#include <assert.h>

typedef struct {
	Entity entities[MAX_ENTITIES];
	size_t count;
} Layer;

extern const Vector2 gameCameraOffset; // declared by game.c

static constexpr size_t MAX_LAYERS = 50;

static Layer layers[MAX_LAYERS];

static void _ResetLayers()
{
	for (size_t i = 0; i < MAX_LAYERS; i++) {
		layers[i].count = 0;
	}
}

static void _RenderInColorMode(const Position *const p, const Hitbox *const h,
			       const Color color)
{
	// apply game offset
	DrawRectangle(p->x + gameCameraOffset.x, p->y + gameCameraOffset.y,
		      h->width, h->height, color);
}

static void _RenderInSpriteMode(const Position *const p,
				const AssetId textureId, const Rectangle frame)
{
	Texture2D texture = AssetsGetTexture2D(textureId);

	Vector2 screenPos = { p->x + gameCameraOffset.x,
			      p->y + gameCameraOffset.y };
	DrawTextureRec(texture, frame, screenPos, WHITE);
#ifdef DEBUG
	DrawRectangleLines(screenPos.x, screenPos.y, frame.width, frame.height,
			   RED);
#endif
}

static void _RenderHitbox(const Position *const p, const Hitbox *const h,
			  const Render *const r)
{
	Vector2 pos = GetHitboxPos(p, r, h, gameCameraOffset);

	DrawRectangleLines(pos.x, pos.y, h->width, h->height, RED);
}

static void _RenderWeapon(const Position *const p, const Hitbox *const h,
			  const Render *const r, const WeaponModifier *const w)
{
	if (r->renderMode == RENDER_COLOR) {
		Hitbox hb = { w->sprite.frame.width, 5 };
		Position pos = { p->x - hb.width / 2 + h->width / 2,
				 p->y + h->height / 2 };
		_RenderInColorMode(&pos, &hb, GRAY);
		return;
	}

	Rectangle frame = w->sprite.frame;
	_RenderInSpriteMode(p, w->sprite.textureId, frame);
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
	_ResetLayers();

	// gather entities and place them at the appropriate layer
	for (size_t i = 0; i < self->count; i++) {
		Entity entity = self->entities[i];

		Render *r = GET_COMPONENT(Render, entity, COMPONENT_RENDER);

		assert(r->zIndex < MAX_LAYERS && "Invalid zIndex");
		if (r->zIndex >= MAX_LAYERS) {
			continue;
		}

		size_t countAtLayer = layers[r->zIndex].count;
		assert(countAtLayer < MAX_ENTITIES && "Max entities at layer");
		if (countAtLayer >= MAX_ENTITIES) {
			continue;
		}

		layers[r->zIndex].entities[countAtLayer] = entity;
		layers[r->zIndex].count++;
	}

	// render by layer
	for (size_t layer = 0; layer < MAX_LAYERS; layer++) {
		size_t count = layers[layer].count;
		if (count < 1)
			continue;

		for (size_t i = 0; i < count; i++) {
			Entity entity = layers[layer].entities[i];

			Position *p = GET_COMPONENT(Position, entity,
						    COMPONENT_POSITION);
			Hitbox *h =
				GET_COMPONENT(Hitbox, entity, COMPONENT_HITBOX);
			Render *r =
				GET_COMPONENT(Render, entity, COMPONENT_RENDER);

			if (CoordinatorIsPlayer(entity)) {
				WeaponModifier *w = GET_COMPONENT(
					WeaponModifier, entity,
					COMPONENT_WEAPON_MODIFIER);
				_RenderWeapon(p, h, r, w);
			}

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
}
