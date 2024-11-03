#pragma once
#include "Object.h"

class Polygon : public Object {
private:
	int exp;
	float rotation_speed;
	bool died;


public:
	Polygon(std::string name, const char* image_path, int maximum_health, int exp_points);

	//getter of exp value
	int getExp();

	//getter of died value
	bool isDead();

	//object is dead (died=true)
	void Kill();

	//constant object rotating differs from rotation_speed 
	void rotate();
};