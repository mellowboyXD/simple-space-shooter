#include "system.h"

#include "component_pool.h"
#include <assert.h>

/**
 * Initializes the system
 */
void SystemInit(System *self)
{
	for (size_t i = 0; i < MAX_ENTITIES; i++) {
		self->entities[i] = INVALID_ENTITY;
		self->entityToIndexMap[i] = INVALID_INDEX;
	}
	self->update = NULL;
	self->count = 0;
}

/**
 * Adds an existing entity from a system
 */
void SystemAddEntity(System *self, Entity entity)
{
	assert(entity < MAX_ENTITIES && "Invalid entities. Out of range");
	if (self->entityToIndexMap[entity] != INVALID_INDEX) {
		// entity already exists
		return;
	}

	size_t index = self->count++;
	self->entityToIndexMap[entity] = index;
	self->entities[index] = entity;
}

/**
 * Removes an existing entity from the system
 */
void SystemRemoveEntity(System *self, Entity entity)
{
	assert(entity < MAX_ENTITIES && "Invalid entities. Out of range");
	if (self->entityToIndexMap[entity] == INVALID_INDEX) {
		// Entity does not exist
		return;
	}

	assert(self->count > 0 &&
	       "There should be at least 1 entity to be removed");

	size_t index = self->entityToIndexMap[entity];
	Entity lastEntity = self->entities[self->count - 1];

	if (lastEntity != entity) {
		// swap with last entity
		// update map
		self->entities[index] = lastEntity;
		self->entityToIndexMap[lastEntity] = index;
	}

	self->entityToIndexMap[entity] = INVALID_INDEX;
	self->count--;
}
