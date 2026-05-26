#ifndef _AMMO_H
#define _AMMO_H

#include "raylib.h"

typedef struct {
        Vector2 pos;
        Color color;
        int speed;
        int width;
        int height;
        bool active;
} Ammo;

void InitAmmo(Ammo *ammo);
void DrawAmmo(Ammo ammo);
void UpdateAmmo(Ammo *ammo, float dt);

#endif // _AMMO_H
