#include "enemy.h"
#include "collision.h"
#include "constants.h"
#include "debug.h"
#include "utils.h"

#include <math.h>
#include <stdlib.h>
#include <time.h>

void MoveEnemy(Enemy *enemy, float dt);
int RandomInt(int start, int end);
float RandomFloat(float min, float max);
void SpawnSineEnemy(Enemy *e, float baseX, float startY);

void InitEnemies(Enemy enemyPool[MAX_ENEMIES])
{
        srand(time(NULL));
        int gap = Gap(EC_FIGHTER_WIDTH, MAX_ENEMIES - 1);
        if (gap == -1) {
                LOG(L_ERROR, "Cannot determine a suitable gap to have all \
                                enemies equally spaced");
                gap = FALLBACK_GAP;
        }

        int i;
        for (i = 0; i < MAX_ENEMIES - 1; i++) {
                enemyPool[i].id = i;
                enemyPool[i].width = EC_FIGHTER_WIDTH;
                enemyPool[i].height = EC_FIGHTER_HEIGHT;
                enemyPool[i].pos.x = gap + i * (EC_FIGHTER_WIDTH + gap);
                enemyPool[i].pos.y = 10;
                enemyPool[i].color = YELLOW;
                enemyPool[i].class = EC_FIGHTER;
                enemyPool[i].hp = EC_FIGHTER_HP;
                enemyPool[i].attack = EC_FIGHTER_ATK;
                enemyPool[i].shield = EC_FIGHTER_SHIELD;
                enemyPool[i].isAlive = true;
                SpawnSineEnemy(enemyPool + i, enemyPool[i].pos.x, 10);
        }

        // boss
        SpawnSineEnemy(enemyPool + i, enemyPool[i].pos.x, 10);
        enemyPool[i].id = i;
        enemyPool[i].pos.x = GamePanelMiddleX(enemyPool[i].width);
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

        Bullet *bulletPool = gameState->bulletPool;
        Enemy *enemyPool = gameState->enemyPool;
        for (int i = 0; i < MAX_ENEMIES; i++) {
                if (!enemyPool[i].isAlive)
                        continue;

                Enemy *enemy = enemyPool + i;
                for (int j = 0; j < MAX_BULLETS; j++) {
                        if (!bulletPool[j].active)
                                continue;

                        Bullet *bullet = bulletPool + j;
                        if (isCollision(enemy, bullet)) {
                                enemy->isAlive = false;
                                bullet->active = false;
                        }
                }

                MoveEnemy(enemy, dt);
        }
}

void MoveEnemy(Enemy *e, float dt)
{
        if (e->pos.y > GamePanelHeight() / 2.0f)
                e->velY *= -1;
        else if (e->pos.y < 0)
                e->velY *= -1;

        e->pos.y += e->velY * dt;
        e->pos.x += e->amp * sin(e->pos.y * e->freq + e->phase) * dt;
}

void SpawnSineEnemy(Enemy *e, float baseX, float startY)
{
    e->baseX = baseX;
    e->pos = (Vector2){baseX, startY};
    e->velY = 80.0f + RandomFloat(-15.0f, 15.0f);
    e->phase = RandomFloat(0.0f, 6.283f);  // 0 to 2π
    e->amp = 40.0f + RandomFloat(-10.0f, 10.0f);
    e->freq = 0.05f + RandomFloat(-0.01f, 0.01f);
}

int RandomInt(int start, int end)
{
        return start + rand() % (end - start + 1); /* biased; consider other ways */
}

float RandomFloat(float min, float max)
{
        return min + (float)rand() / RAND_MAX * (max - min);
}
