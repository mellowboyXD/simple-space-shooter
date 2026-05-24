#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include "player.h"
#include "raylib.h"

#define MAXLEN 256              // Max string length

#define SCREEN_WIDTH 480        // Screen width
#define SCREEN_HEIGHT 640       // Screen height

#define PLAYER_WIDTH 40         // Player width
#define PLAYER_HEIGHT 60        // Player height
#define PLAYER_SPEED 280        // Player speed

#define FPS 60                  // Target FPS

typedef enum {STATE_MENU, STATE_PLAY, STATE_PAUSE, STATE_GAMEOVER} State;

typedef struct {
        Player player;
        State currentState;
} GameState;

#endif /* _CONSTANTS_H */
