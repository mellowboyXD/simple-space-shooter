#include "constants.h"
#include "debug.h"
#include "game.h"

int main(void)
{
	LOG(L_INFO, "Hello brave new world!");

	int configFlags = FLAG_VSYNC_HINT;
#ifndef DEBUG
	configFlags |= FLAG_FULLSCREEN_MODE;
#endif

	SetConfigFlags(configFlags);

	InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
		   "Space Shooter");
	SetTargetFPS(TARGET_FPS);

	GameData gameData = { 0 };
	GameInit(&gameData);

	while (!WindowShouldClose()) {
		float dt = GetFrameTime();

		// update game
		GameUpdate(&gameData, dt);

		// draw
		GameDraw(&gameData);
	}

	GameDeinit(&gameData);
	CloseWindow();
	return 0;
}
