/**
 * This is the game manager, is responsible for controlling the flow of data
 * and orchestrating other managers like screen manager (screen.h) to update its
 * state.
 *
 * It is also responsible for updating all the systems that have been registered
 * as well as registering both systems and components. It is thought of as the 
 * user of the Entity-Component-System.
 */
#include "game.h"
#include "constants.h"
#include "coordinator.h"
#include "debug.h"
#include "raylib.h"
#include "screen.h"
#include "systems_pool.h"
#include "ui_components.h"
#include "systems/movement_system.h"
#include "systems/render_system.h"
#include "systems/ui_callback_system.h"

#include <assert.h>
#include <stddef.h>

static void _RegisterComponents()
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

void _UpdateSystems(GameData *gameData, float dt)
{
	for (size_t i = 0; i < gameData->systemsPool.count; i++) {
		System *sys = SystemsPoolGetSystem(&gameData->systemsPool, i);
		assert(sys != NULL && "System is null");
		sys->update(sys, dt);
	}
}

void _AssociateComponents(GameData *gameData)
{
	Entity player = gameData->player;
	Entity global = gameData->global;

	CoordinatorAddComponent(player, COMPONENT_POSITION, &POSITION(10, 10));
	CoordinatorAddComponent(player, COMPONENT_VELOCITY, &VELOCITY(100, 20));
	CoordinatorAddComponent(player, COMPONENT_HITBOX, &HITBOX(30, 30));
	CoordinatorAddComponent(player, COMPONENT_RENDER, &RENDER_C(BLUE));

	CoordinatorAddComponent(global, COMPONENT_UI_MOUSE_INPUT_STATE,
				&UI_MOUSE_INPUT_STATE(false));
	CoordinatorAddComponent(global, COMPONENT_UI_CALLBACK,
				&UICALLBACK(NULL));
}

void GameInit(GameData *gameData)
{
	CoordinatorInit();

	_RegisterComponents();
	_CreateSystems(gameData);

	Entity global = CoordinatorCreateEntity();
	Entity player = CoordinatorCreateEntity();
	gameData->player = player;
	gameData->global = global;

	_AssociateComponents(gameData);

	// Initialize the screen
	ScreenInit(&gameData->gameTarget, DEFAULT_GAME_TARGET_WIDTH,
		   DEFAULT_GAME_TARGET_HEIGHT);
	ScreenInit(&gameData->uiTarget, DEFAULT_UI_TARGET_WIDTH,
		   DEFAULT_UI_TARGET_HEIGHT);
}

void GameDeinit(GameData *gameData)
{
	CoordinatorDeinit();
	ScreenDeinit(&gameData->gameTarget);
	ScreenDeinit(&gameData->uiTarget);
    SystemsPoolDeinit(&gameData->systemsPool);
}

/**
 * Main update function that updates the available systems.
 */
void GameUpdate(GameData *gameData, float dt)
{
	Position *playerPos =
		GET_COMPONENT(Position, gameData->player, COMPONENT_POSITION);
	if (playerPos->x >
	    (float)ScreenGetVirtualWidth(&gameData->gameTarget)) {
		LOG(L_INFO, "player out of bounds: %f", playerPos->x);
		LOG(L_INFO, "width: %d",
		    ScreenGetVirtualWidth(&gameData->gameTarget));
	}

	// draws to a separate virtual game screen.
	BeginTextureMode(ScreenGetRenderTexture(&gameData->gameTarget));
	ClearBackground(RAYWHITE);
	_UpdateSystems(gameData, dt);
#ifdef DEBUG
#endif
	EndTextureMode();

	// draws to the ui screen; reserve specifically for text and widgets
	BeginTextureMode(ScreenGetRenderTexture(&gameData->uiTarget));
	ClearBackground(BLANK);
#ifdef DEBUG
	int w = ScreenGetVirtualWidth(&gameData->uiTarget);
	int h = ScreenGetVirtualHeight(&gameData->uiTarget);
	DrawFPS(w - 100, h - 88);
#endif
	EndTextureMode();
}

/**
 * Main draw function that gets called repeatedly inside the main loop.
 */
void GameDraw(GameData *gameData)
{
	// draws to screen directly
	BeginDrawing();
	ClearBackground(BLACK);
	ScreenDrawTarget(&gameData->gameTarget);
	ScreenDrawTarget(&gameData->uiTarget);
	EndDrawing();
}
