#include "Enemy.h"

Enemy::Enemy(int player_level) : Tank("Enemy", "images/Enemy.png")
{
	reset_stats(player_level);
	ON = true;
}

bool Enemy::isON()
{
	return ON;
}

void Enemy::reset_stats(int player_level)
{
	level = player_level + rand() % 10;
	level = (level > 50 ? 50 : level);
	for (int i = 0; i < level; i++)
	{
		//setting random levels for enemy
		int button = ALLEGRO_KEY_1 + rand() % 8;
		while (stats_levels[button - ALLEGRO_KEY_1] == 6)
			button = ALLEGRO_KEY_1 + rand() % 8;
		upgradeStat(button);
	}

	health = max_health;
	spawn_time = clock();
	position_x = 10000 * (rand() % 2 == 0 ? 1 : -1);
	position_y = 10000 * (rand() % 2 == 0 ? 1 : -1);
}

void Enemy::move(float player_x, float player_y)
{
	double vector_length = getDistance(player_x, player_y);
	position_x += double(player_x - position_x) / vector_length * movement_speed;
	position_y += double(player_y - position_y) / vector_length * movement_speed;
}

double Enemy::getDistance(float player_x, float player_y)
{
	return std::sqrt((player_x - position_x) * (player_x - position_x) + (player_y - position_y) * (player_y - position_y));
}