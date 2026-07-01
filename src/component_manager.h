#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "component_pool.h"
#include "components.h"

typedef struct {
	ComponentPool pools[MAX_COMPONENTS];
	bool activePools[MAX_COMPONENTS];
	size_t count;
} ComponentManager;

void ComponentManagerInit(ComponentManager *manager);
ComponentType ComponentManagerRegister(ComponentManager *manager,
				       size_t sizeOfComponent);
void *ComponentManagerAdd(ComponentManager *manager,
			  ComponentType componentType, Entity entity,
			  void *component);
void ComponentManagerRemove(ComponentManager *manager,
			    ComponentType componentType, Entity entity);
bool ComponentManagerHas(const ComponentManager *manager,
			 ComponentType componentType, Entity entity);
void *ComponentManagerGet(const ComponentManager *manager,
			  ComponentType componentType, Entity entity);
void ComponentManagerEntityDestroyed(ComponentManager *manager, Entity entity);

#endif /* COMPONENT_MANAGER_H */
