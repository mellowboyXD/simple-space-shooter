#ifndef UTILS_H
#define UTILS_H

#define max(a, b) a > b ? a : b
#define min(a, b) a < b ? a : b

int GetScreenMiddleX(int width); // Returns the midpoint along x-axis on the screen
int GamePanelMiddleX(int width); // Returns the midpoint along the x-axis inside the game panel
int Gap(int width, int n); // Returns the gap between n width to fill screen?
int GamePanelWidth();   // Returns the width of the game panel
int GamePanelHeight();  // Returns the height of the playable game panel. i.e., screen height

#endif // UTILS_H

