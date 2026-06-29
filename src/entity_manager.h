#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <stdint.h>

// An entity is just an id.
typedef uint32_t Entity;

constexpr Entity MAX_ENTITIES = 100;
constexpr Entity INVALID_ENTITY = MAX_ENTITIES + 1;

// each bit represents a component as defined by ComponentMask
typedef uint16_t Signature;

void InitEntityManager();
Entity CreateEntity();
void DestroyEntity(Entity entity);
void SetSignature(Entity entity, Signature signature);
Signature GetSignature(Entity entity);

#endif /* ENTITY_MANAGER_H */
