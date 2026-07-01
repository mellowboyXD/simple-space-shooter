/**
 * Global coordinator is responsible for coordinating tasks in the ECS.
 * Allows creation on entities and registration of components and systems.
 */
#include "coordinator.h"
#include "component_manager.h"
#include "systems/system_manager.h"
#include <assert.h>
#include <string.h>

static ComponentManager componentManager = { 0 };
static EntityManager entityManager = { 0 };
static SystemManager systemManager = { 0 };

/**
 * Initializes all the managers
 */
void CoordinatorInit()
{
	memset(&componentManager, 0, sizeof(componentManager));
	memset(&entityManager, 0, sizeof(entityManager));
	memset(&systemManager, 0, sizeof(systemManager));

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
 * Destroy an entity. 
 */
void CoordinatorDestroyEntity(Entity entity)
{
	EntityManagerDestroy(&entityManager, entity);
	ComponentManagerEntityDestroyed(&componentManager, entity);
	SystemManagerEntityDestroyed(&systemManager, entity);
}

/** 
 * Returns the entity signature
 */
Signature CoordinatorGetEntitySignature(Entity entity)
{
	return EntityManagerGetSignature(&entityManager, entity);
}

bool CoordinatorHasComponent(Entity entity, ComponentType componentType)
{
	assert(entity < MAX_ENTITIES && "Invalid entity. Out of range");
	ASSERT_COMPONENT_TYPE(componentType);

	Signature componentSignature = COMPONENT_BIT(componentType);
	Signature entitySignature =
		EntityManagerGetSignature(&entityManager, entity);
	return entitySignature & componentSignature;
}

/**
 * Delegates component registration to the component manager.
 * Prefer using the REGISTER_COMPONENT macro.
 */
void CoordinatorRegisterComponent(ComponentType type, size_t sizeOfComponent)
{
	ComponentManagerRegister(&componentManager, type, sizeOfComponent);
}

/**
 * Adds a component to an entity.
 */
void CoordinatorAddComponent(Entity entity, ComponentType type, void *component)
{
	ASSERT_COMPONENT_TYPE(type);

	ComponentManagerAdd(&componentManager, type, entity, component);
	Signature sig = EntityManagerGetSignature(&entityManager, entity);
	sig |= COMPONENT_BIT(type);
	EntityManagerSetSignature(&entityManager, entity, sig);

	SystemManagerEntitySignatureChanged(&systemManager, entity, sig);
}

/**
 * Removes a component from an entity.
 */
void CoordinatorRemoveComponent(Entity entity, ComponentType type)
{
	ASSERT_COMPONENT_TYPE(type);

	ComponentManagerRemove(&componentManager, type, entity);
	Signature sig = EntityManagerGetSignature(&entityManager, entity);
	sig &= ~COMPONENT_BIT(type);
	EntityManagerSetSignature(&entityManager, entity, sig);

	SystemManagerEntitySignatureChanged(&systemManager, entity, sig);
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
