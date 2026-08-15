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
#include "assets.h"
#include "components.h"
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

const Vector2 gameCameraOffset = { GAME_VIEW_X, GAME_VIEW_Y };

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

static bool _shouldSkipSystemUpdate(SystemsPool *pool, size_t index)
{
	return SystemsPoolGetIndex(pool, UI_CALLBACK_SYSTEM_TYPE) == index ||
	       SystemsPoolGetIndex(pool, RENDER_SYSTEM_TYPE) == index;
}

static void _UpdateSystems(GameData *gameData, float dt)
{
	for (size_t i = 0; i < gameData->systemsPool.count; i++) {
		System *sys =
			SystemsPoolGetSystemByIndex(&gameData->systemsPool, i);
		assert(sys != NULL && "System is null");
		if (_shouldSkipSystemUpdate(&gameData->systemsPool, i)) {
			// skips the update of certain systems as they will be updated
			// manually
			continue;
		}
		sys->update(sys, dt);
	}
}

static void _AssociateComponents(GameData *gameData)
{
	Entity player = gameData->player;
	Entity global = gameData->global;

	CoordinatorAddComponent(player, COMPONENT_POSITION, &POSITION(10, 10));
	CoordinatorAddComponent(player, COMPONENT_VELOCITY, &VELOCITY(100, 20));
	CoordinatorAddComponent(player, COMPONENT_HITBOX, &HITBOX(30, 30));

	AssetId id = CoordinatorLoadAsset(
		"resources/sprites/main_ship/base_full_health.png");
	Rectangle frame = { 0 };
	CoordinatorAddComponent(player, COMPONENT_RENDER, &RENDER_S(id, frame));

	CoordinatorAddComponent(global, COMPONENT_UI_MOUSE_INPUT_STATE,
				&UI_MOUSE_INPUT_STATE(false));
	CoordinatorAddComponent(global, COMPONENT_UI_CALLBACK,
				&UICALLBACK(NULL));

#ifdef DEBUG // verifies that hash map + ref count asset manager are working
	for (int i = 0; i < 10; i++) {
		Entity e = CoordinatorCreateEntity();
		CoordinatorAddComponent(e, COMPONENT_POSITION,
					&POSITION(i * 15, 20));
		CoordinatorAddComponent(e, COMPONENT_HITBOX, &HITBOX(30, 30));
		id = CoordinatorLoadAsset(
			"resources/sprites/main_ship/base_very_damaged.png");
		CoordinatorAddComponent(e, COMPONENT_RENDER,
					&RENDER_S(id, frame));
	}

	AssetLogInfo();
	AssetLogRefCount(id);
#endif
}

static void _RenderGameSystems(GameData *gameData)
{
	RenderSystem *renderSystem = SystemsPoolGetSystem(
		&gameData->systemsPool, RENDER_SYSTEM_TYPE);
	assert(renderSystem &&
	       "RenderSystem not available. Possibly unregistered.");
	renderSystem->update(renderSystem, 0);
}

static void _DrawGameViewport(GameData *gameData)
{
	DrawRectangle(gameCameraOffset.x, gameCameraOffset.y, GAME_VIEW_WIDTH,
		      GAME_VIEW_HEIGHT, RAYWHITE);

#ifdef DEBUG
	DrawRectangle(gameCameraOffset.x, gameCameraOffset.y, 10, 10, BLUE);
	DrawRectangle(gameCameraOffset.x + GAME_VIEW_WIDTH - 10,
		      gameCameraOffset.y + GAME_VIEW_HEIGHT - 10, 10, 10, RED);
#endif

	/* Set clipping mode so systems can't draw outside the viewport. */
	BeginScissorMode(gameCameraOffset.x, gameCameraOffset.y,
			 GAME_VIEW_WIDTH, GAME_VIEW_HEIGHT);
	_RenderGameSystems(gameData);
	EndScissorMode();
}

static void _DrawHUD([[maybe_unused]] GameData *gameData)
{
#ifdef DEBUG
	DrawFPS(VIRTUAL_WIDTH - 80, VIRTUAL_HEIGHT - 20);
#endif
}

/* Public Functions */

void GameInit(GameData *gameData)
{
	CoordinatorInit();

	_RegisterComponents();
	_CreateSystems(gameData);

	Entity global = CoordinatorCreateEntity();
	Entity player = CoordinatorCreateEntity();
	gameData->player = player;
	gameData->global = global;

	LOG(L_INFO, "Entities created successfully.");

	_AssociateComponents(gameData);

	// Initialize the virtual screen
	ScreenInit(&gameData->screen, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
}

void GameDeinit(GameData *gameData)
{
	CoordinatorDeinit();
	ScreenDeinit(&gameData->screen);
	SystemsPoolDeinit(&gameData->systemsPool);
}

/**
 * Main update function that updates the available systems.
 */
void GameUpdate(GameData *gameData, float dt)
{
	/* Handle Input */

	Position *playerPos =
		GET_COMPONENT(Position, gameData->player, COMPONENT_POSITION);
	if (playerPos->x + gameCameraOffset.x > (float)GAME_VIEW_WIDTH) {
		//LOG(L_INFO, "player out of bounds: %f", playerPos->x);
	}
	_UpdateSystems(gameData, dt);
}

/**
 * Main draw function that gets called repeatedly inside the main loop.
 */
void GameDraw(GameData *gameData)
{
	BeginTextureMode(ScreenGetRenderTexture(&gameData->screen));
	ClearBackground(BLACK);
	_DrawGameViewport(gameData);
	_DrawHUD(gameData);
	EndTextureMode();

	// draws to screen directly
	BeginDrawing();
	ClearBackground(BLACK);
	ScreenDrawToWindow(&gameData->screen);
	EndDrawing();
}
