#include "debug.h"
#include "raylib.h"
#include "types.h"

int main(void)
{
	LOG(L_INFO, "Hello brave new world!");
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Shooter");
	SetTargetFPS(FPS);

	GameState gameState = { 0 };

	while (!WindowShouldClose()) {
		float dt = GetFrameTime();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
	}

	CloseWindow();
	return 0;
}
