#include "obstacle.h"
#include "config.h"

void obstaclesInit(Obstacle *obstacles) {
	for (int i = 0; i < MAX_OBSTACLES; i++) {
		obstacles[i].active = 0;
	}
}

void spawnObstacle(Obstacle *obstacles, int lane, ObstacleType type) {
	for (int i = 0; i < MAX_OBSTACLES; i++) {
		if (!obstacles[i].active) {
			obstacles[i].active = 1;
			obstacles[i].lane = lane;
			obstacles[i].type = type;
			obstacles[i].z = SPAWN_DIST;
			obstacles[i].prevZ = SPAWN_DIST;
			obstacles[i].scored = 0;
			return;
		}
	}
}

void obstaclesUpdate(Obstacle *obstacles, float speed) {
	for (int i = 0; i < MAX_OBSTACLES; i++) {
		if (!obstacles[i].active) {
			continue;
		}

		obstacles[i].prevZ = obstacles[i].z;
		obstacles[i].z += speed;

		if (obstacles[i].z > Z_REMOVE) {
			obstacles[i].active = 0;
		}
	}
}
