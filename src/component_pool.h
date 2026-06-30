#ifndef COMPONENT_POOL_H
#define COMPONENT_POOL_H

#include "entity_manager.h"

#include <stddef.h>

#define POOL_GET(pool, entity, T) ((T *)ComponentPoolGet((pool), (entity)))
#define POOL_ADD(pool, entity, component, T) \
	((T *)ComponentPoolAdd((pool), (entity), &(component)))

typedef struct {
	void *data;
	size_t sizeOfComponent;
	Entity indexToEntityMap[MAX_ENTITIES]; /* maps indices to entity */
	size_t entityToIndexMap[MAX_ENTITIES]; /* maps entity to array index*/
	size_t count; /* current count */
} ComponentPool;

void ComponentPoolInit(ComponentPool *pool);
bool ComponentPoolHas(ComponentPool *pool, Entity entity);
void *ComponentPoolAdd(ComponentPool *pool, Entity entity, void *component);
void *ComponentPoolGet(ComponentPool *pool, Entity entity);
void ComponentPoolRemove(ComponentPool *pool, Entity entity);

#endif /* COMPONENT_POOL_H */
