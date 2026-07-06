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

	Rectangle *panelRec = &gameData.devPanel;
	bool isDragging = false;
	Vector2 dragOffset = { 0.0f, 0.0f };

	while (!WindowShouldClose()) {
		float dt = GetFrameTime();

		BeginTextureMode(gameData.screen);
		ClearBackground(RAYWHITE);
		GameUpdateSystems(&gameData, dt);
		EndTextureMode();

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
		    CheckCollisionPointRec(GetMousePosition(), *panelRec)) {
			isDragging = true;
			dragOffset.x = GetMouseX() - panelRec->x;
			dragOffset.y = GetMouseY() - panelRec->y;
		} else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			isDragging = false;
		}

		if (isDragging) {
			panelRec->x = GetMouseX() - dragOffset.x;
			panelRec->y = GetMouseY() - dragOffset.y;
		}

		BeginDrawing();
		GameDrawScreen(&gameData.screen);
		DrawFPS(0, 0);
		DEBUG_WINDOW(*panelRec);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
