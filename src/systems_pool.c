#include "systems_pool.h"
#include <assert.h>

void SystemsPoolInit(SystemsPool *systemsPool)
{
	systemsPool->size = MAX_SYSTEMS_TYPE;
	for (size_t i = 0; i < systemsPool->size; i++) {
		systemsPool->systems[i] = NULL;
	}
	systemsPool->count = 0;
}

void SystemsPoolAddSystem(SystemsPool *systemsPool, System *system)
{
	assert(systemsPool->count < systemsPool->size &&
	       "Not enough space to add another system");

	systemsPool->systems[systemsPool->count] = system;
	systemsPool->count++;
}

System *SystemsPoolGetSystem(SystemsPool *systemsPool, size_t index)
{
	assert(index < systemsPool->size && "Invalid system pool index");

	return systemsPool->systems[index];
}
