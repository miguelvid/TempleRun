#include "resources.h"
#include "texture.h"

void resourcesLoad(Resources *r) {
	r->armsDefault = loadTexture("assets/arms-default.png");
	r->armsCrash   = loadTexture("assets/arms-crash.png");
	r->legs        = loadTexture("assets/legs.png");
	r->legsJump    = loadTexture("assets/legs-jump.png");
	r->boim        = loadTexture("assets/boim.png");
	r->vupp        = loadTexture("assets/vupp.png");
	r->cabrumm     = loadTexture("assets/cabrumm.png");

	r->loaded = 1;
}
