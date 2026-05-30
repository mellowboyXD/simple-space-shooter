#ifndef BULLET_H
#define BULLET_H

#include "types.h"

void InitBullets(Bullet bulletPool[MAX_BULLETS]);
void DrawBullets(Bullet bulletPool[MAX_BULLETS]);
void UpdateBullets(GameState *gameState, float dt);

#endif // BULLET_H
