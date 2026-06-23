#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAXLEN 256              /* Max string length */

/* -- Screen & FPS -- */
#define SCREEN_WIDTH 480        /* Screen width */
#define SCREEN_HEIGHT 640       /* Screen height */
#define FPS 60                  /* Target FPS */
#define UI_PANEL_RATIO 0.25     /* Ratio of the UI panel to the screen width */

/* -- Player -- */
#define PLAYER_WIDTH 40         /* Player width */
#define PLAYER_HEIGHT 60        /* Player height */
#define PLAYER_SPEED 280        /* Player speed */

/* -- Bullet -- */
#define BULLET_WIDTH 10
#define BULLET_HEIGHT 10
#define BULLET_SPEED 400
#define MAX_BULLETS 50
#define DEFAULT_FIRE_RATE 0.3f

/* -- Enemies -- */
#define MAX_ENEMIES 6           /* 5 enemies and 1 boss */

#define EC_FIGHTER_WIDTH 20
#define EC_FIGHTER_HEIGHT 40
#define EC_FIGHTER_HP 100
#define EC_FIGHTER_ATK 120
#define EC_FIGHTER_SHIELD 200

#define EC_MOTHERSHIP_WIDTH 60
#define EC_MOTHERSHIP_HEIGHT 80
#define EC_MOTHERSHIP_HP 200
#define EC_MOTHERSHIP_ATK 150
#define EC_MOTHERSHIP_SHIELD 250

/* -- Utility Fallbacks -- */
#define FALLBACK_GAP 5

#endif /* CONSTANTS_H */
