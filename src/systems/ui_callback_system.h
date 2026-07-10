#ifndef UI_CALLBACK_SYSTEM_H
#define UI_CALLBACK_SYSTEM_H

#include "game.h"
#include "raylib.h"
#include "systems/system.h"

typedef System UICallbackSystem;

UICallbackSystem *UICallbackSystemCreate();
void UICallbackSystemUpdate(UICallbackSystem *self, float dt);
Vector2 GetVirtualMousePosition(GameData *gameData);

#endif /* UI_CALLBACK_SYSTEM_H */
