#include "screen.h"
#include "raylib.h"
#include <assert.h>
#include <math.h>

#define ASSERT_INITLIALIZED(screen) \
	(assert(screen->initCalled && "Screen was not initialized."))

void ScreenInit(ScreenData *screen, int width, int height)
{
	screen->renderWidth = width;
	screen->renderHeight = height;
	screen->target = LoadRenderTexture(width, height);

	SetTextureFilter(screen->target.texture, TEXTURE_FILTER_POINT);
	SetTextureWrap(screen->target.texture, TEXTURE_WRAP_CLAMP);
	screen->initCalled = true;
}

void ScreenDeinit(ScreenData *screen)
{
	ASSERT_INITLIALIZED(screen);
	UnloadRenderTexture(screen->target);
	screen->initCalled = false;
}

RenderTexture2D ScreenGetRenderTexture(ScreenData *screen)
{
	ASSERT_INITLIALIZED(screen);
	return screen->target;
}

void ScreenDrawToWindow(ScreenData *screen)
{
	float scale = fminf((float)GetRenderWidth() / screen->renderWidth,
			    (float)GetRenderHeight() / screen->renderHeight);

	float destW = screen->renderWidth * scale;
	float destH = screen->renderHeight * scale;
	float destX = (GetRenderWidth() - destW) * 0.5f;
	float destY = (GetRenderHeight() - destH) * 0.5f;

	Rectangle source = { 0, 0, screen->renderWidth, -screen->renderHeight };
	Rectangle dest = { destX, destY, destW, destH };
	Vector2 origin = { 0, 0 };

	DrawTexturePro(screen->target.texture, source, dest, origin, 0.0f,
		       WHITE);
}
