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
#include "components.h"
#include "constants.h"
#include "coordinator.h"
#include "debug.h"
#include "player.h"
#include "raylib.h"
#include "screen.h"
#include "systems/bullet_system.h"
#include "ui_components.h"
#include "systems_pool.h"
#include "systems/collision_system.h"
#include "systems/entity_cleanup_system.h"
#include "systems/kb_input_system.h"
#include "systems/velocity_input_system.h"
#include "systems/movement_system.h"
#include "systems/render_system.h"
#include "systems/ui_callback_system.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

const Vector2 gameCameraOffset = { GAME_VIEW_X, GAME_VIEW_Y };

static void _RegisterComponents()
{
	REGISTER_COMPONENT(Position, COMPONENT_POSITION);
	REGISTER_COMPONENT(Velocity, COMPONENT_VELOCITY);
	REGISTER_COMPONENT(Hitbox, COMPONENT_HITBOX);
	REGISTER_COMPONENT(Render, COMPONENT_RENDER);
	REGISTER_COMPONENT(UIMouseInputState, COMPONENT_UI_MOUSE_INPUT_STATE);
	REGISTER_COMPONENT(UICallback, COMPONENT_UI_CALLBACK);
	REGISTER_COMPONENT(WeaponModifier, COMPONENT_WEAPON_MODIFIER);
	REGISTER_COMPONENT(PlayerInput, COMPONENT_PLAYER_INPUT);
	REGISTER_COMPONENT(PlayerModifiers, COMPONENT_PLAYER_MODIFIERS);

	// tags need registration just like regular components
	REGISTER_COMPONENT(Tag, TAG_PLAYER);
	REGISTER_COMPONENT(Tag, TAG_BULLET);
	REGISTER_COMPONENT(Tag, TAG_FOR_CLEANUP);
}

static void _CreateSystems(GameData *gameData)
{
	SystemsPoolInit(&gameData->systemsPool);
	SystemsPoolAddSystem(&gameData->systemsPool, KBInputSystemCreate());
	SystemsPoolAddSystem(&gameData->systemsPool,
			     VelocityInputSystemCreate());
	SystemsPoolAddSystem(&gameData->systemsPool, BulletSystemCreate());
	SystemsPoolAddSystem(&gameData->systemsPool, CollisionSystemCreate());
	SystemsPoolAddSystem(&gameData->systemsPool, MovementSystemCreate());
	SystemsPoolAddSystem(&gameData->systemsPool, UICallbackSystemCreate());
	SystemsPoolAddSystem(&gameData->systemsPool, RenderSystemCreate());
	SystemsPoolAddSystem(&gameData->systemsPool,
			     EntityCleanUpSystemCreate());
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

	/* Set clipping mode so systems can't draw outside the viewport. */
	BeginScissorMode(gameCameraOffset.x, gameCameraOffset.y,
			 GAME_VIEW_WIDTH, GAME_VIEW_HEIGHT);
	_RenderGameSystems(gameData);
	EndScissorMode();
}

static void _DrawHUD([[maybe_unused]] GameData *gameData)
{
#ifdef DEBUG
	char entityCountText[MAX_STR_LEN] = { 0 };
	sprintf(entityCountText, "entity count: %d\n",
		CoordinatorGetEntityCount());

	DrawFPS(VIRTUAL_WIDTH - 80, VIRTUAL_HEIGHT - 20);
	DrawText(entityCountText, 0, 0, 14, BLUE);
#endif
}

/* Public Functions */

void GameInit(GameData *gameData)
{
	CoordinatorInit();

	_RegisterComponents();
	_CreateSystems(gameData);

	gameData->player = CoordinatorCreateEntity();

#ifdef DEBUG
	Entity d = CoordinatorCreateEntity();
	Hitbox hb = { 40, 40 };
	Render r = { .zIndex = 1,
		     .renderColor = BLUE,
		     .renderMode = RENDER_COLOR,
		     .frame = { 0, 0, 0, 0 },
		     .textureId = 0 };

	CoordinatorAddComponent(d, COMPONENT_HITBOX, &hb);
	CoordinatorAddComponent(d, COMPONENT_POSITION, &POSITION(100, 100));
	CoordinatorAddComponent(d, COMPONENT_RENDER, &r);
#endif // DEBUG

	LOG(L_INFO, "Entities created successfully.");

	PlayerInit(gameData->player);

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
