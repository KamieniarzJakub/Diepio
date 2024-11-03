#include "GameManager.h"

GameManager::GameManager() {
	numer_of_obstacles = 0;
	game_in_progress = true;
	last_time_obstacle_spawned = clock();
}

bool GameManager::canSpawnObstacle()
{
	return (1000.0 * (clock() - last_time_obstacle_spawned) / CLOCKS_PER_SEC) > SPAWN_TIME && numer_of_obstacles <= MAX_OBSTACLES ? ++numer_of_obstacles : false;
}