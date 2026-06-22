#include "player.h"
#include "utils.h"

static void Shoot(Bullet bulletPool[MAX_BULLETS], Player *player);

static float shootTimer = 0.0f;

void InitPlayer(Player *player)
{
        player->width = PLAYER_WIDTH;
        player->height = PLAYER_HEIGHT;
        player->pos.x = GetScreenWidth() / 2 - PLAYER_WIDTH / 2;
        player->pos.y = GetScreenHeight() - PLAYER_HEIGHT - 20;
        player->speed = PLAYER_SPEED;
        player->color = BLUE;
        player->fireRate = DEFAULT_FIRE_RATE;
}

void DrawPlayer(Player player)
{
        float x = player.pos.x;
        float y = player.pos.y;

        DrawRectangle(x, y, player.width, player.height, player.color);
}

void UpdatePlayer(GameState *gameState, float dt)
{
        Player *player = &gameState->player;
        shootTimer += dt;
        float newPos = 0;

        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
                newPos = player->pos.x + player->speed * dt;
                if (newPos + player->width < GetScreenWidth())
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
                if (newPos + player->height < GetScreenHeight())
                        player->pos.y = newPos;
        }

        if (IsKeyDown(KEY_SPACE) && shootTimer >= player->fireRate) {
                Shoot(gameState->bulletPool, player);
                shootTimer = 0.0f;
        }
}

/**
 * Spawns a bullet at the player's location and set it to active.
 */
static void Shoot(Bullet bulletPool[MAX_BULLETS], Player *player)
{
        for (int i = 0; i < MAX_BULLETS; i++) {
                if(!bulletPool[i].active) {
                        bulletPool[i].pos.x = 
                                (player->pos.x + player->width / 2) 
                                - bulletPool[i].width / 2;
                        bulletPool[i].pos.y = player->pos.y;
                        bulletPool[i].active = true;
                        bulletPool[i].owner = BO_PLAYER;
                        break;
                }
        }
}

