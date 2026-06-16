#include "collision.h"
#include "config.h"

int checkCollision(const Player *p, const Obstacle *o) {
	// must be on the same lane
	if (p->lane != o->lane) {
		return 0;
	}

	// swept collision: did this frame's path [prevZ, z] cross the player's band
	// [-half, +half]? This way the step speed does not cause tunneling.
	float half = COLLISION_WINDOW;
	if (o->z < Z_PLAYER - half || o->prevZ > Z_PLAYER + half) {
		return 0;
	}

	switch (o->type) {
		case OBST_LOW:
			// collides if NOT high enough (i.e. did not jump)
			return p->y < JUMP_CLEARANCE;
		case OBST_HIGH:
			// collides if NOT ducking
			return !p->ducking;
		case OBST_BLOCK:
		default:
			// full block: always collides if on the same lane
			return 1;
	}
}
