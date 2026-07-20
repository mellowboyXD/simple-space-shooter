#include "systems_pool.h"
#include <assert.h>

#define ASSERT_INITIALIZED \
	(assert(initCalled && "Error. SystemsPool uninitialized."))

// there should be at most 1 SystemsPool running
static bool initCalled = false;

void SystemsPoolInit(SystemsPool *systemsPool)
{
	assert(!initCalled && "Already initialized.");
	systemsPool->size = MAX_SYSTEMS_TYPE;
	for (size_t i = 0; i < systemsPool->size; i++) {
		systemsPool->systems[i] = NULL;
		systemsPool->sysToIndexMap[i] = -1;
	}
	systemsPool->count = 0;
    initCalled = true;
}

void SystemsPoolDeinit(SystemsPool *systemsPool)
{
	ASSERT_INITIALIZED;
	for (size_t i = 0; i < systemsPool->size; i++) {
		systemsPool->systems[i] = NULL;
	}
	systemsPool->count = 0;
}

void SystemsPoolAddSystem(SystemsPool *systemsPool, System *system)
{
	ASSERT_INITIALIZED;
	assert(systemsPool->count < systemsPool->size &&
	       "Not enough space to add another system");
	SystemType type = system->type;
	assert(type < MAX_SYSTEMS_TYPE &&
	       "Invalid system. The requested system has been terminated.");
	assert(systemsPool->sysToIndexMap[type] == -1 &&
	       "System is already present.");

	systemsPool->systems[systemsPool->count] = system;
	systemsPool->sysToIndexMap[type] = systemsPool->count++;
}

System *SystemsPoolGetSystem(SystemsPool *systemsPool, SystemType type)
{
	ASSERT_INITIALIZED;
	assert(type < MAX_SYSTEMS_TYPE && "Invalid system type.");
	assert(systemsPool->sysToIndexMap[type] > -1 &&
	       "System is not registered.");

	size_t index = systemsPool->sysToIndexMap[type];
	return systemsPool->systems[index];
}

/**
 * Returns the index inside of pool where the particular system type lives.
 */
size_t SystemsPoolGetIndex(SystemsPool *systemsPool, SystemType type)
{
	assert(type < MAX_SYSTEMS_TYPE && "Invalid System.");
	assert(systemsPool->sysToIndexMap[type] > -1 &&
	       "System is not registered.");

	return systemsPool->sysToIndexMap[type];
}
