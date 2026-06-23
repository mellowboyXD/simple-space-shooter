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
        if (total > GamePanelWidth())
                return -1; // no enough space to fit n widths
        return (GamePanelWidth() - total) / (n + 1);
}

int GamePanelMiddleX(int width)
{
        return GamePanelWidth() / 2 + width / 2;
}

int GamePanelWidth()
{
        return (1 - UI_PANEL_RATIO) * GetScreenWidth();
}

int GamePanelHeight()
{
        return GetScreenHeight();
}
