#include "movement_system.h"
#include "coordinator.h"
#include "debug.h"

/**
 * This method creates a new movement system adding the required system
 * signatures.
 *
 * This is basically how I am going to use the ECS. The create function
 * encapsulates all the required setup for creating a new system, like for
 * example setting the system's signature.
 */
MovementSystem *MovementSystemCreate()
{
	MovementSystem *self = CoordinatorRegisterSystem(MOVEMENT_SYSTEM_TYPE,
							 MovementSystemUpdate);
	Signature signature = COMPONENT_BIT(COMPONENT_POSITION) |
			      COMPONENT_BIT(COMPONENT_VELOCITY);
	CoordinatorSetSystemSignature(MOVEMENT_SYSTEM_TYPE, signature);
	return self;
}

/**
 * This is where all the movement logic happens. Gathers all entities with the
 * required component and applies logic to them.
 */
void MovementSystemUpdate(MovementSystem *self, float dt)
{
	for (size_t i = 0; i < self->count; i++) {
		Entity entity = self->entities[i];

		Position *position =
			GET_COMPONENT(Position, entity, COMPONENT_POSITION);
		Velocity *velocity =
			GET_COMPONENT(Velocity, entity, COMPONENT_VELOCITY);

		position->x += velocity->dx * dt;
		position->y += velocity->dy * dt;

		// TODO: will need to map to debug window
		if (IsKeyPressed(KEY_P)) {
			LOG(L_INFO, "player (%f, %f)", position->x,
			    position->y);
		}
	}
}
