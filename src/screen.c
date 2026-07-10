#include "screen.h"
#include "raylib.h"

static bool initCalled = false;

void ScreenInit(ScreenData *screen, int width, int height)
{
    screen->renderWidth = width;
    screen->renderHeight = height;
    screen->target = LoadRenderTexture(width, height);

    SetTextureFilter(screen->target.texture, TEXTURE_FILTER_POINT);
    initCalled = true;
}
