#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "systems_pool.h"

typedef struct {
    SystemsPool systemsPool;
    Entity player;
    Entity global;
    RenderTexture2D screen;
    float renderWidth;
    float renderHeight;
} GameData;

void GameInit(GameData *gameData);
void GameDeinit(GameData *gameData);
void GameUpdateSystems(GameData *gameData, float dt);
void GameDrawScreen(RenderTexture2D *screen);

#endif /* GAME_H */
