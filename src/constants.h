#ifndef CONSTANTS_H
#define CONSTANTS_H

constexpr int MAX_STR_LEN = 256;

constexpr int DEFAULT_WINDOW_WIDTH = 1600;
constexpr int DEFAULT_WINDOW_HEIGHT = 900;

constexpr int VIRTUAL_WIDTH = 640;
constexpr int VIRTUAL_HEIGHT = 480;

constexpr int GAME_VIEW_X = 16; /* left margin */
constexpr int GAME_VIEW_Y = 16; /* top margin */
constexpr int GAME_VIEW_WIDTH = 384;
constexpr int GAME_VIEW_HEIGHT = 448;

constexpr int HUD_X = GAME_VIEW_X + GAME_VIEW_WIDTH;
constexpr int HUD_Y = GAME_VIEW_Y;
constexpr int HUD_WIDTH = VIRTUAL_WIDTH - HUD_X;
constexpr int HUD_HEIGHT = GAME_VIEW_HEIGHT;

constexpr int TARGET_FPS = 150;

constexpr float PLAYER_SPEED = 225.8f;
constexpr float BULLET_SPEED = 255.5f;
constexpr float NORMAL_FIRE_RATE = 0.2f;
#endif /* CONSTANTS_H */
