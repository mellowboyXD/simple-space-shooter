#ifndef VELOCITY_INPUT_SYSTEM_H
#define VELOCITY_INPUT_SYSTEM_H

#include "systems/system.h"

typedef System VelocityInputSystem;

VelocityInputSystem *VelocityInputSystemCreate();
void VelocityInputSystemUpdate(VelocityInputSystem *self,
			       [[maybe_unused]] float dt);

#endif // VELOCITY_INPUT_SYSTEM_H
