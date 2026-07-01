#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <stddef.h>
#include <stdint.h>

// An entity is just an id.
typedef uint32_t Entity;

constexpr Entity MAX_ENTITIES = 100;
constexpr Entity INVALID_ENTITY = MAX_ENTITIES + 1;

// each bit represents a component as defined by ComponentMask
typedef uint16_t Signature;

typedef struct {
	// Queue holding the entities. Index represents the entityID and the entityMasks
	// at that index describes which components are associated with that entity.
	// Possibly a circular queue?
	Entity entityQueue[MAX_ENTITIES];

	// Total number of free entities
	size_t entityCount;

	uint32_t qFront; // Front of queue. Points to the first element of the queue
	uint32_t qRear; // Back of queue. Points to the last element of the queue

	// Stores the component associated with each entity
	Signature signatures[MAX_ENTITIES];

} EntityManager;

void EntityManagerInit(EntityManager *manager);
Entity EntityManagerCreate(EntityManager *manager);
void EntityManagerDestroy(EntityManager *manager, Entity entity);
void EntityManagerSetSignature(EntityManager *manager, Entity entity,
			       Signature signature);
Signature EntityManagerGetSignature(EntityManager *manager, Entity entity);

#endif /* ENTITY_MANAGER_H */
