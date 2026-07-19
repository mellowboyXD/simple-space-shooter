#ifndef SCREEN_H
#define SCREEN_H

#include "raylib.h"

typedef struct {
	int renderWidth;
	int renderHeight;
	RenderTexture2D target;
	bool initCalled;
	int padding[4]; /* specify top, right, bottom and left margins in this order */
} ScreenData;

typedef enum {
	PADDING_TOP,
	PADDING_RIGHT,
	PADDING_BOTTOM,
	PADDING_LEFT,
    PADDING_ALL
} PaddingDirection;

void ScreenInit(ScreenData *screen, int width, int height);
void ScreenDeinit(ScreenData *screen);
void ScreenDrawTarget(ScreenData *screen);
RenderTexture2D ScreenGetRenderTexture(ScreenData *screen);
int ScreenGetVirtualWidth(ScreenData *screen);
int ScreenGetVirtualHeight(ScreenData *screen);
void ScreenSetPadding(ScreenData *screen, PaddingDirection direction,
		      int value);

#endif /* SCREEN_H */
