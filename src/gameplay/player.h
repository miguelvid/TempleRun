#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
	int lane;       // current lane (0=left, 1=middle, 2=right)
	float x;        // interpolated X (smooth lateral movement)
	float y;        // vertical offset (0 = ground)
	float velY;     // vertical velocity during the jump
	int jumping;
	int ducking;
	float crouch;   // 0 = standing, 1 = fully sliding (animated)
} Player;

void playerInit(Player *p);
void playerMoveLeft(Player *p);
void playerMoveRight(Player *p);
void playerJump(Player *p);
void playerUpdate(Player *p);

#endif
