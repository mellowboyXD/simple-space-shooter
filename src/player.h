#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"

void InitPlayer(Player *player);
void DrawPlayer(Player player);
void UpdatePlayer(GameState *gameState, float dt);

#endif // PLAYER_H
