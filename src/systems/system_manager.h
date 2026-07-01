#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "entity_manager.h"
#include "system.h"
#include <stddef.h>

typedef struct {
	Signature signatures[MAX_SYSTEMS_TYPE];
	System systemPool[MAX_SYSTEMS_TYPE];
} SystemManager;

void SystemManagerInit(SystemManager *manager);
System *SystemManagerRegister(SystemManager *manager, SystemType systemType,
			   void (*systemUpdate)(System *self, float dt));
void SystemManagerSetSignature(SystemManager *manager, SystemType systemType,
			       Signature signature);
void SystemManagerEntityDestroyed(SystemManager *manager, Entity entity);
void SystemManagerEntitySignatureChanged(SystemManager *manager, Entity entity,
					 Signature entitySignature);
System *SystemManagerGetSystem(SystemManager *manager, SystemType systemType);

#endif /* SYSTEM_MANAGER_H */
