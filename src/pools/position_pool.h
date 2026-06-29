/**
 * Responsible for adding an entity into the position pool array, which means
 * that the entity will have a position component for the position system to
 * operate on.
 * TODO: maintain lookup tables (entityToIndex[] && indexToEntity[])
 * TODO: provide a function to add a component value to an entity
 * TODO: provide a function to check if entity already has component
 * TODO: provide a function to get the component data for an entity
 * TODO: provide a function to remove a component from an entity
 */
#ifndef POSITION_POOL_H
#define POSITION_POOL_H

#include "entity_manager.h"
#include "components.h"

#include <stddef.h>

typedef struct {
    Position pool[MAX_ENTITIES];
    Entity indexToEntityMap[MAX_ENTITIES]; /* array index to entity */
    size_t entityToIndexMap[MAX_ENTITIES]; /* entity to array index in pool */
    size_t size; /* number of valid entries */
} PositionPool;

bool hasPosition(Entity entity);
Position *AddPosition(Entity entity, Position position);
void RemovePosition(Entity entity);

#endif /* POSITION_POOL_H */
