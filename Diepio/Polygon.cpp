#include "Polygon.h"

Polygon::Polygon(std::string name, const char* image_path, int maximum_health, int exp_points) : Object(name, image_path) {
	max_health = maximum_health;
	health = maximum_health;
	exp = exp_points;
	rotation_speed = 0.005;
	died = false;
}

int Polygon::getExp()
{
	return exp;
}

bool Polygon::isDead()
{
	return died;
}

void Polygon::Kill()
{
	died = true;
}

void Polygon::rotate()
{
	rotation += rotation_speed;
}
