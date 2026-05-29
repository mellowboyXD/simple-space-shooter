#ifndef _PLAYER_H
#define _PLAYER_H

#include "bullet.h"
#include "raylib.h"

typedef struct {
        Vector2 pos;
        int width;
        int height;
        int speed;
        float fireRate;
        int ammoIdx;
        int ammoCount;
        Color color;
} Player;

void InitPlayer(Player *player);
void DrawPlayer(Player player);
void UpdatePlayer(Player *player, float dt);
Bullet *InitAmmo(int size);
void DeInitPlayer(Player *player);

#endif // _PLAYER_H
