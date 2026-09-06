#ifndef GAME_H
#define GAME_H

#include "screen.h"
#include "systems_pool.h"

typedef struct {
	SystemsPool systemsPool;
	Entity player;
	ScreenData screen;
} GameData;

void GameInit(GameData *gameData);
void GameDeinit(GameData *gameData);
void GameUpdate(GameData *gameData, float dt);
void GameDraw(GameData *gameData);

#endif /* GAME_H */
