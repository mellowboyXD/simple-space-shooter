#include "entity_cleanup_system.h"
#include "components.h"
#include "constants.h"
#include "coordinator.h"

EntityCleanUpSystem *EntityCleanUpSystemCreate()
{
	EntityCleanUpSystem *self = CoordinatorRegisterSystem(
		ENTITY_CLEANUP_SYSTEM_TYPE, EntityCleanUpSystemUpdate);

	Signature signature = COMPONENT_BIT(COMPONENT_POSITION) |
			      COMPONENT_BIT(TAG_BULLET);
	CoordinatorSetSystemSignature(ENTITY_CLEANUP_SYSTEM_TYPE, signature);
	return self;
}

void EntityCleanUpSystemUpdate(EntityCleanUpSystem *self,
			       [[maybe_unused]] float dt)
{
	for (size_t i = 0; i < self->count; i++) {
		Entity entity = self->entities[i];
		Position *p =
			GET_COMPONENT(Position, entity, COMPONENT_POSITION);

		int offset = 20;

		if (p->x < -offset || p->x > GAME_VIEW_WIDTH + offset ||
		    p->y < -offset || p->y > GAME_VIEW_HEIGHT + offset) {
			CoordinatorDestroyEntity(entity);
		}
	}
}
