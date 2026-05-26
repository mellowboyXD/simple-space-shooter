#include "ammo.h"
#include "constants.h"
#include "player.h"
#include "raylib.h"

#define max(a, b) a > b ? a : b
#define min(a, b) a < b ? a : b

static void Shoot(Ammo *ammo, Player player);

static Ammo ammo = {0};

void InitPlayer(Player *player)
{
        player->width = PLAYER_WIDTH;
        player->height = PLAYER_HEIGHT;
        player->pos.x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
        player->pos.y = SCREEN_HEIGHT - PLAYER_HEIGHT - 20;
        player->speed = PLAYER_SPEED;
        player->color = BLUE;
        
        InitAmmo(&ammo);
}

void DrawPlayer(Player player)
{
        float x = player.pos.x;
        float y = player.pos.y;

        DrawAmmo(ammo);
        DrawRectangle(x, y, player.width, player.height, player.color);
}

void UpdatePlayer(Player *player, float dt)
{
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

        if (IsKeyDown(KEY_SPACE)) {
                Shoot(&ammo, *player);
        }

        UpdateAmmo(&ammo, dt);
}


static void Shoot(Ammo *ammo, Player player)
{
        ammo->pos.x = (player.pos.x + player.width / 2) - ammo->width / 2;
        ammo->pos.y = player.pos.y;
        ammo->active = true;
}
