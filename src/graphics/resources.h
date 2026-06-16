#ifndef RESOURCES_H
#define RESOURCES_H

#include <GL/glut.h>

// All game assets loaded once and kept across restarts.
typedef struct {
	GLuint armsDefault;   // running arms
	GLuint armsCrash;     // arms at the moment of the crash
	GLuint legs;          // legs (appear while ducking/sliding)
	GLuint legsJump;      // legs seen from above during the jump
	GLuint boim;          // comic popup shown on jump
	GLuint vupp;          // comic popup shown on slide/duck
	GLuint cabrumm;       // comic onomatopoeia shown on the crash
	int loaded;
} Resources;

// Loads every asset into 'r' (textures). Call once.
void resourcesLoad(Resources *r);

#endif
