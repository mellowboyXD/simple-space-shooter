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
	/* Index of where a specific system is in the pool */
	int sysToIndexMap[MAX_SYSTEMS_TYPE];
} SystemsPool;

void SystemsPoolInit(SystemsPool *systemsPool);
void SystemsPoolDeinit(SystemsPool *systemsPool);
void SystemsPoolAddSystem(SystemsPool *systemsPool, System *system);
System *SystemsPoolGetSystem(SystemsPool *systemsPool, SystemType type);
System *SystemsPoolGetSystemByIndex(SystemsPool *systemsPool, size_t index);
size_t SystemsPoolGetIndex(SystemsPool *systemsPool, SystemType type);

#endif /* SYSTEMS_POOL_H */
