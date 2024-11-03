#pragma once
#include "Object.h"

#define MAX_LEVEL 50

class Tank : public Object {
protected:
	int level;
	int exp;
	int upgrade_points;
	int stats_levels[8];
	int exp_for_next_level_up[50];
	float bullet_speed;
	float bullet_damage;
	float bullet_penetration;
	int reload_time;
	float movement_speed;
	clock_t last_shoot;
	clock_t dead_time;
	
public:
	Tank(const char* name, const char* path_name);

	//getter of level
	int getLevel();
	//getter and setter of exp
	int getExp();
	void setExp(int value);
	//getter upgrade_points
	int getUpgradePoints();
	//getter of stats_levels
	int* getStatsLevels();
	//getter of exp_for_next_level_up
	int* getExpForNextLevel();
	//getter of bullet_speed
	float getBulletSpeed();
	//getter fo bullet_damage
	float getBulletDamage();
	//getter of bullet_penetration
	float getBulletPenetration();
	//getter of reload_time
	int getReloadTime();
	//getter of movement_speedd
	float getMovementSpeed();
	//getter of dead_time
	clock_t getDeadTime();
	//setter of rotation value
	void setRotation(float value);
	//reseting player stats
	void reset_stats();
	//setting stats for dead_mode
	void setDeadStats();
	//checking if tank is ready to shoot
	bool canShoot();
	//leveling up
	bool levelUp();
	//upgrading one of 8 stats
	void upgradeStat(int keycode);
};