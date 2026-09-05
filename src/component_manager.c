#include "component_manager.h"
#include "components.h"
#include "debug.h"
#include <assert.h>

#define ASSERT_ACTIVE_TYPE(componentType)                        \
	(assert(manager->activePools[(componentType)] == true && \
		"Inactive component type."))

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
	LOG(L_INFO, "ComponentManager successfully initialized.");
}

/**
 * Deinitializes the manager and frees up memory.
 */
void ComponentManagerDeinit(ComponentManager *manager)
{
	for (size_t i = 0; i < MAX_COMPONENTS; i++) {
		if (manager->activePools[i]) {
			ComponentPoolDeinit(manager->pools + i);
			manager->activePools[i] = false;
		}
	}

	LOG(L_INFO, "ComponentManager successfully deinitialized.");
}

/**
 * Registers a new component for use and returns its unique id(ComponentType).
 */
void ComponentManagerRegister(ComponentManager *manager,
			      ComponentType componentType,
			      size_t sizeOfComponent)
{
	assert(manager->count < MAX_COMPONENTS && "Max components reached.");
	assert(manager->activePools[componentType] == false &&
	       "Component has alredy been registered.");

	ComponentPool *pool = manager->pools + componentType;
	ComponentPoolInit(pool, sizeOfComponent);
	manager->activePools[componentType] = true;
	manager->count++;
}

/**
 * Adds a component to the pool after it has been registered
 */
void *ComponentManagerAdd(ComponentManager *manager,
			  ComponentType componentType, Entity entity,
			  void *component)
{
	ASSERT_COMPONENT_TYPE(componentType);
	ASSERT_ACTIVE_TYPE(componentType);

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
	ASSERT_COMPONENT_TYPE(componentType);
	ASSERT_ACTIVE_TYPE(componentType);

	ComponentPool *pool = manager->pools + componentType;
	ComponentPoolRemove(pool, entity);
}

/**
 * Returns true if the entity has the component
 */
bool ComponentManagerHas(const ComponentManager *manager,
			 ComponentType componentType, Entity entity)
{
	ASSERT_COMPONENT_TYPE(componentType);
	ASSERT_ACTIVE_TYPE(componentType);

	const ComponentPool *pool = manager->pools + componentType;
	return ComponentPoolHas(pool, entity);
}

/**
 * Returns the component data for an entity
 */
void *ComponentManagerGet(const ComponentManager *manager,
			  ComponentType componentType, Entity entity)
{
	ASSERT_COMPONENT_TYPE(componentType);
	ASSERT_ACTIVE_TYPE(componentType);

	const ComponentPool *pool = manager->pools + componentType;
	return ComponentPoolGet(pool, entity);
}

/**
 * If an entity has been destroyed, all of its associated components must be
 * released into the wild.
 */
void ComponentManagerEntityDestroyed(ComponentManager *manager, Entity entity)
{
	for (size_t i = 0; i < MAX_COMPONENTS; i++) {
		ComponentPool *pool = manager->pools + i;
		if (manager->activePools[i] && ComponentPoolHas(pool, entity)) {
			ComponentPoolRemove(pool, entity);
		}
	}
}
