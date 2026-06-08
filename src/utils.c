#include "utils.h"
#include "constants.h"
#include "raylib.h"

int GetScreenMiddleX(int width)
{
        return GetScreenWidth() / 2 + width / 2;
}

int Gap(int width, int n)
{
        int total = width * n;
        if (total > SCREEN_WIDTH)
                return -1; // no enough space to fit n widths
        return (GetScreenWidth() - total) / (n + 1);
}
