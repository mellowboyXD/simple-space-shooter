#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "systems/system.h"

typedef System MovementSystem;

MovementSystem *MovementSystemCreate();
void MovementSystemUpdate(MovementSystem *self, float dt);

#endif /* MOVEMENT_SYSTEM_H */
