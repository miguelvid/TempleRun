#ifndef OBSTACLE_H
#define OBSTACLE_H

typedef enum {
	OBST_LOW,     // bloco baixo: PULE por cima
	OBST_HIGH,    // barra alta: AGACHE para passar embaixo
	OBST_BLOCK    // bloco inteiro: MUDE de pista
} ObstacleType;

typedef struct {
	int active;
	int lane;
	float z;
	float prevZ;        // Z no quadro anterior (colisão varrida, sem tunneling)
	ObstacleType type;
	int scored;         // já contou ponto quando foi ultrapassado
} Obstacle;

void obstaclesInit(Obstacle *obstacles);
void spawnObstacle(Obstacle *obstacles, int lane, ObstacleType type);
// Same, but born at a custom Z (used by the run's lead-in pre-spawn).
void spawnObstacleAt(Obstacle *obstacles, int lane, ObstacleType type, float z);
void obstaclesUpdate(Obstacle *obstacles, float speed);
int obstaclesAnyActive(const Obstacle *obstacles);

#endif
