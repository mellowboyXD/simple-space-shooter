#include "game.h"
#include "constants.h"
#include "coordinator.h"
#include "raylib.h"
#include "systems/movement_system.h"
#include "systems/render_system.h"
#include "systems/system.h"
#include <math.h>
#include <stddef.h>

void GameInit(GameData *gameData)
{
	CoordinatorInit();

	REGISTER_COMPONENT(Position, COMPONENT_POSITION);
	REGISTER_COMPONENT(Velocity, COMPONENT_VELOCITY);
	REGISTER_COMPONENT(Hitbox, COMPONENT_HITBOX);
	REGISTER_COMPONENT(Render, COMPONENT_RENDER);

	Entity player = CoordinatorCreateEntity();
	gameData->player = player;

	CoordinatorAddComponent(player, COMPONENT_POSITION, &POSITION(10, 10));
	CoordinatorAddComponent(player, COMPONENT_VELOCITY, &VELOCITY(100, 20));
	CoordinatorAddComponent(player, COMPONENT_HITBOX, &HITBOX(30, 30));
	CoordinatorAddComponent(player, COMPONENT_RENDER, &RENDER_C(BLUE));

	gameData->screen = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
	gameData->devPanel = (Rectangle){ 0, 0, 350, 300 };
}

void GameCreateSystems(GameData *gameData)
{
	gameData->systems[0] = MovementSystemCreate();
	gameData->systems[1] = RenderSystemCreate();
}

void GameUpdateSystems(GameData *gameData, float dt)
{
	for (size_t i = 0; i < MAX_SYSTEMS_TYPE; i++) {
		System *sys = gameData->systems[i];
		sys->update(sys, dt);
	}
}

void GameDrawScreen(RenderTexture2D *screen)
{
	float scale = fminf((float)GetScreenWidth() / SCREEN_WIDTH,
			    (float)GetScreenHeight() / SCREEN_HEIGHT);

	float destWidth = SCREEN_WIDTH * scale;
	float destHeight = SCREEN_HEIGHT * scale;

	Rectangle src = { 0, 0, SCREEN_WIDTH, -SCREEN_HEIGHT }; // y-flip
	Rectangle dest = { (GetScreenWidth() - destWidth) * 0.5f,
			   (GetScreenHeight() - destHeight) * 0.5f, destWidth,
			   destHeight };
	Vector2 origin = { 0, 0 };

	ClearBackground(BLACK); // paint the letterbox bars
	DrawTexturePro(screen->texture, src, dest, origin, 0.0f, WHITE);
}
