#include "world.h"
#include "config.h"

#include <GL/glut.h>
#include <math.h>

#define CORRIDOR_WIDTH (NUM_LANES * LANE_WIDTH)
#define WALL_HEIGHT 5.0f

// Draws a colored cube with a black outline (helps perceive depth).
static void drawBox(float cx, float cy, float cz,
                    float sx, float sy, float sz,
                    float r, float g, float b) {
	glPushMatrix();
	glTranslatef(cx, cy, cz);
	glScalef(sx, sy, sz);

	glColor3f(r, g, b);
	glutSolidCube(1.0);

	glColor3f(0.0f, 0.0f, 0.0f);
	glutWireCube(1.02);

	glPopMatrix();
}

void drawCorridor(float distance) {
	float half = CORRIDOR_WIDTH / 2.0f;
	float zFront = Z_REMOVE;
	float zBack = SPAWN_DIST - 10.0f;

	// floor
	glColor3f(0.15f, 0.15f, 0.18f);
	glBegin(GL_QUADS);
		glVertex3f(-half, 0.0f, zFront);
		glVertex3f( half, 0.0f, zFront);
		glVertex3f( half, 0.0f, zBack);
		glVertex3f(-half, 0.0f, zBack);
	glEnd();

	// ceiling
	glColor3f(0.10f, 0.10f, 0.12f);
	glBegin(GL_QUADS);
		glVertex3f(-half, WALL_HEIGHT, zFront);
		glVertex3f( half, WALL_HEIGHT, zFront);
		glVertex3f( half, WALL_HEIGHT, zBack);
		glVertex3f(-half, WALL_HEIGHT, zBack);
	glEnd();

	// left wall
	glColor3f(0.20f, 0.20f, 0.26f);
	glBegin(GL_QUADS);
		glVertex3f(-half, 0.0f, zFront);
		glVertex3f(-half, WALL_HEIGHT, zFront);
		glVertex3f(-half, WALL_HEIGHT, zBack);
		glVertex3f(-half, 0.0f, zBack);
	glEnd();

	// right wall
	glColor3f(0.22f, 0.22f, 0.28f);
	glBegin(GL_QUADS);
		glVertex3f(half, 0.0f, zFront);
		glVertex3f(half, WALL_HEIGHT, zFront);
		glVertex3f(half, WALL_HEIGHT, zBack);
		glVertex3f(half, 0.0f, zBack);
	glEnd();

	// lane dividers (lines on the floor)
	glColor3f(0.40f, 0.40f, 0.45f);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
		for (int f = 1; f < NUM_LANES; f++) {
			float x = -half + f * LANE_WIDTH;
			glVertex3f(x, 0.01f, zFront);
			glVertex3f(x, 0.01f, zBack);
		}
	glEnd();

	// transverse markers that "scroll" to give a sense of speed
	glColor3f(0.30f, 0.30f, 0.36f);
	glBegin(GL_LINES);
		float offset = fmodf(distance, 4.0f);
		for (float z = zBack; z < zFront; z += 4.0f) {
			float zl = z + offset;
			glVertex3f(-half, 0.02f, zl);
			glVertex3f( half, 0.02f, zl);
		}
	glEnd();
}

void drawObstacle(const Obstacle *o) {
	float x = laneToX(o->lane);
	float width = LANE_WIDTH * 0.8f;
	float depth = 0.8f;

	switch (o->type) {
		case OBST_LOW:
			// low block on the floor -> jump
			drawBox(x, 0.45f, o->z, width, 0.9f, depth, 0.85f, 0.25f, 0.20f);
			break;
		case OBST_HIGH:
			// bar hanging from the ceiling -> duck (room left below)
			drawBox(x, 3.4f, o->z, width, 3.2f, depth, 0.25f, 0.45f, 0.85f);
			break;
		case OBST_BLOCK:
		default:
			// full block -> change lane
			drawBox(x, 2.5f, o->z, width, 5.0f, depth, 0.90f, 0.55f, 0.15f);
			break;
	}
}
