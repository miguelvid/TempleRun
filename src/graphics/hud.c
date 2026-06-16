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

void drawHud(int score) {
	char buffer[64];
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	float s = fontScale();

	sprintf(buffer, "SCORE: %d", score);

	beginOverlay();
	glColor3f(1.0f, 1.0f, 1.0f);
	drawPixelText(20.0f * s, height - 38.0f * s, 4.0f * s, buffer);
	endOverlay();
}

void drawGameOver(int score) {
	char buffer[64];
	int width  = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	float s = fontScale();

	sprintf(buffer, "SCORE: %d", score);

	float marginX = width * 0.03f;   // left-aligned, proportional

	beginOverlay();
	glColor3f(1.0f, 0.3f, 0.3f);
	drawPixelText(marginX, height / 2.0f + 20 * s, 9.0f * s, "GAME OVER");
	glColor3f(1.0f, 1.0f, 1.0f);
	drawPixelText(marginX, height / 2.0f - 24 * s, 4.0f * s, buffer);
	drawPixelText(marginX, height / 2.0f - 64 * s, 4.0f * s, "PRESS R TO RESTART");
	endOverlay();
}

void drawMenu(void) {
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	float s = fontScale();
	float titlePx = 11.0f * s;
	float subPx = 4.0f * s;
	float hintPx = 3.0f * s;

	beginOverlay();
	glColor3f(0.95f, 0.85f, 0.2f);
	drawPixelText(centeredX("TEMPLE RUN", titlePx), height * 0.58f, titlePx, "TEMPLE RUN");
	glColor3f(1.0f, 1.0f, 1.0f);
	drawPixelText(centeredX("PRESS ENTER TO START", subPx), height * 0.40f, subPx, "PRESS ENTER TO START");
	glColor3f(0.7f, 0.7f, 0.7f);
	drawPixelText(centeredX("ARROWS MOVE   UP JUMP   DOWN SLIDE", hintPx), height * 0.30f, hintPx, "ARROWS MOVE   UP JUMP   DOWN SLIDE");
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
