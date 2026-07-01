#include "components.h"
#include "constants.h"
#include "coordinator.h"
#include "debug.h"
#include "raylib.h"
#include "systems/movement_system.h"
#include "systems/render_system.h"

int main(void)
{
	LOG(L_INFO, "Hello brave new world!");
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Shooter");
	SetTargetFPS(FPS);

    CoordinatorInit();

	REGISTER_COMPONENT(Position, COMPONENT_POSITION);
	REGISTER_COMPONENT(Velocity, COMPONENT_VELOCITY);
	REGISTER_COMPONENT(Hitbox, COMPONENT_HITBOX);
	REGISTER_COMPONENT(Render, COMPONENT_RENDER);

	MovementSystem *movementSystem = MovementSystemCreate();
	RenderSystem *renderSystem = RenderSystemCreate();

	Entity player = CoordinatorCreateEntity();

	CoordinatorAddComponent(player, COMPONENT_POSITION,
				&POSITION(10, 10));
	CoordinatorAddComponent(player, COMPONENT_VELOCITY,
				&VELOCITY(100, 20));
	CoordinatorAddComponent(player, COMPONENT_HITBOX,
				&HITBOX(30, 30));
	CoordinatorAddComponent(
		player, COMPONENT_RENDER,
		&((Render){ .renderColor = BLUE, .renderMode = RENDER_COLOR }));

	while (!WindowShouldClose()) {
		float dt = GetFrameTime();
		BeginDrawing();
		ClearBackground(RAYWHITE);
		movementSystem->update(movementSystem, dt);
		renderSystem->update(renderSystem, dt);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
