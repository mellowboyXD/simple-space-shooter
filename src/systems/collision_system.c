#include "collision_system.h"
#include "components.h"
#include "constants.h"
#include "coordinator.h"
#include "debug.h"
#include "utils.h"

// both pointer and value are immutable
static bool _IsXOutOfBounds(const Vector2 hitboxPos, const Hitbox *const hb)
{
	return hitboxPos.x < 0 || hitboxPos.x + hb->width > GAME_VIEW_WIDTH;
}

static bool _IsYOutOfBounds(const Vector2 hbPos, const Hitbox *const hb)
{
	return hbPos.y < 0 || hbPos.y + hb->height > GAME_VIEW_HEIGHT;
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
		Vector2 hitboxPos =
			GetHitboxPos(&newPos, r, hb, (Vector2){ 0, 0 });

                if (entity == 12) {
                        LOG(L_DEBUG, "pos: (%f, %f)", pos->x, pos->y);
                }
		if (_IsXOutOfBounds(hitboxPos, hb)) {
			if (CoordinatorIsPlayer(entity)) {
				vel->dx = 0;
			} else {
				CoordinatorAddTag(entity, TAG_FOR_CLEANUP);
			}
		}

		if (_IsYOutOfBounds(hitboxPos, hb)) {
			if (CoordinatorIsPlayer(entity)) {
				vel->dy = 0;
			} else {
				CoordinatorAddTag(entity, TAG_FOR_CLEANUP);
			}
		}
	}
}
