#pragma once
#include "Tank.h"

class Enemy : public Tank {
private:
	//enemy switch
	bool ON;

public:
	Enemy(int player_level);

	//getter for value of ON
	bool isON();

	//setting enemy level simmilar to player level
	void reset_stats(int player_level);

	//player chasing
	void move(float player_x, float player_y);

	//distance between player and enemy
	double getDistance(float player_x, float player_y);
};