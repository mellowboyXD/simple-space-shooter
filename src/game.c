#include "game.h"
#include "constants.h"
#include "coordinator.h"
#include "systems_pool.h"
#include "ui_components.h"
#include "systems/movement_system.h"
#include "systems/render_system.h"
#include "systems/ui_callback_system.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>

static void _RegisterComponents(GameData *gameData)
{
	REGISTER_COMPONENT(Position, COMPONENT_POSITION);
	REGISTER_COMPONENT(Velocity, COMPONENT_VELOCITY);
	REGISTER_COMPONENT(Hitbox, COMPONENT_HITBOX);
	REGISTER_COMPONENT(Render, COMPONENT_RENDER);
	REGISTER_COMPONENT(UIMouseInputState, COMPONENT_UI_MOUSE_INPUT_STATE);
	REGISTER_COMPONENT(UICallback, COMPONENT_UI_CALLBACK);
}

static void _CreateSystems(GameData *gameData)
{
	SystemsPoolInit(&gameData->systemsPool);
	SystemsPoolAddSystem(&gameData->systemsPool, MovementSystemCreate());
	SystemsPoolAddSystem(&gameData->systemsPool, RenderSystemCreate());
	SystemsPoolAddSystem(&gameData->systemsPool, UICallbackSystemCreate());
}

void GameInit(GameData *gameData)
{
	CoordinatorInit();

	_RegisterComponents(gameData);
	_CreateSystems(gameData);

	Entity global = CoordinatorCreateEntity();
	Entity player = CoordinatorCreateEntity();
	gameData->player = player;
	gameData->global = global;

	CoordinatorAddComponent(player, COMPONENT_POSITION, &POSITION(10, 10));
	CoordinatorAddComponent(player, COMPONENT_VELOCITY, &VELOCITY(100, 20));
	CoordinatorAddComponent(player, COMPONENT_HITBOX, &HITBOX(30, 30));
	CoordinatorAddComponent(player, COMPONENT_RENDER, &RENDER_C(BLUE));

	CoordinatorAddComponent(global, COMPONENT_UI_MOUSE_INPUT_STATE,
				&UI_MOUSE_INPUT_STATE(false));
	CoordinatorAddComponent(global, COMPONENT_UI_CALLBACK,
				&UICALLBACK(NULL));

	gameData->screen = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    gameData->renderHeight = SCREEN_HEIGHT;
    gameData->renderWidth = SCREEN_WIDTH;
}

void GameDeinit(GameData *gameData)
{
	CoordinatorDeinit();
	UnloadRenderTexture(gameData->screen);
}

void GameUpdateSystems(GameData *gameData, float dt)
{
	for (size_t i = 0; i < gameData->systemsPool.count; i++) {
		System *sys = SystemsPoolGetSystem(&gameData->systemsPool, i);
		assert(sys != NULL && "System is null");
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
