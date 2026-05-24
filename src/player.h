#ifndef _PLAYER_H
#define _PLAYER_H

#include "constants.h"
#include "raylib.h"

typedef struct {
        Vector2 pos;
        int width;
        int height;
        int speed;
} Player;

void InitPlayer(Player *player);

void DrawPlayer(Player player);

void UpdatePlayer(Player *player, float dt);

#endif // _PLAYER_H
