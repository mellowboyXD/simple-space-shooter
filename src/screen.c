#include "screen.h"
#include "raylib.h"
#include <assert.h>
#include <math.h>

#define ASSERT_INITLIALIZED \
	(assert(initCalled && "Screen was not initialized."))

static bool initCalled = false;

float _GetTargetScale(ScreenData *screen)
{
    ASSERT_INITLIALIZED;
    float scaleX = (float) GetRenderWidth() / screen->renderWidth;
    float scaleY = (float) GetRenderHeight() / screen->renderHeight;
    
    return fminf(scaleX, scaleY);
}

void ScreenInit(ScreenData *screen, int width, int height)
{
	screen->renderWidth = width;
	screen->renderHeight = height;
	screen->target = LoadRenderTexture(width, height);

	SetTextureFilter(screen->target.texture, TEXTURE_FILTER_POINT);
	initCalled = true;
}

void ScreenDeinit(ScreenData *screen)
{
	UnloadRenderTexture(screen->target);
	initCalled = false;
}

void ScreenDraw(ScreenData *screen)
{
    ASSERT_INITLIALIZED;

	float scale = _GetTargetScale(screen);
    float destWidth = screen->renderWidth * scale;
    float destHeight = screen->renderHeight * scale;
    float destX = 0.0f;
    float destY = (GetRenderHeight() - destHeight) * 0.5f;
    
	Vector2 origin = { 0, 0 };
	Rectangle source = { 0, 0, screen->renderWidth, -screen->renderHeight };
	Rectangle dest = { destX, destY, destWidth, destHeight };

	DrawTexturePro(screen->target.texture, source, dest, origin, 0.0f,
		       WHITE);
}

RenderTexture2D ScreenGetRenderTexture(ScreenData *screen)
{
    ASSERT_INITLIALIZED;
	return screen->target;
}
