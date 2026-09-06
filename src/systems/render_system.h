#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "system.h"

static constexpr size_t MAX_LAYERS = 50;

typedef System RenderSystem;

RenderSystem *RenderSystemCreate();
void RenderSystemUpdate(RenderSystem *self, float dt);

#endif /* RENDER_SYSTEM_H */
