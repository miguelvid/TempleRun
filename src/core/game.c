#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>

#include "game.h"
#include "config.h"
#include "player.h"
#include "obstacle.h"
#include "collision.h"
#include "resources.h"
#include "world.h"
#include "sprites.h"
#include "hud.h"
#include "audio.h"

typedef enum { PHASE_MENU, PHASE_PLAYING, PHASE_PAUSED, PHASE_GAMEOVER } GamePhase;

typedef struct {
	Player player;
	Obstacle obstacles[MAX_OBSTACLES];

	float totalDistance;
	float speed;
	int score;

	GamePhase phase;
	float gameOverTime;     // ms since the crash (delay before the game over screen)

	float spawnAccum;       // distance accumulated since the last spawn
	float spawnInterval;    // distance between spawns (shrinks over time)
	int duckHeld;           // 1 while the duck key is held
	float duckTime;         // ms accumulated in the ducking pose
	float lookLegs;         // 0..1 camera looks down at the legs during the jump
	float popupTime;        // ms left showing the comic popup (jump/slide)
	GLuint popupTex;        // which comic sprite the current popup shows
} GameState;

static GameState g;
static Resources res;

// Resets the run state and starts playing (used on ENTER from the menu and on R).
static void resetRun(void) {
	playerInit(&g.player);
	obstaclesInit(g.obstacles);

	g.totalDistance = 0.0f;
	g.speed = INITIAL_SPEED;
	g.score = 0;
	g.phase = PHASE_PLAYING;
	g.gameOverTime = 0.0f;

	g.spawnAccum = 0.0f;
	g.spawnInterval = 14.0f;
	g.duckHeld = 0;
	g.duckTime = 0.0f;
	g.lookLegs = 0.0f;
	g.popupTime = 0.0f;
	g.popupTex = 0;
}

// Jump triggered by input: plays the sound and pops the comic sprite only on a
// real take-off (ignored if already airborne or not playing).
static void doJump(void) {
	if (g.phase != PHASE_PLAYING) {
		return;
	}
	int wasJumping = g.player.jumping;
	playerJump(&g.player);
	if (!wasJumping && g.player.jumping) {
		audioPlay(SND_JUMP);
		g.popupTime = POPUP_MS;
		g.popupTex = res.boim;       // jump -> BOIM
	}
}

// Start ducking from input: plays the slide sound + popup only when the slide
// actually begins (a fresh press, not while already holding).
static void startDuck(void) {
	if (g.phase != PHASE_PLAYING) {
		return;
	}
	if (!g.duckHeld) {
		audioPlay(SND_SLIDE);
		g.popupTime = POPUP_MS;
		g.popupTex = res.vupp;       // slide/duck -> VUPP
	}
	g.duckHeld = 1;
}

void gameInit(void) {
	srand((unsigned int)time(NULL));

	// load the assets (only the first time; restart does not reload)
	if (!res.loaded) {
		resourcesLoad(&res);
	}

	resetRun();
	g.phase = PHASE_MENU;   // start on the menu, not playing
}

void gameUpdate(void) {
	// the running loop plays only while actually playing
	audioRunning(g.phase == PHASE_PLAYING);

	// "look at legs" camera dip eases in/out (also keeps easing on game over)
	float legTarget = g.player.jumping ? 1.0f : 0.0f;
	g.lookLegs += (legTarget - g.lookLegs) * LOOK_LEGS_RATE;

	if (g.phase == PHASE_GAMEOVER) {
		g.gameOverTime += MS_PER_FRAME;   // counts the delay until the game over screen
		return;
	}

	// menu and pause freeze the simulation
	if (g.phase != PHASE_PLAYING) {
		return;
	}

	// comic popup countdown
	if (g.popupTime > 0.0f) {
		g.popupTime -= MS_PER_FRAME;
	}

	// speed rises with the score (up to the maximum)
	g.speed = INITIAL_SPEED + g.score * SPEED_GAIN_PER_POINT;
	if (g.speed > MAX_SPEED) {
		g.speed = MAX_SPEED;
	}

	// ducking is not infinite: stands up after MAX_DUCK_TIME even while holding.
	// Releasing the key resets the timer, allowing to duck again.
	if (!g.duckHeld) {
		g.duckTime = 0.0f;
		g.player.ducking = 0;
	} else if (g.duckTime < MAX_DUCK_TIME) {
		g.duckTime += MS_PER_FRAME;
		g.player.ducking = 1;
	} else {
		g.player.ducking = 0;   // ran out of time: stand up
	}

	playerUpdate(&g.player);

	g.totalDistance += g.speed;
	obstaclesUpdate(g.obstacles, g.speed);

	// score when overtaking each obstacle
	for (int i = 0; i < MAX_OBSTACLES; i++) {
		if (g.obstacles[i].active && !g.obstacles[i].scored && g.obstacles[i].z > Z_PLAYER) {
			g.obstacles[i].scored = 1;
			g.score += 1;
		}
	}

	// collision
	for (int i = 0; i < MAX_OBSTACLES; i++) {
		if (g.obstacles[i].active && checkCollision(&g.player, &g.obstacles[i])) {
			g.phase = PHASE_GAMEOVER;
			audioPlay(SND_CRASH);
			break;
		}
	}

	// spawn based on the distance traveled
	g.spawnAccum += g.speed;
	if (g.spawnAccum >= g.spawnInterval) {
		g.spawnAccum = 0.0f;

		int lane = rand() % NUM_LANES;
		ObstacleType type = (ObstacleType)(rand() % 3);
		spawnObstacle(g.obstacles, lane, type);

		// difficulty: keeps shrinking the interval down to a minimum
		if (g.spawnInterval > 7.0f) {
			g.spawnInterval -= 0.15f;
		}
	}
}

// Draws the first-person view-model (running/sliding/jumping arms and legs).
static void drawViewModel(void) {
	if (res.armsDefault != 0) {
		float bob = sinf(g.totalDistance * 1.4f) * 12.0f;
		// legs first (stay BEHIND), appearing as the slide progresses
		if (res.legs != 0 && g.player.crouch > 0.01f) {
			drawArmsSprite(res.legs, bob, g.player.crouch);
		}
		// jump: legs seen from above, rotated 180° so they look right.
		// drop the image (proportional to screen height) to show more of the legs.
		if (res.legsJump != 0 && g.lookLegs > 0.01f) {
			float drop = glutGet(GLUT_WINDOW_HEIGHT) * LEGS_JUMP_DROP;
			drawArmsSpriteEx(res.legsJump, bob - drop, g.lookLegs, 1);
		}
		// arms on top, lower a bit during the slide but stay visible
		drawArmsSprite(res.armsDefault, bob - g.player.crouch * 60.0f, 1.0f);
	} else {
		drawArms(g.totalDistance, g.player.ducking);
	}
}

void gameRender(void) {
	float eyeHeight;
	float camX;
	float camZ = 3.0f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// eye height interpolated by the crouch (COD-style slide)
	eyeHeight = EYE_HEIGHT + (EYE_HEIGHT_DUCK - EYE_HEIGHT) * g.player.crouch;
	eyeHeight += g.player.y;   // the camera rises with the jump
	camX = g.player.x;

	// during the slide: rolls the camera sideways and looks a bit down.
	// during the jump: also looks down (as if looking at the legs).
	float roll = g.player.crouch * 0.14f;          // sideways roll (~8°)
	float upX = sinf(roll);
	float upY = cosf(roll);
	float dip = g.player.crouch * CAM_DIP_SLIDE + g.lookLegs * CAM_DIP_JUMP;

	// first-person camera, looking into the corridor (-Z)
	gluLookAt(camX, eyeHeight, camZ,
	          camX, eyeHeight - 0.1f - dip, camZ - 1.0f,
	          upX, upY, 0.0f);

	drawCorridor(g.totalDistance);

	for (int i = 0; i < MAX_OBSTACLES; i++) {
		if (g.obstacles[i].active) {
			drawObstacle(&g.obstacles[i]);
		}
	}

	switch (g.phase) {
		case PHASE_PLAYING:
			drawViewModel();
			// comic popup on jump (BOIM) / slide (VUPP), right side, fading out
			if (g.popupTime > 0.0f && g.popupTex != 0) {
				drawCornerSprite(g.popupTex, POPUP_FRAC, g.popupTime / POPUP_MS);
			}
			drawHud(g.score);
			break;
		case PHASE_PAUSED:
			drawViewModel();
			drawHud(g.score);
			drawDimOverlay(0.55f);
			drawPause();
			break;
		case PHASE_MENU:
			drawViewModel();
			drawDimOverlay(0.50f);
			drawMenu();
			break;
		case PHASE_GAMEOVER:
		default:
			// collision: red flash + crash arms + onomatopoeia (comic-book)
			drawRedOverlay(0.30f);
			if (res.armsCrash != 0) {
				drawArmsSprite(res.armsCrash, 0.0f, 1.0f);
			}
			if (res.cabrumm != 0) {
				drawCenterSprite(res.cabrumm, 0.5f);
			}
			// the game over screen only shows after the delay (the crash stays visible before)
			if (g.gameOverTime >= GAME_OVER_DELAY) {
				drawGameOver(g.score);
			}
			break;
	}
}

void gameOnKeyDown(unsigned char key) {
	// ----- keys that work regardless of phase -----
	if (key == 'p' || key == 'P') {           // pause toggle
		if (g.phase == PHASE_PLAYING) {
			g.phase = PHASE_PAUSED;
		} else if (g.phase == PHASE_PAUSED) {
			g.phase = PHASE_PLAYING;
		}
		return;
	}
	if (key == 13) {                          // ENTER: start from the menu
		if (g.phase == PHASE_MENU) {
			resetRun();
		}
		return;
	}
	if (key == 'r' || key == 'R') {           // restart from the game over screen
		if (g.phase == PHASE_GAMEOVER && g.gameOverTime >= GAME_OVER_DELAY) {
			resetRun();
		}
		return;
	}

	// ----- gameplay keys only while playing -----
	if (g.phase != PHASE_PLAYING) {
		return;
	}
	switch (key) {
		case ' ':
		case 'w':
		case 'W':
			doJump();
			break;
		case 'a':
		case 'A':
			playerMoveLeft(&g.player);
			break;
		case 'd':
		case 'D':
			playerMoveRight(&g.player);
			break;
		case 's':
		case 'S':
			startDuck();
			break;
		default:
			break;
	}
}

void gameOnKeyUp(unsigned char key) {
	if (key == 's' || key == 'S') {
		g.duckHeld = 0;
	}
}

void gameOnSpecialDown(int key) {
	if (g.phase != PHASE_PLAYING) {
		return;
	}
	if (key == GLUT_KEY_LEFT)  playerMoveLeft(&g.player);
	if (key == GLUT_KEY_RIGHT) playerMoveRight(&g.player);
	if (key == GLUT_KEY_UP)    doJump();
	if (key == GLUT_KEY_DOWN)  startDuck();
}

void gameOnSpecialUp(int key) {
	if (key == GLUT_KEY_DOWN) {
		g.duckHeld = 0;
	}
}

void gameOnMouseClick(int button, int state, int x, int y) {
	(void)button;
	(void)state;
	(void)x;
	(void)y;
}
