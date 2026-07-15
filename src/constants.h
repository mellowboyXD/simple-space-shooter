#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>
#define MAXLEN 256              /* Max string length */

/* -- Screen & FPS -- */
#define SCREEN_WIDTH 1600        /* Screen width */
#define SCREEN_HEIGHT 900       /* Screen height */
#define UI_PANEL_RATIO 0.25     /* Ratio of the UI panel to the screen width */

constexpr int DEFAULT_UI_TARGET_WIDTH = 960;
constexpr int DEFAULT_UI_TARGET_HEIGHT = 540;

constexpr int DEFAULT_GAME_TARGET_WIDTH = 480;
constexpr int DEFAULT_GAME_TARGET_HEIGHT = 640;

constexpr uint8_t TARGET_FPS = 150;

#endif /* CONSTANTS_H */
