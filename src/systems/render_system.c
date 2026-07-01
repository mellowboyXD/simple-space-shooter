#include "render_system.h"
#include "coordinator.h"
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

		if (r->renderMode == RENDER_COLOR) {
			DrawRectangle(p->pos.x, p->pos.y, h->width, h->height,
				      r->renderColor);
		}
	}
}
