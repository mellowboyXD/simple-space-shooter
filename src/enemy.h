/**
 * Enemy battle happens in 3 phases:
 *      Phase 1: PHASE_REG, regular battle phase
 *              - fighters
 *              - after fighters are cleared, begin next phase
 *      Phase 2: PHASE_BOSS, boss battle phase
 *              - mothership boss battle
 *              - after mothership's hp is about 1/4 left, begin next phase
 *      Phase 3: PHASE_RAGE, boss battle conclusion
 *              - mothership + fighter support
 *              - final phase before beginning next level
 */

#ifndef ENEMY_H
#define ENEMY_H

#include "types.h"

void InitEnemies(Enemy *enemyPool[MAX_ENEMIES]);
void DrawEnemies(Enemy *enemyPool[MAX_ENEMIES]);
void UpdateEnemies(GameState *gameState, float dt);

extern Enemy enemyPool[MAX_ENEMIES]; // Regular enemies + 1 boss

#endif // ENEMY_H
