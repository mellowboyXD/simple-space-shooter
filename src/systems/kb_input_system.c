#include "kb_input_system.h"
#include "constants.h"
#include "coordinator.h"

#include <math.h>

Velocity _NormalizeVelocity(Velocity vel)
{
	double len = sqrt(vel.dx * vel.dx + vel.dy * vel.dy);
	if (len == 0)
		return vel;

	return (Velocity) {vel.dx / len, vel.dy / len};
}

KBInputSystem *KBInputSystemCreate()
{
	KBInputSystem *self =
		CoordinatorRegisterSystem(KB_INPUT_SYSTEM, KBInputSystemUpdate);
	Signature signature = COMPONENT_BIT(COMPONENT_VELOCITY) |
			      COMPONENT_BIT(TAG_PLAYER);
	CoordinatorSetSystemSignature(KB_INPUT_SYSTEM, signature);
	return self;
}

void KBInputSystemUpdate(KBInputSystem *self, [[maybe_unused]] float dt)
{
	for (size_t i = 0; i < self->count; i++) {
		Entity player = self->entities[i];

		float speed = PLAYER_SPEED;

		Velocity *vel =
			GET_COMPONENT(Velocity, player, COMPONENT_VELOCITY);

                Velocity v = {0};
		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
			v.dy = -1;
		}

		if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
			v.dy = 1;
		}

		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
			v.dx = -1;
		}

		if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
			v.dx = 1;
		}

		Velocity normalized = _NormalizeVelocity(v);
		vel->dy = normalized.dy * speed;
		vel->dx = normalized.dx * speed;
	}
}
