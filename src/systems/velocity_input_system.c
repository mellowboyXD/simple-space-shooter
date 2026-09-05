#include "velocity_input_system.h"
#include "coordinator.h"

#include <math.h>

static Vector2 _NormalizeVector2(Vector2 dir)
{
	double len = sqrt(dir.x * dir.x + dir.y * dir.y);
	if (len == 0)
		return dir;

	return (Vector2){ dir.x / len, dir.y / len };
}

VelocityInputSystem *VelocityInputSystemCreate()
{
	VelocityInputSystem *self = CoordinatorRegisterSystem(
		VELOCITY_INPUT_SYSTEM_TYPE, VelocityInputSystemUpdate);

	Signature signature = COMPONENT_BIT(COMPONENT_PLAYER_INPUT) |
			      COMPONENT_BIT(COMPONENT_PLAYER_MODIFIERS) |
			      COMPONENT_BIT(COMPONENT_VELOCITY);

	CoordinatorSetSystemSignature(VELOCITY_INPUT_SYSTEM_TYPE, signature);
	return self;
}

void VelocityInputSystemUpdate(VelocityInputSystem *self,
			       [[maybe_unused]] float dt)
{
	for (size_t i = 0; i < self->count; i++) {
		Entity entity = self->entities[i];

		PlayerInput *input = GET_COMPONENT(PlayerInput, entity,
						   COMPONENT_PLAYER_INPUT);
		Velocity *vel =
			GET_COMPONENT(Velocity, entity, COMPONENT_VELOCITY);
		PlayerModifiers *mod = GET_COMPONENT(
			PlayerModifiers, entity, COMPONENT_PLAYER_MODIFIERS);

		Vector2 dir = { 0, 0 };
		if (input->left) {
			dir.x -= 1;
		}

		if (input->right) {
			dir.x += 1;
		}

		if (input->up) {
			dir.y -= 1;
		}

		if (input->down) {
			dir.y += 1;
		}

		Vector2 normalized = _NormalizeVector2(dir);
		vel->dx = normalized.x * mod->speed;
		vel->dy = normalized.y * mod->speed;
	}
}
