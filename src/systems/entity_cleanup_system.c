#include "entity_cleanup_system.h"
#include "components.h"
#include "coordinator.h"
#include "debug.h"
#include <stddef.h>

EntityCleanUpSystem *EntityCleanUpSystemCreate()
{
	EntityCleanUpSystem *self = CoordinatorRegisterSystem(
		ENTITY_CLEANUP_SYSTEM_TYPE, EntityCleanUpSystemUpdate);

	Signature signature = COMPONENT_BIT(TAG_FOR_CLEANUP);
	CoordinatorSetSystemSignature(ENTITY_CLEANUP_SYSTEM_TYPE, signature);
	return self;
}

void EntityCleanUpSystemUpdate(EntityCleanUpSystem *self,
			       [[maybe_unused]] float dt)
{
	if (self->count <= 0)
		return; // no entities to cleanup

        for (size_t i = self->count; i > 0; i--) {
		Entity entity = self->entities[i - 1];

		CoordinatorDestroyEntity(entity);
	}
}
