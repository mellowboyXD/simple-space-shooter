#include <stdlib.h>
#include <stdio.h>

#include "bullet.h"
#include "constants.h"
#include "player.h"
#include "raylib.h"

#define max(a, b) a > b ? a : b
#define min(a, b) a < b ? a : b

static void Shoot(Bullet *bullet, Player player);

static Bullet *ammo = NULL;
static float shootTimer = 0.0f;

void InitPlayer(Player *player)
{
        player->width = PLAYER_WIDTH;
        player->height = PLAYER_HEIGHT;
        player->pos.x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
        player->pos.y = SCREEN_HEIGHT - PLAYER_HEIGHT - 20;
        player->speed = PLAYER_SPEED;
        player->color = BLUE;
        player->fireRate = DEFAULT_FIRE_RATE;
        player->ammoIdx = 0;
        player->ammoCount = DEFAULT_AMMO_COUNT;

        ammo = InitAmmo(player->ammoCount);
        if (!ammo) {
                fprintf(stderr, "[ERROR]: Could not initialize ammo.");
                exit(1);
        }

        for (int i = 0; i < player->ammoCount; i++)
                InitBullet(ammo + i);
}

void DrawPlayer(Player player)
{
        float x = player.pos.x;
        float y = player.pos.y;

        
        for (int i = 0; i < player.ammoCount; i++)
                DrawBullet(ammo[i]);

        DrawRectangle(x, y, player.width, player.height, player.color);
}

void UpdatePlayer(Player *player, float dt)
{
        shootTimer += dt;
        float newPos = 0;

        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
                newPos = player->pos.x + player->speed * dt;
                if (newPos + player->width < SCREEN_WIDTH)
                        player->pos.x = newPos;
        }

        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                newPos = player->pos.x - player->speed * dt;
                player->pos.x = max(0, newPos);
        }

        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
                newPos = player->pos.y - player->speed * dt;
                player->pos.y = max(0, newPos);
        }

        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
                newPos = player->pos.y + player->speed * dt;
                if (newPos + player->height < SCREEN_HEIGHT)
                        player->pos.y = newPos;
        }

        if (IsKeyDown(KEY_SPACE) && shootTimer >= player->fireRate) {
                Shoot(ammo + player->ammoIdx, *player);
                player->ammoIdx = (player->ammoIdx + 1) % player->ammoCount;
                shootTimer = 0.0f;
        }

        for (int i = 0; i < player->ammoCount; i++)
                UpdateBullet(ammo + i, dt);
}

/**
 * Spawns a bullet at the player's location and set it to active.
 */
static void Shoot(Bullet *bullet, Player player)
{
        bullet->pos.x = (player.pos.x + player.width / 2) - bullet->width / 2;
        bullet->pos.y = player.pos.y;
        bullet->active = true;
}

/**
 * Allocates n bullets and returns a pointer to the begining of the list
 */
Bullet *InitAmmo(int n) 
{
        if (n < 1)
                return NULL;

        return malloc(sizeof(Bullet) * n);
}


/**
 * Frees any allocated resources.
 */
void DeInitPlayer(Player *player)
{
        player->ammoIdx = 0;
        free(ammo);
}
