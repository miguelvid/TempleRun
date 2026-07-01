#include <stdlib.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "game.h"
#include "audio.h"
#include "config.h"

static int g_windowX = 50;
static int g_windowY = 50;
static int g_width = 1248;
static int g_height = 756;
static char g_title[] = "Temple Run 3D";
static int g_timerInterval = 5;   // how often we sample real time (ms); the
                                  // simulation still advances in MS_PER_FRAME steps

static void onDisplay(void) {
	gameRender();
	glutSwapBuffers();
}

static void onReshape(GLsizei width, GLsizei height) {
	if (height == 0) {
		height = 1;
	}

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0, (double)width / (double)height, 0.1, 200.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void onKeyDown(unsigned char key, int x, int y) {
	(void)x; (void)y;
	gameOnKeyDown(key);
}

static void onKeyUp(unsigned char key, int x, int y) {
	(void)x; (void)y;
	gameOnKeyUp(key);
}

static void onSpecialDown(int key, int x, int y) {
	(void)x; (void)y;
	gameOnSpecialDown(key);
}

static void onSpecialUp(int key, int x, int y) {
	(void)x; (void)y;
	gameOnSpecialUp(key);
}

static void onMouse(int button, int state, int x, int y) {
	gameOnMouseClick(button, state, x, y);
}

static void onTimer(int value) {
	(void)value;

	// Fixed-timestep with accumulator: advance the simulation in fixed
	// MS_PER_FRAME steps based on real elapsed time, so the game feels the same
	// regardless of how often (or unevenly) this timer actually fires.
	static int lastTime = -1;
	static float accumulator = 0.0f;

	int now = glutGet(GLUT_ELAPSED_TIME);
	if (lastTime < 0) {
		lastTime = now;   // first tick: no elapsed time yet
	}
	accumulator += (float)(now - lastTime);
	lastTime = now;

	// cap to avoid a spiral of death after a stall (e.g. window lost focus)
	if (accumulator > MAX_FRAME_MS) {
		accumulator = MAX_FRAME_MS;
	}

	while (accumulator >= MS_PER_FRAME) {
		gameUpdate();
		accumulator -= MS_PER_FRAME;
	}

	glutPostRedisplay();
	glutTimerFunc(g_timerInterval, onTimer, 0);
}

static void setup(void) {
	GLfloat fogColor[4] = {0.05f, 0.05f, 0.08f, 1.0f};

	glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	// neblina: dá profundidade ao corredor e suaviza como os obstáculos surgem
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_START, 20.0f);
	glFogf(GL_FOG_END, 70.0f);

	audioInit();
	atexit(audioShutdown);   // stop the engine cleanly on exit
	gameInit();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowPosition(g_windowX, g_windowY);
	glutInitWindowSize(g_width, g_height);
	glutCreateWindow(g_title);

	glutDisplayFunc(onDisplay);
	glutReshapeFunc(onReshape);

	glutKeyboardFunc(onKeyDown);
	glutKeyboardUpFunc(onKeyUp);
	glutIgnoreKeyRepeat(1);
	glutSpecialFunc(onSpecialDown);
	glutSpecialUpFunc(onSpecialUp);
	glutMouseFunc(onMouse);

	glutTimerFunc(g_timerInterval, onTimer, 0);

	setup();

	glutMainLoop();

	return 0;
}
