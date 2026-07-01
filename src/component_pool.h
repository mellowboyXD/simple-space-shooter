#ifndef COMPONENT_POOL_H
#define COMPONENT_POOL_H

#include "entity_manager.h"

#include <stddef.h>

typedef struct {
	void *data;
	size_t sizeOfComponent;
	Entity indexToEntityMap[MAX_ENTITIES]; /* maps indices to entity */
	size_t entityToIndexMap[MAX_ENTITIES]; /* maps entity to array index*/
	size_t count; /* current count */
} ComponentPool;

constexpr size_t INVALID_INDEX = (size_t) INVALID_ENTITY;

void ComponentPoolInit(ComponentPool *pool, size_t size);
bool ComponentPoolHas(ComponentPool *pool, Entity entity);
void *ComponentPoolGet(ComponentPool *pool, Entity entity);
void *ComponentPoolAdd(ComponentPool *pool, Entity entity, void *component);
void ComponentPoolRemove(ComponentPool *pool, Entity entity);

#endif /* COMPONENT_POOL_H */
