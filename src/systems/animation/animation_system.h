#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "system.h"

typedef System AnimationSystem;

AnimationSystem *AnimationSystemCreate();

void AnimationSystemUpdate(AnimationSystem *self, float dt);

#endif // ANIMATION_SYSTEM_H
