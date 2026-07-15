#ifndef SCREEN_H
#define SCREEN_H

#include "raylib.h"

typedef struct {
	int renderWidth;
	int renderHeight;
	RenderTexture2D target;
} ScreenData;

void ScreenInit(ScreenData *screen, int width, int height);
void ScreenDeinit(ScreenData *screen);
void ScreenDrawTarget(ScreenData *screen);
RenderTexture2D ScreenGetRenderTexture(ScreenData *screen);
int ScreenGetVirtualWidth(ScreenData *screen);
int ScreenGetVirtualHeight(ScreenData *screen);
int ScreenGetWidth(ScreenData *screen);
int ScreenGetHeight(ScreenData *screen);

#endif /* SCREEN_H */
