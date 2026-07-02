#include "movement_system.h"
#include "coordinator.h"

MovementSystem *MovementSystemCreate()
{
	MovementSystem *self = CoordinatorRegisterSystem(MOVEMENT_SYSTEM_TYPE,
							 MovementSystemUpdate);
	Signature signature = COMPONENT_BIT(COMPONENT_POSITION) |
			      COMPONENT_BIT(COMPONENT_VELOCITY);
	CoordinatorSetSystemSignature(MOVEMENT_SYSTEM_TYPE, signature);
	return self;
}

void MovementSystemUpdate(MovementSystem *self, float dt)
{
	for (size_t i = 0; i < self->count; i++) {
		Entity entity = self->entities[i];

		Position *position =
			GET_COMPONENT(Position, entity, COMPONENT_POSITION);
		Velocity *velocity =
			GET_COMPONENT(Velocity, entity, COMPONENT_VELOCITY);

		position->pos.x += velocity->vel.x * dt;
		position->pos.y += velocity->vel.y * dt;
	}
}
