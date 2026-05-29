#include "bullet.h"
#include "constants.h"
#include "raylib.h"

void InitBullet(Bullet *bullet)
{
        bullet->width = BULLET_WIDTH;
        bullet->height = BULLET_HEIGHT;
        bullet->speed = BULLET_SPEED;
        bullet->pos.x = 0;
        bullet->pos.y = 0;
        bullet->active = false;
        bullet->color = RED;
}

void DrawBullet(Bullet bullet)
{
        float x = bullet.pos.x;
        float y = bullet.pos.y;
        if (bullet.active)
                DrawRectangle(x, y, bullet.width, bullet.height, bullet.color);
}

void UpdateBullet(Bullet *bullet, float dt)
{
        if (bullet->active)
                bullet->pos.y -= bullet->speed * dt;

        if (bullet->pos.y < 0)
                bullet->active = false;
}
