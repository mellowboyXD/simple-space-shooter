#include "component_pool.h"
#include "components.h"
#include "debug.h"
#include <assert.h>
#include <string.h>

/**
 * Initializes the current component pool with some sensible defaults
 */
void ComponentPoolInit(ComponentPool *pool, size_t size)
{
	pool->count = 0;
	pool->sizeOfComponent = size;

	for (Entity i = 0; i < MAX_ENTITIES; i++) {
		pool->entityToIndexMap[i] = INVALID_INDEX;
		pool->indexToEntityMap[i] = INVALID_ENTITY;
	}
}

/**
 * Checks if a entity  already has the component
 */
bool ComponentPoolHas(ComponentPool *pool, Entity entity)
{
	assert(entity < MAX_ENTITIES && "Invalid entity. Out of range");

	return pool->entityToIndexMap[entity] != INVALID_INDEX;
}

/**
 * Returns the component data for an entity
 */
void *ComponentPoolGet(ComponentPool *pool, Entity entity)
{
	assert(entity < MAX_ENTITIES && "Invalid entity. Out of range");

	if (!ComponentPoolHas(pool, entity))
		return NULL;

	size_t index = pool->entityToIndexMap[entity];
	return (char *)pool->data + index * pool->sizeOfComponent;
}

/**
 * Adds a component to an entity.
 */
void *ComponentPoolAdd(ComponentPool *pool, Entity entity, void *component)
{
	assert(entity < MAX_ENTITIES && "Invalid entity. Out of range");

	if (ComponentPoolHas(pool, entity)) {
		LOG(L_INFO, "Entity `%d` already has component.", entity);
		size_t index = pool->entityToIndexMap[entity];
		return (char *)pool->data + index * pool->sizeOfComponent;
	}

	assert(pool->count < MAX_COMPONENTS && "Max components reached");
	size_t index = pool->count++;

	memcpy((char *)pool->data + index * pool->sizeOfComponent, component,
	       pool->sizeOfComponent);

	pool->entityToIndexMap[entity] = index;
	pool->indexToEntityMap[index] = entity;
	return (char *)pool->data + index * pool->sizeOfComponent;
}

/**
 * Removes a component from entity.
 */
void ComponentPoolRemove(ComponentPool *pool, Entity entity)
{
	assert(entity < MAX_ENTITIES && "Invalid entity. Out of range");

	if (!ComponentPoolHas(pool, entity)) {
		LOG(L_INFO,
		    "Attempting to remove a non-existant component from entity `%d`.",
		    entity);
		return;
	}

	assert(pool->count > 0 &&
	       "There should be at least 1 entity with the current component.");

	size_t index = pool->entityToIndexMap[entity];
	size_t indexOfLastEntity = pool->count - 1;
	Entity lastEntity = pool->indexToEntityMap[indexOfLastEntity];

	if (lastEntity != entity) {
		// points to last entity's data
		void *lastData = (char *)pool->data +
				 indexOfLastEntity * pool->sizeOfComponent;

		// points to the removed entity's data
		void *removedData =
			(char *)pool->data + index * pool->sizeOfComponent;

		// copy the last entity's data into the index of the removed component
		memcpy(removedData, lastData, pool->sizeOfComponent);

		// update the maps
		pool->entityToIndexMap[lastEntity] = index;
		pool->indexToEntityMap[index] = lastEntity;
	}

	// invalidate the last entity locations in the map since we've already moved
	// the last entity to be where the removed entity was
	pool->entityToIndexMap[entity] = INVALID_INDEX;
	pool->indexToEntityMap[indexOfLastEntity] = INVALID_ENTITY;

	pool->count--;
}
