#ifndef BULLET_SYSTEM_H
#define BULLET_SYSTEM_H

#include "systems/system.h"

typedef System BulletSystem;

BulletSystem *BulletSystemCreate();
void BulletSystemUpdate(BulletSystem *self, float dt);

#endif // BULLET_SYSTEM_H
