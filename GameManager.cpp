class GameManager {
public:
	const int MAX_EXP = INT_MAX;
	const int MAX_OBSTACLES = 15;
	const int SPAWN_TIME = 500;
	time_t delta_time;
	int numer_of_obstacles = 0;
	bool game_in_progress = true;
	bool show_debug = false;

	time_t last_time_obstacle_spawned;
	GameManager() {
		delta_time = clock();
		game_in_progress = true;
		last_time_obstacle_spawned = clock();
	}
	bool canSpawnObstacle()
	{
		return (1000.0 * (clock() - last_time_obstacle_spawned) / CLOCKS_PER_SEC) > SPAWN_TIME && numer_of_obstacles < MAX_OBSTACLES ? ++numer_of_obstacles : false;
	}
};