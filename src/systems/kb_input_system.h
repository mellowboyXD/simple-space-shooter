#ifndef KB_INPUT_SYSTEM_H
#define KB_INPUT_SYSTEM_H

#include "systems/system.h"

typedef System KBInputSystem;

KBInputSystem *KBInputSystemCreate();
void KBInputSystemUpdate(KBInputSystem *self, [[maybe_unused]] float dt);

#endif // KB_INPUT_SYSTEM_H
