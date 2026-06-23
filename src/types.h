#ifndef TYPES_H
#define TYPES_H

#include "constants.h"
#include "raylib.h"

typedef enum {STATE_MENU, STATE_PLAY, STATE_PAUSE, STATE_GAMEOVER} State;

typedef enum {BO_NONE, BO_PLAYER, BO_ENEMY} BulletOwner;

typedef enum {EC_FIGHTER, EC_MOTHERSHIP} EnemyClass;

typedef enum {PHASE_REG, PHASE_BOSS, PHASE_RAGE} EnemyPhase;

typedef struct {
        Vector2 pos;
        int width;
        int height;
        int speed;
        float fireRate;
        Color color;
} Player;

typedef struct {
        Vector2 pos;
        Color color;
        int speed;
        int width;
        int height;
        bool active;
        BulletOwner owner;
} Bullet;

typedef struct {
        int id;
        float velY;
        float phase;
        float freq;
        float amp;
        float baseX;
        Vector2 pos;
        int width;
        int height;
        Color color; // may be replaced with image in the future
        EnemyClass class;
        float hp;
        float attack;
        float shield;
        bool isAlive;
        void (*specialAttack) (Player *player); // a special boost attack
} Enemy;


/**
 * GameState is the single source of truth and should be passed around between
 * the different entity updates.
 */
typedef struct {
        Player player;
        Bullet bulletPool[MAX_BULLETS];
        Enemy enemyPool[MAX_ENEMIES];
        State currentState;
        EnemyPhase enemyPhase;
        Rectangle uiPanel;
} GameState;

#endif // TYPES_H
