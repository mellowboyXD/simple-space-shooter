#ifndef ENTITY_CLEANUP_SYSTEM_H
#define ENTITY_CLEANUP_SYSTEM_H

#include "systems/system.h"

typedef System EntityCleanUpSystem;

EntityCleanUpSystem *EntityCleanUpSystemCreate();
void EntityCleanUpSystemUpdate(EntityCleanUpSystem *self,
			       [[maybe_unused]] float dt);

#endif // ENTITY_CLEANUP_SYSTEM_H
