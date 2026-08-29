#include "collision_system.h"
#include "components.h"
#include "constants.h"
#include "coordinator.h"
#include "utils.h"

extern const Vector2 gameCameraOffset; // declared in game.c

// both pointer and value are immutable
bool _IsOutOfBounds(const Position *const pos, const Hitbox *const hb,
		    const Render *const r)
{
	Vector2 hbPos = GetHitboxPos(pos, r, hb, gameCameraOffset);

	hbPos.x -= gameCameraOffset.x;
	hbPos.y -= gameCameraOffset.y;

	return hbPos.x <= 0 || hbPos.x + hb->width >= GAME_VIEW_WIDTH ||
	       hbPos.y <= 0 || hbPos.y + hb->height >= GAME_VIEW_HEIGHT;
}

CollisionSystem *CollisionSystemCreate()
{
	CollisionSystem *self = CoordinatorRegisterSystem(
		COLLISION_SYSTEM_TYPE, CollisionSystemUpdate);

	Signature signature = COMPONENT_BIT(COMPONENT_POSITION) |
			      COMPONENT_BIT(COMPONENT_HITBOX) |
			      COMPONENT_BIT(COMPONENT_RENDER) |
			      COMPONENT_BIT(COMPONENT_VELOCITY);

	CoordinatorSetSystemSignature(COLLISION_SYSTEM_TYPE, signature);
	return self;
}

void CollisionSystemUpdate(CollisionSystem *self, [[maybe_unused]] float dt)
{
	for (size_t i = 0; i < self->count; i++) {
		Entity entity = self->entities[i];

		Position *pos =
			GET_COMPONENT(Position, entity, COMPONENT_POSITION);
		Hitbox *hb = GET_COMPONENT(Hitbox, entity, COMPONENT_HITBOX);
		Velocity *vel =
			GET_COMPONENT(Velocity, entity, COMPONENT_VELOCITY);
		Render *r = GET_COMPONENT(Render, entity, COMPONENT_RENDER);

		Position newPos = { pos->x + vel->dx * dt,
				    pos->y + vel->dy * dt };

		if (_IsOutOfBounds(&newPos, hb, r)) {
			vel->dx = 0;
			vel->dy = 0;
		}
	}
}
