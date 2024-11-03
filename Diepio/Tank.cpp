#include "Tank.h"

Tank::Tank(const char* name, const char* path_name) : Object(name, path_name) {
	reset_stats();
	last_shoot = clock() - reload_time;

	exp_for_next_level_up[1] = 4;
	exp_for_next_level_up[2] = 13;
	for (int i = 3; i < 50; i++)
	{
		exp_for_next_level_up[i] = 0.33 * pow(i, 3) - 4.4 * i * i + 48.8 * i - 88;
	}
}

int Tank::getLevel()
{
	return level;
}

int Tank::getExp()
{
	return exp;
}

void Tank::setExp(int value)
{
	exp = value;
}

int Tank::getUpgradePoints()
{
	return upgrade_points;
}

int* Tank::getStatsLevels()
{
	return stats_levels;
}

int* Tank::getExpForNextLevel()
{
	return exp_for_next_level_up;
}

float Tank::getBulletSpeed()
{
	return bullet_speed;
}

float Tank::getBulletDamage()
{
	return bullet_damage;
}

float Tank::getBulletPenetration()
{
	return bullet_penetration;
}

int Tank::getReloadTime()
{
	return reload_time;
}

float Tank::getMovementSpeed()
{
	return movement_speed;
}

clock_t Tank::getDeadTime()
{
	return dead_time;
}


void Tank::setRotation(float value)
{
	rotation = value;
}

void Tank::reset_stats()
{
	level = 1;
	exp = 0;
	upgrade_points = 0;
	for (int i = 0; i < 8; i++) stats_levels[i] = 0;
	max_health = 1000;
	health = max_health;
	health_regen = 20;
	body_damage = 25;
	bullet_speed = 1.0;
	bullet_damage = 15;
	bullet_penetration = 1.0;	
	reload_time = 500;
	movement_speed = 5.0;
	spawn_time = clock();
	position_x = rand() % ScreenWidth;
	position_y = rand() % ScreenHeight;
	dead_time = clock();
}
void Tank::setDeadStats()
{
	dead_time = clock();
	health = max_health;
	bullet_speed = 1.0;
	bullet_damage = 10;
	bullet_penetration = 1.0;	
	reload_time = 500;
	movement_speed = 0;
	position_x = ScreenWidth / 2 + 300;
	position_y = ScreenHeight / 2;
}

bool Tank::canShoot()
{
	if (1000.0 * (clock() - last_shoot) / CLOCKS_PER_SEC > reload_time)
	{
		last_shoot = clock();
		return true;
	}
	return false;
}

bool Tank::levelUp()
{
	if (exp < exp_for_next_level_up[level] || level >= MAX_LEVEL)
		return false;
	level++;
	if (level < MAX_LEVEL)
		upgrade_points++;
	return true;
}

void Tank::upgradeStat(int keycode)
{
	switch (keycode)
	{
	case ALLEGRO_KEY_1:
		if (stats_levels[0] < 6)
		{
			upgrade_points--;
			stats_levels[0]++;
			health_regen *= 1.25;
		}
		break;
	case ALLEGRO_KEY_2:
		if (stats_levels[1] < 6)
		{
			upgrade_points--;
			stats_levels[1]++;
			max_health *= 1.25;
		}
		break;
	case ALLEGRO_KEY_3:
		if (stats_levels[2] < 6)	
		{
			upgrade_points--;
			stats_levels[2]++;
			body_damage *= 2;
		}
		break;
	case ALLEGRO_KEY_4:
		if (stats_levels[3] < 6)
		{
			upgrade_points--;
			stats_levels[3]++;
			bullet_speed *= 1.1;
		}
		break;
	case ALLEGRO_KEY_5:
		if (stats_levels[4] < 6)
		{
			upgrade_points--;
			stats_levels[4]++;
			bullet_penetration *= 1.5;
		}
		break;
	case ALLEGRO_KEY_6:
		if (stats_levels[5] < 6)
		{
			upgrade_points--;
			stats_levels[5]++;
			bullet_damage *= 1.5;
		}
		break;
	case ALLEGRO_KEY_7:
		if (stats_levels[6] < 6)
		{
			upgrade_points--;
			stats_levels[6]++;
			reload_time *= 0.85;
		}
		break;
	case ALLEGRO_KEY_8:
		if (stats_levels[7] < 6)
		{
			upgrade_points--;
			stats_levels[7]++;
			movement_speed *= 1.1;
		}
		break;
	default:
		break;
	}
}