#ifndef OBSTACLE_H
#define OBSTACLE_H

typedef enum {
	OBST_LOW,     // low block: JUMP over it
	OBST_HIGH,    // high bar: DUCK to pass under
	OBST_BLOCK    // full block: CHANGE lane
} ObstacleType;

typedef struct {
	int active;
	int lane;
	float z;
	float prevZ;        // Z in the previous frame (swept collision, no tunneling)
	ObstacleType type;
	int scored;         // already counted a point when overtaken
} Obstacle;

void obstaclesInit(Obstacle *obstacles);
void spawnObstacle(Obstacle *obstacles, int lane, ObstacleType type);
void obstaclesUpdate(Obstacle *obstacles, float speed);

#endif
