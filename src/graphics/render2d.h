#ifndef RENDER2D_H
#define RENDER2D_H

// 2D overlay helpers drawn on top of the 3D scene: orthographic setup,
// font scaling and the pixel-art (5x7) text drawing.

// Sets up a 2D orthographic projection (0..width, 0..height), disabling depth
// and fog. Pair every call with endOverlay().
void beginOverlay(void);
void endOverlay(void);

// Font scale factor: proportional to the window height (reference 756px).
float fontScale(void);

// Draws pixel-art text. (x, y) = bottom-left corner; px = size of each "big pixel"
// in screen pixels. Must be called between beginOverlay()/endOverlay().
void drawPixelText(float x, float y, float px, const char *text);

#endif
