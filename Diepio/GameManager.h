#pragma once
#include <time.h>

class GameManager {
public:
	//Maximum number of polygons at the same time
	const int MAX_OBSTACLES = 15;

	//New polygon spawn cooldown
	const int SPAWN_TIME = 500;

	//actual numer of polygons
	int numer_of_obstacles;

	//main loop variable
	bool game_in_progress;

	//timestamp of last polygon spawn
	time_t last_time_obstacle_spawned;


	GameManager();

	//checks if cooldown has passed and numer_of_obstacles<=MAX_OBSTACLES
	bool canSpawnObstacle();
};
