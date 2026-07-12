#include "constants.h"
#include "debug.h"
#include "game.h"

int main(void)
{
	LOG(L_INFO, "Hello brave new world!");

    // enable vsync and fullscreen
    SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_VSYNC_HINT );

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Shooter");
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
