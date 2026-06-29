#ifndef HUD_H
#define HUD_H

// On-screen text overlays (drawn with the pixel-art font).

void drawHud(int score, int distance);
void drawGameOver(int score, int distance);

// Fading "BONUS +N" text shown when an obstacle is cleared in-lane. alpha 1..0.
void drawBonus(int amount, float alpha);

// Title / start screen.
void drawMenu(void);
void drawPause(void);

#endif
