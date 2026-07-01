/**
 * Global coordinator is responsible for coordinating tasks in the ECS.
 * Allows creation on entities and registration of components and systems.
 */
#include "coordinator.h"
#include "component_manager.h"
#include "systems/system_manager.h"

static ComponentManager componentManager = { 0 };
static EntityManager entityManager = { 0 };
static SystemManager systemManager = { 0 };

/**
 * Initializes all the managers
 */
void CoordinatorInit()
{
	ComponentManagerInit(&componentManager);
	EntityManagerInit(&entityManager);
	SystemManagerInit(&systemManager);
}

/**
 * Create a new entity and return it. Delegates entity creation logic to the
 * entity manager.
 */
Entity CoordinatorCreateEntity()
{
	return EntityManagerCreate(&entityManager);
}

/**
 * Destroy an entity. Delegates entity destruction logic to the entity manager.
 */
void CoordinatorDestroyEntity(Entity entity)
{
	EntityManagerDestroy(&entityManager, entity);
}

/**
 * Delegates component registration to the component manager.
 * Prefer using the REGISTER_COMPONENT macro.
 */
ComponentType CoordinatorRegisterComponent(size_t sizeOfComponent)
{
	return ComponentManagerRegister(&componentManager, sizeOfComponent);
}

/**
 * Delegates component addition to the component manager
 */
void CoordinatorAddComponent(Entity entity, ComponentType type, void *component)
{
	ComponentManagerAdd(&componentManager, type, entity, component);
}

/**
 * Delegates component removal to component manager
 */
void CoordinatorRemoveComponent(Entity entity, ComponentType type)
{
	ComponentManagerRemove(&componentManager, type, entity);
}

/**
 * Delegates the getting component logic to the component manager.
 * Prefer using the GET_COMPONENT macro that handles typecasting automatically.
 */
void *CoordinatorGetComponent(Entity entity, ComponentType type)
{
	return ComponentManagerGet(&componentManager, type, entity);
}

/**
 * Delegates system registration logic to system manager. You get the system 
 * type by consulting the system.h file from the SystemType enum.
 */
System *CoordinatorRegisterSystem(SystemType type,
				  void (*update)(System *self, float dt))
{
	return SystemManagerRegister(&systemManager, type, update);
}

/**
 * Delegates the getting of system signature to system manager.
 */
void CoordinatorSetSystemSignature(SystemType type, Signature signature)
{
	SystemManagerSetSignature(&systemManager, type, signature);
}

/**
 * Delegates getting of system to system manager
 */
System *CoordinatorGetSystem(SystemType type)
{
	return SystemManagerGetSystem(&systemManager, type);
}
