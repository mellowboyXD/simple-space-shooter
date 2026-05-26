#include "ammo.h"
#include "constants.h"
#include "raylib.h"

void InitAmmo(Ammo *ammo)
{
        ammo->width = BULLET_WIDTH;
        ammo->height = BULLET_HEIGHT;
        ammo->speed = BULLET_SPEED;
        ammo->pos.x = 0;
        ammo->pos.y = 0;
        ammo->active = false;
        ammo->color = RED;
}

void DrawAmmo(Ammo ammo)
{
        float x = ammo.pos.x;
        float y = ammo.pos.y;
        DrawRectangle(x, y, ammo.width, ammo.height, ammo.color);
}

void UpdateAmmo(Ammo *ammo, float dt)
{
        if (ammo->active)
                ammo->pos.y -= ammo->speed * dt;
}
