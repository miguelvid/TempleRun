#include "sprites.h"
#include "render2d.h"

#include <math.h>

void drawArmsSpriteEx(GLuint tex, float bob, float alpha, int flip180) {
	int width  = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	beginOverlay();   // 2D projection (0..width, 0..height), no depth nor fog

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   // respects the PNG transparency
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glColor4f(1.0f, 1.0f, 1.0f, alpha);

	// 180° rotation = flip the texture coordinates on both axes
	float u0 = flip180 ? 1.0f : 0.0f, u1 = flip180 ? 0.0f : 1.0f;
	float v0 = flip180 ? 1.0f : 0.0f, v1 = flip180 ? 0.0f : 1.0f;

	float offsetY = height * -0.12f;        // negative = lowers the image
	float bottom = bob + offsetY;           // bottom edge
	float top = height * 0.55f + bottom;    // top edge (keeps the image height)
	glBegin(GL_QUADS);
		glTexCoord2f(u0, v0); glVertex2f(0.0f,  bottom);
		glTexCoord2f(u1, v0); glVertex2f(width, bottom);
		glTexCoord2f(u1, v1); glVertex2f(width, top);
		glTexCoord2f(u0, v1); glVertex2f(0.0f,  top);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	endOverlay();
}

void drawArmsSprite(GLuint tex, float bob, float alpha) {
	drawArmsSpriteEx(tex, bob, alpha, 0);
}

void drawCenterSprite(GLuint tex, float frac) {
	int width  = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	float side = (width < height ? width : height) * frac;
	float cx = width / 2.0f;
	float cy = height / 2.0f;

	beginOverlay();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(cx - side/2, cy - side/2);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(cx + side/2, cy - side/2);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(cx + side/2, cy + side/2);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(cx - side/2, cy + side/2);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	endOverlay();
}

void drawCornerSprite(GLuint tex, float frac, float alpha) {
	int width  = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	float side = (width < height ? width : height) * frac;
	// upper-right area, with a small margin
	float cx = width - side * 0.6f - width * 0.04f;
	float cy = height * 0.66f;

	beginOverlay();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glColor4f(1.0f, 1.0f, 1.0f, alpha);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(cx - side/2, cy - side/2);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(cx + side/2, cy - side/2);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(cx + side/2, cy + side/2);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(cx - side/2, cy + side/2);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	endOverlay();
}

// Fills the whole screen with a single translucent color.
static void fillScreen(float r, float g, float b, float alpha) {
	int width  = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	beginOverlay();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(r, g, b, alpha);
	glBegin(GL_QUADS);
		glVertex2f(0.0f,  0.0f);
		glVertex2f(width, 0.0f);
		glVertex2f(width, height);
		glVertex2f(0.0f,  height);
	glEnd();
	glDisable(GL_BLEND);
	endOverlay();
}

void drawRedOverlay(float alpha) {
	fillScreen(0.8f, 0.0f, 0.0f, alpha);
}

void drawDimOverlay(float alpha) {
	fillScreen(0.0f, 0.0f, 0.0f, alpha);
}

// ----- Procedural first-person arms (view-model attached to the camera) -----

static GLUquadric *g_quad = NULL;

static void ensureQuadric(void) {
	if (g_quad == NULL) {
		g_quad = gluNewQuadric();
		gluQuadricNormals(g_quad, GLU_SMOOTH);   // smooth normals -> continuous shading
	}
}

// cylinder along the local +Z (from z=0 to z=height), optionally tapering
static void cylinder(float baseRadius, float topRadius, float height) {
	gluCylinder(g_quad, baseRadius, topRadius, height, 18, 3);
}

static void sphere(float radius) {
	gluSphere(g_quad, radius, 16, 12);
}

// One arm (shoulder -> forearm -> wrist -> hand -> fingers) growing along local +Z.
// bend = extra angle of the running "pump".
static void drawOneArm(float bend) {
	glColor3f(0.86f, 0.64f, 0.48f);   // skin tone (becomes material via GL_COLOR_MATERIAL)

	sphere(0.085f);                   // shoulder / joint

	glRotatef(bend, 1.0f, 0.0f, 0.0f);    // pumps the forearm up/down
	cylinder(0.085f, 0.07f, 0.58f);       // tapered forearm

	glTranslatef(0.0f, 0.0f, 0.58f);
	sphere(0.075f);                   // wrist

	// palm (flattened sphere)
	glColor3f(0.82f, 0.58f, 0.44f);
	glPushMatrix();
		glScalef(1.05f, 0.55f, 0.95f);
		sphere(0.11f);
	glPopMatrix();

	// 4 short fingers fanning out in front of the palm
	glTranslatef(0.0f, 0.0f, 0.08f);
	for (int d = 0; d < 4; d++) {
		float dx = (d - 1.5f) * 0.045f;
		glPushMatrix();
			glTranslatef(dx, 0.0f, 0.0f);
			glRotatef((d - 1.5f) * 6.0f, 0.0f, 1.0f, 0.0f);
			cylinder(0.022f, 0.018f, 0.13f);
		glPopMatrix();
	}
	// thumb, more to the side and slightly tilted
	glPushMatrix();
		glTranslatef(0.085f, 0.0f, -0.04f);
		glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
		cylinder(0.024f, 0.02f, 0.10f);
	glPopMatrix();
}

void drawArms(float phase, int ducking) {
	float pump = sinf(phase * 1.4f) * 9.0f;        // alternating arm pump
	float baseY = ducking ? -0.78f : -0.52f;

	ensureQuadric();

	// ---- save state and set up local lighting just for the arms ----
	glPushAttrib(GL_LIGHTING_BIT | GL_FOG_BIT | GL_ENABLE_BIT);

	glDisable(GL_FOG);                 // arms are close: should not fade into the fog
	glEnable(GL_NORMALIZE);            // fix normals after glScalef
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	GLfloat lightPos[4] = {0.4f, 0.8f, 1.0f, 0.0f};   // directional, from above/front
	GLfloat lightDif[4] = {0.9f, 0.9f, 0.88f, 1.0f};
	GLfloat lightAmb[4] = {0.45f, 0.42f, 0.40f, 1.0f};
	GLfloat shine[4]    = {0.25f, 0.25f, 0.25f, 1.0f};

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();                  // camera space: eye at the origin, looking down -Z

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);   // positioned in camera space
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glMaterialfv(GL_FRONT, GL_SPECULAR, shine);
	glMaterialf(GL_FRONT, GL_SHININESS, 12.0f);

	// right arm (pumps opposite to the left)
	glPushMatrix();
		glTranslatef(0.30f, baseY, -0.45f);
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);   // local +Z now points into the screen
		glRotatef(-9.0f, 0.0f, 1.0f, 0.0f);    // pushes it slightly outward
		glRotatef(18.0f, 1.0f, 0.0f, 0.0f);    // tilts the arm downward
		drawOneArm(pump);
	glPopMatrix();

	// left arm (mirrored)
	glPushMatrix();
		glTranslatef(-0.30f, baseY, -0.45f);
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(9.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(18.0f, 1.0f, 0.0f, 0.0f);
		drawOneArm(-pump);
	glPopMatrix();

	glPopMatrix();
	glPopAttrib();                     // restores fog/light for the rest of the scene
}
