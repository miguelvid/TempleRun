#ifndef COLLISION_H
#define COLLISION_H

#include "player.h"
#include "obstacle.h"

// Returns 1 if the player collided with the obstacle.
int checkCollision(const Player *p, const Obstacle *o);

#endif
