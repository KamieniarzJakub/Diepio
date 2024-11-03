#pragma once
#include "Object.h"
#include "Tank.h"

class Bullet : public Object {
private:
	float velocity_x;
	float velocity_y;
	float speed;
	float max_lifespan;
	std::vector<Object*> damaged_objects;

public:
	Bullet(Tank tank, float target_x, float traget_y, const char* image_path);

	//getter for max_lifespan
	float getMaxLifespan();

	//return alive time in miliseconds
	int getAliveTime();

	//bullet movement
	void move();

	//drawing bullets
	void draw();
};