/**
 * The entity manager is responsible to controlling the creation of entities
 * and giving signatures to entities.
 * As explained multiple times by now, a signature represents the components
 * associated with an entity where each bit represents a component.
 */

#include "entity_manager.h"

#include "components.h"
#include "debug.h"
#include <assert.h>

/**
 * Initializes the entity queue and populate it with ids.
 * Also resets the associated components.
 */
void EntityManagerInit(EntityManager *manager)
{
	manager->qFront = 0;
	manager->qRear = MAX_ENTITIES - 1;
	for (Entity i = 0; i < MAX_ENTITIES; i++) {
		manager->entityQueue[i] = i;
		manager->signatures[i] = COMPONENT_NONE;
	}
    LOG(L_INFO, "EntityManager successfully initialized.");
}

/**
 * Deinitializes the manager
 */
void EntityManagerDeinit(EntityManager *manager)
{
    // explicitly set to invalid to catch any potential use after free
    manager->entityCount = MAX_ENTITIES;
    manager->qFront = MAX_ENTITIES + 1;
    manager->qRear = MAX_ENTITIES + 1;
    LOG(L_INFO, "EntityManager successfully deinitialized.");
}

/**
 * Create a brand new entity with no components associated with it.
 * Returns an entity which is just an id or index to where that entity is in the
 * entityQueue.
 */
Entity EntityManagerCreate(EntityManager *manager)
{
	assert(manager->entityCount < MAX_ENTITIES && "Too many entities");

	// How to create an entity?
	// Consume the front of the queue
	Entity id = manager->entityQueue[manager->qFront];
	manager->qFront = (manager->qFront + 1) % MAX_ENTITIES;
	manager->entityCount++;

	return id;
}

void EntityManagerDestroy(EntityManager *manager, Entity entity)
{
	assert(manager->entityCount > 0 && "Not enough entities");

	// To destroy an entity, we have to set its index in the queue to be invalid
	// invalidate the entity's component mask
	manager->signatures[entity] = COMPONENT_NONE;

	manager->qRear = (manager->qRear + 1) % MAX_ENTITIES;
	manager->entityQueue[manager->qRear] = entity;
	manager->entityCount--;
}

/**
 * Associates a component to an entity.
 */
void EntityManagerSetSignature(EntityManager *manager, Entity entity,
			       Signature signature)
{
	ASSERT_ENTITY(entity);
	manager->signatures[entity] = signature;
}

/**
 * Returns a mask representing the components associated with that entity.
 */
Signature EntityManagerGetSignature(EntityManager *manager, Entity entity)
{
	ASSERT_ENTITY(entity);
	return manager->signatures[entity];
}
