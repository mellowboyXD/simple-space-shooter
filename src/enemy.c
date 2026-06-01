#include "enemy.h"
#include "constants.h"
#include "debug.h"
#include "utils.h"

void InitEnemies(Enemy enemyPool[MAX_ENEMIES])
{
        int gap = Gap(EC_FIGHTER_WIDTH, MAX_ENEMIES - 1);
        if (gap == -1) {
                LOG(L_ERROR, "Cannot determine a suitable gap to have all \
                                enemies equally spaced");
                gap = FALLBACK_GAP;
        }

        int i;
        for (i = 0; i < MAX_ENEMIES - 1; i++) {
                enemyPool[i].width = EC_FIGHTER_WIDTH;
                enemyPool[i].height = EC_FIGHTER_HEIGHT;
                enemyPool[i].pos.x = 10 + i * gap;
                enemyPool[i].pos.y = 10;
                enemyPool[i].color = YELLOW;
                enemyPool[i].class = EC_FIGHTER;
                enemyPool[i].hp = EC_FIGHTER_HP;
                enemyPool[i].attack = EC_FIGHTER_ATK;
                enemyPool[i].shield = EC_FIGHTER_SHIELD;
                enemyPool[i].isAlive = true;
        }

        // boss
        enemyPool[i].pos.x = GetScreenMiddleX(enemyPool[i].width);
        enemyPool[i].pos.y = 10;
        enemyPool[i].width = EC_MOTHERSHIP_WIDTH;
        enemyPool[i].height = EC_MOTHERSHIP_HEIGHT;
        enemyPool[i].color = ORANGE;
        enemyPool[i].class = EC_MOTHERSHIP;
        enemyPool[i].hp = EC_MOTHERSHIP_HP;
        enemyPool[i].attack = EC_MOTHERSHIP_ATK;
        enemyPool[i].shield = EC_MOTHERSHIP_SHIELD;
        enemyPool[i].isAlive = true;
}

void DrawEnemies(Enemy enemyPool[MAX_ENEMIES])
{
        for (int i = 0; i < MAX_ENEMIES; i++) {
                float x = enemyPool[i].pos.x;
                float y = enemyPool[i].pos.y;
                int width = enemyPool[i].width;
                int height = enemyPool[i].height;
                // TODO: Draw enemies based on enemyPhase
                if (enemyPool[i].isAlive)
                        DrawRectangle(x, y, width, height, enemyPool[i].color);
        }
}

void UpdateEnemies(GameState *gameState, float dt)
{
        // TODO: implement enemy AI
}
