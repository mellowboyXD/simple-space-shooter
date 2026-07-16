#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include "game.h"

#define UI_MOUSE_INPUT_STATE(pressed) ((UIMouseInputState){ (pressed) })
typedef struct {
	bool isPressed;
} UIMouseInputState;

#define UICALLBACK(onClick) ((UICallback){ (onClick) })
typedef struct {
	void (*onClick)(GameData *gameData, Entity self);
} UICallback;

#endif /* UI_COMPONENTS_H */
