#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "system.h"

typedef System RenderSystem;

RenderSystem *RenderSystemCreate();
void RenderSystemUpdate(RenderSystem *self, float dt);

#endif /* RENDER_SYSTEM_H */
