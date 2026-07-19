#include "screen.h"
#include "raylib.h"
#include <assert.h>
#include <math.h>

#define ASSERT_INITLIALIZED(screen) \
	(assert(screen->initCalled && "Screen was not initialized."))

float _GetTargetScale(ScreenData *screen)
{
	ASSERT_INITLIALIZED(screen);
	int paddingY =
		screen->padding[PADDING_TOP] + screen->padding[PADDING_BOTTOM];
	float scaleX = (float)GetRenderWidth() / screen->renderWidth;
	float scaleY =
		(float)(GetRenderHeight() - paddingY) / screen->renderHeight;

	return fminf(scaleX, scaleY);
}

void ScreenInit(ScreenData *screen, int width, int height)
{
	screen->renderWidth = width;
	screen->renderHeight = height;
	screen->target = LoadRenderTexture(width, height);

	SetTextureFilter(screen->target.texture, TEXTURE_FILTER_POINT);
	SetTextureWrap(screen->target.texture, TEXTURE_WRAP_CLAMP);
	for (int i = 0; i < PADDING_ALL; i++) {
		screen->padding[i] = 0;
	}
	screen->initCalled = true;
}

void ScreenDeinit(ScreenData *screen)
{
	ASSERT_INITLIALIZED(screen);
	UnloadRenderTexture(screen->target);
	screen->initCalled = false;
}

void ScreenDrawTarget(ScreenData *screen)
{
	ASSERT_INITLIALIZED(screen);

	float scale = _GetTargetScale(screen);
	float destWidth = screen->renderWidth * scale;
	float destHeight = screen->renderHeight * scale;
	float destX = screen->padding[PADDING_LEFT] * scale;

	int paddingY =
		(screen->padding[PADDING_TOP] + screen->padding[PADDING_BOTTOM]) * scale;
    float availableHeight = GetRenderHeight() - paddingY;
    float centerY = (availableHeight - destHeight) * 0.5f;
	float destY = centerY + (screen->padding[PADDING_TOP] * scale);

	Vector2 origin = { 0, 0 };
	Rectangle source = { 0, 0, screen->renderWidth, -screen->renderHeight };
	Rectangle dest = { destX, destY, destWidth, destHeight };

	DrawTexturePro(screen->target.texture, source, dest, origin, 0.0f,
		       WHITE);
}

RenderTexture2D ScreenGetRenderTexture(ScreenData *screen)
{
	ASSERT_INITLIALIZED(screen);
	return screen->target;
}

int ScreenGetVirtualWidth(ScreenData *screen)
{
	ASSERT_INITLIALIZED(screen);
	return screen->renderWidth;
}

int ScreenGetVirtualHeight(ScreenData *screen)
{
	ASSERT_INITLIALIZED(screen);
	return screen->renderHeight;
}

void ScreenSetPadding(ScreenData *screen, PaddingDirection direction, int value)
{
    if (direction == PADDING_ALL) {
        for (int i = 0; i < PADDING_ALL; i ++) {
            screen->padding[i] = value;
        }
        return;
    }

	screen->padding[direction] = value;
}
