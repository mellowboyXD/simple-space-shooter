/**
 * The entity manager is responsible to controlling the creation of entities
 * and giving signatures to entities.
 * As explained multiple times by now, a signature represents the components
 * associated with an entity where each bit represents a component.
 */
#include "entity_manager.h"
#include "components.h"

#include <assert.h>

// Queue holding the entities. Index represents the entityID and the entityMasks
// at that index describes which components are associated with that entity.
// Possibly a circular queue?
static Entity entityQueue[MAX_ENTITIES] = { 0 };

// Total number of free entities
static uint32_t entityCount = 0;

static uint32_t qFront = 0; // Front of queue. Points to the first element of the queue
static uint32_t qRear = MAX_ENTITIES - 1; // Back of queue. Points to the last element of the queue

// Stores the component associated with each entity
static Signature signatures[MAX_ENTITIES] = { 0 };

/**
 * Initializes the entity queue and populate it with ids.
 * Also resets the associated components.
 */
void InitEntityManager() 
{
    for (Entity i = 0; i < MAX_ENTITIES; i++) {
        entityQueue[i] = i;
        signatures[i] = COMPONENT_NONE;
    }
}

/**
 * Create a brand new entity with no components associated with it.
 * Returns an entity which is just an id or index to where that entity is in the
 * entityQueue.
 */
Entity CreateEntity()
{
	assert(entityCount < MAX_ENTITIES && "Too many entities");

    // How to create an entity?
    // Consume the front of the queue
    Entity id = entityQueue[qFront];
    qFront = (qFront + 1) % MAX_ENTITIES;
    entityCount++;

    return id;
}

void DestroyEntity(Entity entity)
{
    assert(entityCount > 0 && "Not enough entities");

    // To destroy an entity, we have to set its index in the queue to be invalid
    // invalidate the entity's component mask
    signatures[entity] = COMPONENT_NONE;

    qRear = (qRear + 1) % MAX_ENTITIES;
    entityQueue[qRear] = entity;
    entityCount--;
}

/**
 * Associates a component to an entity.
 */
void SetSignature(Entity entity, Signature signature)
{
    assert(entity < MAX_ENTITIES && "Invalid entity. Out of range");

    signatures[entity] = signature;
}

/**
 * Returns a mask representing the components associated with that entity.
 */
Signature GetSignature(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Invalid entity. Out of range");

    return signatures[entity];
}
