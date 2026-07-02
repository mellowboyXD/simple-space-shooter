#ifndef SYSTEM_H
#define SYSTEM_H

#include "entity_manager.h"

typedef enum {
    MOVEMENT_SYSTEM_TYPE,
    RENDER_SYSTEM_TYPE,
    MAX_SYSTEMS_TYPE,
    INVALID_SYSTEM_TYPE,
} SystemType;

typedef struct System System;

struct System {
    Entity entities[MAX_ENTITIES];
    size_t entityToIndexMap[MAX_ENTITIES];
    size_t count;
    void (*update) (System *self, float dt);
};

void SystemInit(System *self);
void SystemAddEntity(System *self, Entity entity);
void SystemRemoveEntity(System *self, Entity entity);

#endif /* SYSTEM_H */
