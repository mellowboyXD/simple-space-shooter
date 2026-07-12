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
void ScreenDraw(ScreenData *screen);
int ScreenGetVirtualWidth(ScreenData *screen);
int ScreenGetVirtualHeight(ScreenData *screen);
RenderTexture2D ScreenGetRenderTexture(ScreenData *screen);

#endif /* SCREEN_H */
