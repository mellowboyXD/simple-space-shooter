#include "system_manager.h"
#include "debug.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void SystemManagerInit(SystemManager *manager)
{
	memset(manager, 0, sizeof(*manager));
    for (SystemType t = 0; t < MAX_SYSTEMS_TYPE; t++) {
        SystemInit(manager->systemPool + t);
    }
}

/**
 * Register a new system's update method to the system registry
 */
System *SystemManagerRegister(SystemManager *manager, SystemType systemType,
			   void (*SystemUpdate)(System *self, float dt))
{
	assert(systemType < MAX_SYSTEMS_TYPE && "Invalid system type.");
	assert(manager->systemPool[systemType].update == NULL &&
	       "System is already registered.");

	manager->systemPool[systemType].update = SystemUpdate;
    return manager->systemPool + systemType;
}

/**
 * Sets the system signature
 */
void SystemManagerSetSignature(SystemManager *manager, SystemType systemType,
			       Signature signature)
{
	assert(systemType < MAX_SYSTEMS_TYPE && "Invalid system type.");
	manager->signatures[systemType] = signature;
}

/**
 * Remove an entity from the system lists
 */
void SystemManagerEntityDestroyed(SystemManager *manager, Entity entity)
{
    assert(entity < MAX_ENTITIES && "Invalid entity. Out of range.");

	for (SystemType t = 0; t < MAX_SYSTEMS_TYPE; t++) {
		System *system = manager->systemPool + t;
		for (size_t i = 0; i < system->count; i++) {
			if (system->entities[i] == entity) {
				// swap with last to maintain packed array
				system->entities[i] = system->entities[system->count--];
				break; // move to next system
			}
		}
	}
}

/**
 * Adds entities or remove entities if it belongs to a system, otherwise remove
 * it.
 */
void SystemManagerEntitySignatureChanged(SystemManager *manager, Entity entity, Signature entitySignature)
{
    assert(entity < MAX_ENTITIES && "Invalid entity. Out of range.");

    for (SystemType t = 0; t < MAX_SYSTEMS_TYPE; t++) {
        System *system = manager->systemPool + t;
        Signature systemSignature = manager->signatures[t];

        if ((entitySignature & systemSignature) == systemSignature) {
            SystemAddEntity(system, entity);
        } else {
            // entity signature does not match system, remove from set
            SystemRemoveEntity(system, entity);
        }
    }
}

/**
 * Returns a pointer to the requested system.
 */
System *SystemManagerGetSystem(SystemManager *manager, SystemType systemType)
{
	assert(systemType < MAX_SYSTEMS_TYPE && "Invalid system type.");

    return manager->systemPool + systemType;
}
