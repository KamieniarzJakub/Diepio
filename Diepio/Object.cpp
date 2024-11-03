#include "Object.h"


Object::Object(std::string set_name, const char* image_path)
{
	newGame();
	name = set_name;
	image = al_load_bitmap(image_path);
	position_x = rand() % ScreenWidth;
	position_y = rand() % ScreenHeight;
	offset_x = al_get_bitmap_width(image) / 2.0;
	offset_y = al_get_bitmap_height(image) / 2.0;
}

Object::~Object()
{
		
}

void Object::newGame()
{
	spawn_time = clock();
	last_hit = clock();
	last_heal = clock();
	max_health = 1000;
	health = max_health;
	health_regen = 10;
	heal_time = 2000;
	body_damage = 10;
	rotation = 0;
	killed_by = "";
}

std::string Object::getName()
{
	return name;
}

clock_t Object::getSpawnTime()
{
	return spawn_time;
}

int Object::getHealth()
{
	return health;
}

int Object::getMaxHealth()
{
	return max_health;
}
	
int Object::getHealthRegen()
{
	return health_regen;
}

float Object::getBodyDamage()
{
	return body_damage;
}

float Object::get_pos_x()
{
	return position_x;
}

float Object::get_pos_y()
{
	return position_y;
}

void Object::set_pos_x(float x)
{
	position_x = x;
}

void Object::set_pos_y(float y)
{
	position_y = y;
}

std::string Object::getKillerName()
{
	return killed_by;
}

void Object::setKillerName(std::string name)
{
	killed_by = name;
}

void Object::draw()
{
	al_draw_scaled_rotated_bitmap(image, offset_x, offset_y, position_x, position_y, 1, 1, rotation, NULL);
	if (health < max_health)
	{
		al_draw_line(position_x - getHpPercentage() * 50, position_y + offset_y + 15, position_x + getHpPercentage() * 50, position_y + offset_y + 15, al_map_rgba(255 * (1.0 - getHpPercentage()), 255.0 * getHpPercentage(), 0, 200), 10);
	}
}

bool Object::canDealDamaged(clock_t current_time, int delay)
{
	return (((1000.0 * (current_time - spawn_time) / CLOCKS_PER_SEC) > delay) ? true : false);
}

void Object::heal()
{
	if ((1000.0 * (clock() - last_heal) / CLOCKS_PER_SEC) > heal_time && health < max_health)
	{
		health = fmin(health + health_regen, max_health);
		last_heal = clock();
	}
}

bool Object::tryToKill(int body_damage) //true - object dead
{
	if (1000.0 * (clock() - last_hit) / CLOCKS_PER_SEC < 10)
		return false;
	last_hit = clock();
	for (int i = 0; i < body_damage; i++)
		health--;
	if (health <= 0)
		return true;
	return false;
}

void Object::dealDamage(Object other, int body_damage)
{
	other.tryToKill(body_damage);
}

bool Object::Collision(Object other)
{
	if (sqrt(pow(position_x - other.position_x, 2) + pow(position_y - other.position_y, 2)) < std::max(offset_x, offset_y) + std::max(other.offset_x, other.offset_y))
		return true;
	return false;
}

float Object::getHpPercentage()
{
	return (float)health / (float)max_health;
}