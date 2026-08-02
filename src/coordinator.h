#ifndef COORDINATOR_H
#define COORDINATOR_H

#include "assets.h"
#include "components.h"
#include "entity_manager.h"
#include "systems/system.h"

#define REGISTER_COMPONENT(T, type) \
	(CoordinatorRegisterComponent((type), sizeof(T)))
#define GET_COMPONENT(T, entity, type) \
	((T *)CoordinatorGetComponent((entity), (type)))

/* Entity related functions */
void CoordinatorInit();
void CoordinatorDeinit();
Entity CoordinatorCreateEntity();
void CoordinatorDestroyEntity(Entity entity);

/* Component related functions */
void CoordinatorRegisterComponent(ComponentType type, size_t sizeOfComponent);
void CoordinatorAddComponent(Entity entity, ComponentType type,
			     void *component);
void CoordinatorRemoveComponent(Entity entity, ComponentType type);
void *CoordinatorGetComponent(Entity entity, ComponentType type);

/* Systems related functions */
System *CoordinatorRegisterSystem(SystemType type,
				  void (*update)(System *self, float dt));
void CoordinatorSetSystemSignature(SystemType type, Signature signature);
System *CoordinatorGetSystem(SystemType type);

/* Assets related functions */
AssetId CoordinatorLoadAsset(const char *filename);
void CoordinatorUnloadAsset(AssetId id);

#endif /* COORDINATOR_H */
