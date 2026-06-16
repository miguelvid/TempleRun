#ifndef SPRITES_H
#define SPRITES_H

#include <GL/glut.h>

// First-person view-model sprites and full-screen overlays.

// Draws a PNG of the arms/legs on top of the screen (FPS view-model style).
//   tex -> texture; bob -> vertical offset; alpha -> opacity (0..1)
//   flip180 -> 1 rotates the image 180° (fixes upside-down sprites)
void drawArmsSpriteEx(GLuint tex, float bob, float alpha, int flip180);
void drawArmsSprite(GLuint tex, float bob, float alpha);

// Draws a square sprite centered on the screen (used for the crash onomatopoeia).
//   frac -> size as a fraction of the smaller screen dimension (0.5 = half)
void drawCenterSprite(GLuint tex, float frac);

// Draws a square sprite in the right area of the screen (comic-style pop).
//   frac -> size as a fraction of the smaller screen dimension; alpha -> opacity
void drawCornerSprite(GLuint tex, float frac, float alpha);

// Covers the whole screen with translucent red (damage flash on collision).
void drawRedOverlay(float alpha);

// Covers the whole screen with translucent black (dims the scene behind menus).
void drawDimOverlay(float alpha);

// Procedural first-person arms (fallback when the arm textures are missing).
//   phase   -> running sway (pass the total distance)
//   ducking -> lowers the arms when the player is crouching
void drawArms(float phase, int ducking);

#endif
