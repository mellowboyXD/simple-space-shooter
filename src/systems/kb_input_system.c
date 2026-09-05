#include "kb_input_system.h"
#include "components.h"
#include "coordinator.h"
#include "raylib.h"

#include <assert.h>

// Public Functions

KBInputSystem *KBInputSystemCreate()
{
	KBInputSystem *self = CoordinatorRegisterSystem(KB_INPUT_SYSTEM_TYPE,
							KBInputSystemUpdate);
	Signature signature = COMPONENT_BIT(COMPONENT_PLAYER_INPUT);
	CoordinatorSetSystemSignature(KB_INPUT_SYSTEM_TYPE, signature);
	return self;
}

void KBInputSystemUpdate(KBInputSystem *self, [[maybe_unused]] float dt)
{
	for (size_t i = 0; i < self->count; i++) {
		Entity player = self->entities[i];

		PlayerInput *input = GET_COMPONENT(PlayerInput, player,
						   COMPONENT_PLAYER_INPUT);
                (*input) = PLAYER_INPUT_INIT; // reset inputs

		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
                        input->up = true;
		}

		if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
                        input->down = true;
		}

		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                        input->left = true;
		}

		if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
                        input->right = true;
		}

		if (IsKeyDown(KEY_SPACE)) {
                        input->fire = true;
		}
	}
}
