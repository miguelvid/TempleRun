#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
	int lane;       // pista atual (0=esquerda, 1=meio, 2=direita)
	float x;        // X interpolado (movimento lateral suave)
	float y;        // deslocamento vertical (0 = chão)
	float velY;     // velocidade vertical durante o pulo
	int jumping;
	int ducking;
	float crouch;   // 0 = de pé, 1 = totalmente em slide (animado)
} Player;

void playerInit(Player *p);
void playerMoveLeft(Player *p);
void playerMoveRight(Player *p);
void playerJump(Player *p);
void playerUpdate(Player *p);

#endif
