#include "position_pool.h"
#include "debug.h"
#include <assert.h>

void PositionPoolInit(PositionPool *pool)
{
	pool->size = 0;
	for (size_t i = 0; i < MAX_ENTITIES; i++) {
		pool->indexToEntityMap[i] = INVALID_ENTITY;
		pool->entityToIndexMap[i] = -1;
	}
}

bool PositionPoolHas(PositionPool *pool, Entity entity)
{
	assert(entity < MAX_ENTITIES && "Invalid entity. Out of range");
	return pool->entityToIndexMap[entity] != -1;
}

Position *PositionPoolAdd(PositionPool *pool, Entity entity, Position position)
{
	assert(entity < MAX_ENTITIES && "Invalid entity. Out of range");

	if (PositionPoolHas(pool, entity)) {
		size_t index = pool->entityToIndexMap[entity];
		return pool->data + index;
	}

	// add to the end of the component array
	assert(pool->size < MAX_ENTITIES &&
	       "Too many entities; Cannot add more.");

	size_t index = pool->size++;
	pool->data[index] = position;
	pool->entityToIndexMap[entity] = index;
	pool->indexToEntityMap[index] = entity;

	return pool->data + index;
}

void PositionPoolRemove(PositionPool *pool, Entity entity)
{
	assert(entity < MAX_ENTITIES && "Invalid entity. Out of range");

	if (!PositionPoolHas(pool, entity)) {
		LOG(L_INFO,
		    "Attempting to remove position component from an entity that does not possess it. (Entity: %d)",
		    entity);
		return;
	}

	// move the last entity to the current index
	// update the maps
    // this is done to maintain a packed array at all times
	size_t index = pool->entityToIndexMap[entity];
	Entity lastEntity = pool->indexToEntityMap[pool->size - 1];

	if (lastEntity != entity) {
		pool->data[index] = pool->data[pool->size - 1];
		pool->entityToIndexMap[lastEntity] = index;
		pool->indexToEntityMap[index] = lastEntity;
	}

	// invalidate old map entries
	pool->entityToIndexMap[entity] = -1;
	pool->indexToEntityMap[pool->size - 1] = INVALID_ENTITY;
	pool->size--;
}
