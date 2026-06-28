#ifndef COMPONENT_ARRAY_H
#define COMPONENT_ARRAY_H

#include "components.h"
#include "entity_manager.h"

typedef struct ComponentArray ComponentArray;

struct ComponentArray {
    Hitbox hitboxes[MAX_ENTITIES];
    Position position[MAX_ENTITIES];
    Velocity volocities[MAX_ENTITIES];
    Render renders[MAX_ENTITIES];
};

#endif /* COMPONENT_ARRAY_H */
