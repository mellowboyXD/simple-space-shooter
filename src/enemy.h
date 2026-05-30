/**
 * Enemy battle happens in 3 phases:
 *      Phase 1: PHASE_REG, regular battle phase
 *              - fighters
 *              - after fighters are cleared, begin next phase
 *      Phase 2: PHASE_BOSS, boss battle phase
 *              - mothership boss battle
 *              - after mothership's hp is about 1/4 left, begin next phase
 *      Phase 3: PHASE_RAGE, boss battle conclusion
 *              - mothership + fighter support
 *              - final phase before beginning next level
 */

#ifndef _ENEMY_H
#define _ENEMY_H

typedef enum {EC_FIGHTER, EC_MOTHERSHIP} EnemyClass;

typedef enum {PHASE_REG, PHASE_BOSS, PHASE_RAGE} EnemyPhase;

typedef struct {
        EnemyClass class;
        float hp;
        float attack;
        float shield;
        void (*specialAttack) (Player *player); // a special boost attack
} Enemy;

extern Enemy enemyPool[MAX_REGULAR_ENEMY + 1]; // Regular enemies + 1 boss

#endif // _ENEMY_H
