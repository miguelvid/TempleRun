#include "obstacle.h"
#include "config.h"

void obstaclesInit(Obstacle *obstacles) {
	for (int i = 0; i < MAX_OBSTACLES; i++) {
		obstacles[i].active = 0;
	}
}

void spawnObstacleAt(Obstacle *obstacles, int lane, ObstacleType type, float z) {
	for (int i = 0; i < MAX_OBSTACLES; i++) {
		if (!obstacles[i].active) {
			obstacles[i].active = 1;
			obstacles[i].lane = lane;
			obstacles[i].type = type;
			obstacles[i].z = z;
			obstacles[i].prevZ = z;
			obstacles[i].scored = 0;
			return;
		}
	}
}

void spawnObstacle(Obstacle *obstacles, int lane, ObstacleType type) {
	spawnObstacleAt(obstacles, lane, type, SPAWN_DIST);
}

int obstaclesAnyActive(const Obstacle *obstacles) {
	for (int i = 0; i < MAX_OBSTACLES; i++) {
		if (obstacles[i].active) {
			return 1;
		}
	}
	return 0;
}

void obstaclesUpdate(Obstacle *obstacles, float speed) {
	for (int i = 0; i < MAX_OBSTACLES; i++) {
		if (!obstacles[i].active) {
			continue;
		}

		obstacles[i].prevZ = obstacles[i].z;
		obstacles[i].z += speed;   // o mundo vem em direção à câmera (esteira)

		if (obstacles[i].z > Z_REMOVE) {
			obstacles[i].active = 0;
		}
	}
}
