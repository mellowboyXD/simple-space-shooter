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
		INPUT_SYSTEM_TYPE, UICallbackSystemUpdate);

	Signature signature = COMPONENT_BIT(COMPONENT_UI_MOUSE_INPUT_STATE) |
			      COMPONENT_BIT(COMPONENT_UI_CALLBACK);
	CoordinatorSetSystemSignature(INPUT_SYSTEM_TYPE, signature);

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

Vector2 GetVirtualMousePosition(GameData *gameData)
{
	float scale = fminf((float)GetRenderWidth() / gameData->renderWidth,
			     (float)GetRenderHeight() / gameData->renderHeight);

	float destWidth = gameData->renderWidth * scale;
	float destHeight = gameData->renderHeight * scale;
	float offsetX = (GetRenderWidth() - destWidth) * 0.5f;
	float offsetY = (GetRenderHeight() - destHeight) * 0.5f;

	Vector2 mouse = GetMousePosition();
	return (Vector2){
		(mouse.x - offsetX) / scale,
		(mouse.y - offsetY) / scale,
	};
}
