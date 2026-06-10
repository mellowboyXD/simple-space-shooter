#include "collision.h"

bool isCollision(Enemy *me, Bullet *you) {
        return (me->pos.x <= you->pos.x + you->width 
                        && me->pos.x + me->width >= you->pos.x 
                        && me->pos.y <= you->pos.y + you->height
                        && me->pos.y + me->height >= you->pos.y);
}

