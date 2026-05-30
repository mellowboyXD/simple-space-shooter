#include "bullet.h"

void InitBullets(Bullet bulletPool[MAX_BULLETS])
{
        for (int i = 0; i < MAX_BULLETS; i++) {
                bulletPool[i].width = BULLET_WIDTH;
                bulletPool[i].height = BULLET_HEIGHT;
                bulletPool[i].speed = BULLET_SPEED;
                bulletPool[i].pos.x = 0;
                bulletPool[i].pos.y = 0;
                bulletPool[i].active = false;
                bulletPool[i].color = RED;
                bulletPool[i].owner = BO_NONE;
        }
}

void DrawBullets(Bullet bulletPool[MAX_BULLETS])
{
        for (int i = 0; i < MAX_BULLETS; i++) {
                if (bulletPool[i].active) {
                        float x = bulletPool[i].pos.x;
                        float y = bulletPool[i].pos.y;
                        DrawRectangle(x, y, bulletPool[i].width, 
                                        bulletPool[i].height, 
                                        bulletPool[i].color);
                }
        }
}

void UpdateBullets(GameState *gameState, float dt)
{
        Bullet *bulletPool = gameState->bulletPool;

        for (int i = 0; i < MAX_BULLETS; i++) {
                if (bulletPool[i].active) {
                        if (bulletPool[i].owner == BO_PLAYER)
                                bulletPool[i].pos.y -= bulletPool[i].speed * dt;
                        else if (bulletPool[i].owner == BO_ENEMY)
                                bulletPool[i].pos.y += bulletPool[i].speed * dt;
                }

                if (bulletPool[i].pos.y < 0)
                        bulletPool[i].active = false;
        }
}
