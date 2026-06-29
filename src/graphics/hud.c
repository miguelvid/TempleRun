#include "hud.h"
#include "render2d.h"

#include <GL/glut.h>
#include <stdio.h>
#include <string.h>

// X so that 'text' is horizontally centered (each glyph cell is 6*px wide).
static float centeredX(const char *text, float px) {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	float textWidth = (float)strlen(text) * 6.0f * px;
	return (width - textWidth) * 0.5f;
}

void drawHud(int score, int distance) {
	char buffer[64];
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	float s = fontScale();

	beginOverlay();
	glColor3f(1.0f, 1.0f, 1.0f);
	sprintf(buffer, "SCORE: %d", score);
	drawPixelText(20.0f * s, height - 38.0f * s, 4.0f * s, buffer);
	sprintf(buffer, "DIST: %dM", distance);
	drawPixelText(20.0f * s, height - 74.0f * s, 4.0f * s, buffer);
	endOverlay();
}

void drawGameOver(int score, int distance) {
	char buffer[64];
	int width  = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	float s = fontScale();

	float marginX = width * 0.03f;   // left-aligned, proportional

	beginOverlay();
	glColor3f(1.0f, 0.3f, 0.3f);
	drawPixelText(marginX, height / 2.0f + 20 * s, 9.0f * s, "GAME OVER");
	glColor3f(1.0f, 1.0f, 1.0f);
	sprintf(buffer, "SCORE: %d", score);
	drawPixelText(marginX, height / 2.0f - 24 * s, 4.0f * s, buffer);
	sprintf(buffer, "DIST: %dM", distance);
	drawPixelText(marginX, height / 2.0f - 60 * s, 4.0f * s, buffer);
	drawPixelText(marginX, height / 2.0f - 100 * s, 4.0f * s, "PRESS R TO RESTART");
	endOverlay();
}

void drawBonus(int amount, float alpha) {
	char buffer[32];
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	float s = fontScale();
	float px = 6.0f * s;

	sprintf(buffer, "BONUS +%d", amount);

	beginOverlay();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 0.9f, 0.2f, alpha);                     // golden, fading out
	drawPixelText(centeredX(buffer, px), height * 0.62f, px, buffer);
	glDisable(GL_BLEND);
	endOverlay();
}

void drawMenu(void) {
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	float s = fontScale();
	float titlePx = 11.0f * s;
	float hintPx = 4.0f * s;

	beginOverlay();
	glColor3f(0.95f, 0.85f, 0.2f);
	drawPixelText(centeredX("TEMPLE RUN", titlePx), height * 0.60f, titlePx, "TEMPLE RUN");

	glColor3f(0.7f, 0.7f, 0.7f);
	drawPixelText(centeredX("ENTER START", hintPx), height * 0.36f, hintPx, "ENTER START");
	endOverlay();
}

void drawPause(void) {
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	float s = fontScale();
	float bigPx = 10.0f * s;
	float subPx = 4.0f * s;

	beginOverlay();
	glColor3f(1.0f, 1.0f, 1.0f);
	drawPixelText(centeredX("PAUSED", bigPx), height * 0.54f, bigPx, "PAUSED");
	glColor3f(0.8f, 0.8f, 0.8f);
	drawPixelText(centeredX("PRESS P TO RESUME", subPx), height * 0.42f, subPx, "PRESS P TO RESUME");
	endOverlay();
}
