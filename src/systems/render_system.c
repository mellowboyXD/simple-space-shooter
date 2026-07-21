#include "render_system.h"
#include "constants.h"
#include "coordinator.h"
#include "game.h"
#include "raylib.h"

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
		Vector2 offset = GameCameraOffset();

		if (r->renderMode == RENDER_COLOR) {
			// apply game offset
			DrawRectangle(p->x + offset.x, p->y + offset.y,
				      h->width, h->height, r->renderColor);
		}
	}
}
