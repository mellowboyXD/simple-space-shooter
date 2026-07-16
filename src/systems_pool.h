#ifndef SYSTEMS_POOL_H
#define SYSTEMS_POOL_H

#include "systems/system.h"

/**
 * SystemsPool is an array of all the registered systems.
 */
typedef struct {
	System *systems[MAX_SYSTEMS_TYPE];
    size_t size;
	size_t count;
} SystemsPool;

void SystemsPoolInit(SystemsPool *systemsPool);
void SystemsPoolDeinit(SystemsPool *systemsPool);
void SystemsPoolAddSystem(SystemsPool *systemsPool, System *system);
System *SystemsPoolGetSystem(SystemsPool *systemsPool, size_t index);

#endif /* SYSTEMS_POOL_H */
