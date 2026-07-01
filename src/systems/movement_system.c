#include "movement_system.h"
#include "components.h"
#include "coordinator.h"

void MovementSystemUpdate(MovementSystem *self, float dt)
{
	for (size_t i = 0; i < self->count; i++) {
		Entity entity = self->entities[i];
		bool hasPosition =
			CoordinatorHasComponent(entity, COMPONENT_POSITION);
		bool hasVelocity =
			CoordinatorHasComponent(entity, COMPONENT_VELOCITY);

		if (hasPosition && hasVelocity) {
			Position *position = GET_COMPONENT(Position, entity,
							   COMPONENT_POSITION);
			Velocity *velocity = GET_COMPONENT(Velocity, entity,
							   COMPONENT_VELOCITY);

			position->pos.x += velocity->vel.x * dt;
			position->pos.y += velocity->vel.y * dt;
		}
	}
}
