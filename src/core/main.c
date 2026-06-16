#include <GL/glut.h>
#include <GL/freeglut.h>

#include "game.h"

static int g_windowX = 50;
static int g_windowY = 50;
static int g_width = 1248;
static int g_height = 756;
static char g_title[] = "Temple Run 3D";
static int g_timerInterval = 16;

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
	gameUpdate();

	glutPostRedisplay();
	glutTimerFunc(g_timerInterval, onTimer, 0);
}

static void setup(void) {
	GLfloat fogColor[4] = {0.05f, 0.05f, 0.08f, 1.0f};

	glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	// fog: gives the corridor depth and softens how obstacles appear
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_START, 20.0f);
	glFogf(GL_FOG_END, 70.0f);

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
