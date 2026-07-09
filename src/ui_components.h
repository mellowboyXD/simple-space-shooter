#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include "game.h"

#define UIMOUSE_INPUT_STATE(pressed, hovered, focused, clicked) \
	((UIMouseInputState){ (pressed), (hovered), (focused), (clicked) })
typedef struct {
	bool isPressed;
	bool isHovered;
	bool isFocused;
	bool isClickedThisFrame;
} UIMouseInputState;

typedef struct {
	void (*onClick)(GameData *gameData, Entity self);
} UICallback;

#endif /* UI_COMPONENTS_H */
