#ifndef SCREEN_H
#define SCREEN_H

#include "raylib.h"

typedef struct {
	int renderWidth;
	int renderHeight;
	RenderTexture2D target;
	bool initCalled;
} ScreenData;

void ScreenInit(ScreenData *screen, int width, int height);
void ScreenDeinit(ScreenData *screen);
RenderTexture2D ScreenGetRenderTexture(ScreenData *screen);

/**
 * Draws the texture to the actual window, scaling to fit.
 */
void ScreenDrawToWindow(ScreenData *screen);

#endif /* SCREEN_H */
