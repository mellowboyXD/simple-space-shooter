#include "kb_input_system.h"
#include "constants.h"
#include "coordinator.h"

#include "debug.h"
#include <math.h>

Velocity _NormalizeVelocity(Velocity vel)
{
	double len = sqrt(vel.dx * vel.dx + vel.dy * vel.dy);
	if (len == 0)
		return vel;

	return (Velocity){ vel.dx / len, vel.dy / len };
}

KBInputSystem *KBInputSystemCreate()
{
	KBInputSystem *self =
		CoordinatorRegisterSystem(KB_INPUT_SYSTEM_TYPE, KBInputSystemUpdate);
	Signature signature = COMPONENT_BIT(COMPONENT_VELOCITY) |
			      COMPONENT_BIT(COMPONENT_POSITION) |
			      COMPONENT_BIT(TAG_PLAYER);
	CoordinatorSetSystemSignature(KB_INPUT_SYSTEM_TYPE, signature);
	return self;
}

void KBInputSystemUpdate(KBInputSystem *self, [[maybe_unused]] float dt)
{
	for (size_t i = 0; i < self->count; i++) {
		Entity player = self->entities[i];

		Position *pos =
			GET_COMPONENT(Position, player, COMPONENT_POSITION);

		Velocity *vel =
			GET_COMPONENT(Velocity, player, COMPONENT_VELOCITY);

		Velocity dir = { 0 };
		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
			dir.dy = -1;
		}

		if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
			dir.dy = 1;
		}

		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
			dir.dx = -1;
		}

		if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
			dir.dx = 1;
		}

		if (IsKeyPressed(KEY_P)) {
			LOG(L_INFO, "player: (%f, %f)", pos->x, pos->y);
		}

		Velocity normalized = _NormalizeVelocity(dir);
		vel->dy = normalized.dy * PLAYER_SPEED;
		vel->dx = normalized.dx * PLAYER_SPEED;
	}
}
