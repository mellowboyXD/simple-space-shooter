#include <stdio.h>

#include "constants.h"
#include "raylib.h"
#include "player.h"

void InitGameState(GameState *gameState);
void MainDraw(GameState gameState);
void MainUpdate(GameState *gameState, float dt);
void DeInitGameState(GameState *gameState);

int main(void)
{
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Shooter");
        SetTargetFPS(FPS);

        GameState gameState = { 0 };

        InitGameState(&gameState);

        while (!WindowShouldClose()) {
                float dt = GetFrameTime();
                MainDraw(gameState);
                MainUpdate(&gameState, dt);
        }

        DeInitGameState(&gameState);
        CloseWindow();
        return 0;
}

void InitGameState(GameState *gameState)
{
        gameState->currentState = STATE_PLAY;
        InitPlayer(&gameState->player);
}

void DeInitGameState(GameState *gameState)
{
        DeInitPlayer(&gameState->player);
}

void MainDraw(GameState gameState) 
{
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(0, 0);
        DrawPlayer(gameState.player);
        EndDrawing();
}

void MainUpdate(GameState *gameState, float dt)
{
        UpdatePlayer(&gameState->player, dt);
}

