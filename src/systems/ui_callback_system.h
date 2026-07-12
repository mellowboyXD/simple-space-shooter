#ifndef UI_CALLBACK_SYSTEM_H
#define UI_CALLBACK_SYSTEM_H

#include "systems/system.h"

typedef System UICallbackSystem;

UICallbackSystem *UICallbackSystemCreate();
void UICallbackSystemUpdate(UICallbackSystem *self, float dt);

#endif /* UI_CALLBACK_SYSTEM_H */
