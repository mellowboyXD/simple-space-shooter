#include "bullet.h"
#include "types.h"
#include "player.h"

void InitGameState(GameState *gameState);
void MainDraw(GameState *gameState);
void MainUpdate(GameState *gameState, float dt);

int main(void)
{
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Shooter");
        SetTargetFPS(FPS);

        GameState gameState = { 0 };

        InitGameState(&gameState);

        while (!WindowShouldClose()) {
                float dt = GetFrameTime();
                MainDraw(&gameState);
                MainUpdate(&gameState, dt);
        }

        CloseWindow();
        return 0;
}

void InitGameState(GameState *gameState)
{
        gameState->currentState = STATE_PLAY;
        InitPlayer(&gameState->player);
        InitBullets(gameState->bulletPool);
}

void MainDraw(GameState *gameState) 
{
        BeginDrawing();

        ClearBackground(BLACK);
        DrawFPS(0, 0);
        DrawBullets(gameState->bulletPool);
        DrawPlayer(gameState->player);

        EndDrawing();
}

void MainUpdate(GameState *gameState, float dt)
{
        UpdatePlayer(gameState, dt);
        UpdateBullets(gameState, dt);
}

