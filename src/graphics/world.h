#ifndef WORLD_H
#define WORLD_H

#include "obstacle.h"

// 3D scene: the running corridor and the obstacles.

// Draws the corridor (floor, ceiling, walls, lane lines and moving markers).
void drawCorridor(float distance);

// Draws a single obstacle (a colored box per type).
void drawObstacle(const Obstacle *o);

#endif
