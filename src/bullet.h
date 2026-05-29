#ifndef _BULLET_H
#define _BULLET_H

#include "raylib.h"

typedef struct {
        Vector2 pos;
        Color color;
        int speed;
        int width;
        int height;
        bool active;
} Bullet;

void InitBullet(Bullet *bullet);
void DrawBullet(Bullet bullet);
void UpdateBullet(Bullet *bullet, float dt);

#endif // _BULLET_H
