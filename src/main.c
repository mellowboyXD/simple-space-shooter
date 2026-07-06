#include "constants.h"
#include "debug.h"
#include "game.h"
#include "raylib.h"

int main(void)
{
	LOG(L_INFO, "Hello brave new world!");
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Shooter");
	SetTargetFPS(TARGET_FPS);
    ToggleFullscreen();

	GameData gameData = { 0 };
	GameInit(&gameData);

	GameCreateSystems(&gameData);

	while (!WindowShouldClose()) {
		float dt = GetFrameTime();

		BeginTextureMode(gameData.screen);
		ClearBackground(RAYWHITE);
		GameUpdateSystems(&gameData, dt);
		EndTextureMode();

		BeginDrawing();
        GameDrawScreen(&gameData.screen);
#if DEBUG
		DrawFPS(0, 0);
#endif
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
