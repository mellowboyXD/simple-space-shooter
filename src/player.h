#ifndef _PLAYER_H
#define _PLAYER_H

#include "raylib.h"

typedef struct {
        Vector2 pos;
        int width;
        int height;
        int speed;
        Color color;
} Player;

void InitPlayer(Player *player);
void DrawPlayer(Player player);
void UpdatePlayer(Player *player, float dt);

#endif // _PLAYER_H
