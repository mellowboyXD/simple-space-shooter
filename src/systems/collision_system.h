#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "systems/system.h"

typedef System CollisionSystem;

CollisionSystem *CollisionSystemCreate();
void CollisionSystemUpdate(CollisionSystem *self, [[maybe_unused]] float dt);

#endif // COLLISION_SYSTEM_H
