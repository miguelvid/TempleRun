#include "player.h"
#include "config.h"

void playerInit(Player *p) {
	p->lane = 1;
	p->x = laneToX(1);
	p->y = 0.0f;
	p->velY = 0.0f;
	p->jumping = 0;
	p->ducking = 0;
	p->crouch = 0.0f;
}

void playerMoveLeft(Player *p) {
	if (p->lane > 0) {
		p->lane--;
	}
}

void playerMoveRight(Player *p) {
	if (p->lane < NUM_LANES - 1) {
		p->lane++;
	}
}

void playerJump(Player *p) {
	if (!p->jumping) {
		p->jumping = 1;
		p->velY = JUMP_SPEED;
	}
}

void playerUpdate(Player *p) {
	// movimento lateral suave em direção ao centro da pista-alvo
	float targetX = laneToX(p->lane);
	p->x += (targetX - p->x) * LANE_LERP;

	// transição rápida do slide (desce rápido, levanta um pouco mais devagar)
	float targetCrouch = p->ducking ? 1.0f : 0.0f;
	float rate = p->ducking ? CROUCH_DOWN_RATE : CROUCH_UP_RATE;
	p->crouch += (targetCrouch - p->crouch) * rate;

	// física do pulo
	if (p->jumping) {
		p->y += p->velY;
		p->velY -= GRAVITY;

		if (p->y <= 0.0f) {
			p->y = 0.0f;
			p->velY = 0.0f;
			p->jumping = 0;
		}
	}
}
