// TODO: this system needs serious make-over
// need to model it after ui widgets and interactions
// possibly implement the Command Design Pattern for input handling
// should also have the rendering of ui components occur on a separate texture
// this is totally not required for completing ticket #16
#include "systems/ui_callback_system.h"
#include "components.h"
#include "coordinator.h"
#include "entity_manager.h"
#include "systems/system.h"
#include "ui_components.h"
#include <math.h>

UICallbackSystem *UICallbackSystemCreate()
{
	UICallbackSystem *system = CoordinatorRegisterSystem(
		UI_CALLBACK_SYSTEM_TYPE, UICallbackSystemUpdate);

	Signature signature = COMPONENT_BIT(COMPONENT_UI_MOUSE_INPUT_STATE) |
			      COMPONENT_BIT(COMPONENT_UI_CALLBACK);
	CoordinatorSetSystemSignature(UI_CALLBACK_SYSTEM_TYPE, signature);

	return system;
}

void UICallbackSystemUpdate(UICallbackSystem *self, float dt)
{
	for (size_t i = 0; i < self->count; i++) {
		Entity entity = self->entities[i];

		UIMouseInputState *mouseInputState =
			GET_COMPONENT(UIMouseInputState, entity,
				      COMPONENT_UI_MOUSE_INPUT_STATE);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			mouseInputState->isPressed = true;
		} else {
			mouseInputState->isPressed = false;
		}
	}
}
