#include "component_manager.h"
#include <assert.h>

/**
 * Initializes the manager to be used.
 */
void ComponentManagerInit(ComponentManager *manager)
{
	manager->count = 0;
	for (size_t i = 0; i < MAX_COMPONENTS; i++) {
		manager->pools[i] = (ComponentPool){ 0 };
		manager->activePools[i] = false;
	}
}

/**
 * Registers a new component for use and returns its unique id(ComponentType).
 */
ComponentType ComponentManagerRegister(ComponentManager *manager,
				       size_t sizeOfComponent)
{
	assert(manager->count < MAX_COMPONENTS && "Max components reached.");

	size_t componentType = manager->count++;
	ComponentPool *pool = manager->pools + componentType;
	ComponentPoolInit(pool, sizeOfComponent);
	manager->activePools[componentType] = true;
	return (ComponentType)componentType;
}

/**
 * Adds a component to the pool after it has been registered
 */
void *ComponentManagerAdd(ComponentManager *manager,
			  ComponentType componentType, Entity entity,
			  void *component)
{
	assert(componentType < MAX_COMPONENTS &&
	       "Invalid component type. Out of range");
	assert(manager->activePools[componentType] == true &&
	       "Invalid component type. Inactive");

	ComponentPool *pool = manager->pools + componentType;
	return ComponentPoolAdd(pool, entity, component);
}

/**
 * Removes the component from the componentPool for an entity. Does not 
 * invalidate the component.
 */
void ComponentManagerRemove(ComponentManager *manager,
			    ComponentType componentType, Entity entity)
{
	assert(componentType < MAX_COMPONENTS &&
	       "Invalid component type. Out of range");
	assert(manager->activePools[componentType] == true &&
	       "Invalid component type. Inactive");

	ComponentPool *pool = manager->pools + componentType;
	ComponentPoolRemove(pool, entity);
}

/**
 * Returns true if the entity has the component
 */
bool ComponentManagerHas(const ComponentManager *manager,
			 ComponentType componentType, Entity entity)
{
	assert(componentType < MAX_COMPONENTS &&
	       "Invalid component type. Out of range");
	assert(manager->activePools[componentType] == true &&
	       "Invalid component type. Inactive");

	ComponentPool *pool = (ComponentPool *)manager->pools + componentType;
	return ComponentPoolHas(pool, entity);
}

/**
 * Returns the component data for an entity
 */
void *ComponentManagerGet(const ComponentManager *manager,
			  ComponentType componentType, Entity entity)
{
	assert(componentType < MAX_COMPONENTS &&
	       "Invalid component type. Out of range");
	assert(manager->activePools[componentType] == true &&
	       "Invalid component type. Inactive");

	ComponentPool *pool = (ComponentPool *)manager->pools + componentType;
	return ComponentPoolGet(pool, entity);
}

/**
 * If an entity has been destroyed, all of its associated components must be
 * released into the wild.
 */
void ComponentManagerEntityDestroyed(ComponentManager *manager, Entity entity)
{
	for (size_t i = 0; i < manager->count; i++) {
		ComponentPool *pool = manager->pools + i;
		if (manager->activePools[i] && ComponentPoolHas(pool, entity)) {
			ComponentPoolRemove(pool, entity);
		}
	}
}
