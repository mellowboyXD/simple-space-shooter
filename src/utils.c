#include "utils.h"
#include "constants.h"

int GetScreenMiddleX(int width)
{
        return SCREEN_WIDTH / 2 + width / 2;
}

int Gap(int width, int n)
{
        int total = width * n;
        if (total > SCREEN_WIDTH)
                return -1; // no enough space to fit n widths
        return (SCREEN_WIDTH - total) / n + 2;
}
