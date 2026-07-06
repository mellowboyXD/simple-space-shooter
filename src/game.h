#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "systems/system.h"

typedef struct {
    System *systems[MAX_SYSTEMS_TYPE];
    Entity player;
    RenderTexture2D screen;
} GameData;

void GameInit(GameData *gameData);
void GameCreateSystems(GameData *gameData);
void GameUpdateSystems(GameData *gameData, float dt);
void GameDrawScreen(RenderTexture2D *screen);

#endif /* GAME_H */
