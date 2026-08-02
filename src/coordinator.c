/**
 * Global coordinator is responsible for coordinating tasks in the ECS.
 * Allows creation on entities and registration of components and systems.
 */

#include "coordinator.h"
#include "component_manager.h"
#include "debug.h"
#include "systems/system_manager.h"
#include <assert.h>
#include <string.h>

#define ASSERT_INITIALIZED(initCalled) \
	(assert((initCalled) &&        \
		"CoordinatorInit was not called for initialization."))

static ComponentManager componentManager = { 0 };
static EntityManager entityManager = { 0 };
static SystemManager systemManager = { 0 };
static bool initCalled = false;

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

	AssetsInit();

	initCalled = true;
	LOG(L_INFO, "Coordinator successfully initialized.");
}

/**
 * Deinitializes all managers
 */
void CoordinatorDeinit()
{
	assert(initCalled && "Coordinator init was never called");
	ComponentManagerDeinit(&componentManager);
	EntityManagerDeinit(&entityManager);
	SystemManagerDeinit(&systemManager);

	AssetsDeinit();

	LOG(L_INFO, "Coordinator successfully deinitialized.");
}

// ===== Entity related functions =====

/**
 * Create a new entity and return it. Delegates entity creation logic to the
 * entity manager.
 */
Entity CoordinatorCreateEntity()
{
	ASSERT_INITIALIZED(initCalled);
	return EntityManagerCreate(&entityManager);
}

/**
 * Destroy an entity.
 */
void CoordinatorDestroyEntity(Entity entity)
{
	ASSERT_INITIALIZED(initCalled);

	// TODO: Remove its assets if has render component

	EntityManagerDestroy(&entityManager, entity);
	ComponentManagerEntityDestroyed(&componentManager, entity);
	SystemManagerEntityDestroyed(&systemManager, entity);
}

/**
 * Returns the entity signature
 */
Signature CoordinatorGetEntitySignature(Entity entity)
{
	ASSERT_INITIALIZED(initCalled);
	return EntityManagerGetSignature(&entityManager, entity);
}

// ===== Component related functions =====

/**
 * Delegates component registration to the component manager.
 * Prefer using the REGISTER_COMPONENT macro.
 */
void CoordinatorRegisterComponent(ComponentType type, size_t sizeOfComponent)
{
	ASSERT_INITIALIZED(initCalled);
	ComponentManagerRegister(&componentManager, type, sizeOfComponent);
}

/**
 * Adds a component to an entity.
 */
void CoordinatorAddComponent(Entity entity, ComponentType type, void *component)
{
	ASSERT_INITIALIZED(initCalled);
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
	ASSERT_INITIALIZED(initCalled);
	ASSERT_COMPONENT_TYPE(type);

	// TODO: Remove texture assets if has render component

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
	ASSERT_INITIALIZED(initCalled);
	return ComponentManagerGet(&componentManager, type, entity);
}

// ===== System related functions =====

/**
 * Delegates system registration logic to system manager. You get the system
 * type by consulting the system.h file from the SystemType enum.
 */
System *CoordinatorRegisterSystem(SystemType type,
				  void (*update)(System *self, float dt))
{
	ASSERT_INITIALIZED(initCalled);
	return SystemManagerRegister(&systemManager, type, update);
}

/**
 * Delegates the getting of system signature to system manager.
 */
void CoordinatorSetSystemSignature(SystemType type, Signature signature)
{
	ASSERT_INITIALIZED(initCalled);
	SystemManagerSetSignature(&systemManager, type, signature);
}

/**
 * Delegates getting of system to system manager
 */
System *CoordinatorGetSystem(SystemType type)
{
	ASSERT_INITIALIZED(initCalled);
	return SystemManagerGetSystem(&systemManager, type);
}

// ===== Asset related functions =====

/**
 * Loads a texture file and returns its id. Delegates logic to
 * assets.h
 */
AssetId CoordinatorLoadAsset(const char *filename)
{
        ASSERT_INITIALIZED(initCalled);
        return AssetsLoadTexture(filename);
}

/**
 * Unloads a particular asset. Delegates logic to assets.h
 */
void CoordinatorUnloadAsset(AssetId id)
{
        ASSERT_INITIALIZED(initCalled);
        AssetUnloadTexture(id);
}
