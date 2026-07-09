#include "systems/ui_callback_system.h"
#include "components.h"
#include "coordinator.h"
#include "entity_manager.h"
#include "systems/system.h"

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
}
