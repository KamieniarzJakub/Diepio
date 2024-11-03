#include "Bullet.h"

Bullet::Bullet(Tank tank, float target_x, float target_y, const char* image_path) : Object("Bulet", image_path) 
{
	max_health = 100 * tank.getBulletPenetration();
	health = 100;
	body_damage = tank.getBulletDamage();
	speed = tank.getBulletSpeed();
	max_lifespan = 1000;
	spawn_time = clock();
	position_x = tank.get_pos_x();
	position_y = tank.get_pos_y();
	double vector_length = std::sqrt(pow(target_x - tank.get_pos_x(), 2.0) + pow(target_y - tank.get_pos_y(), 2.0));
	velocity_x = ((double)target_x - tank.get_pos_x()) / vector_length * 10.0;
	velocity_y = ((double)target_y - tank.get_pos_y()) / vector_length * 10.0;
}

void Bullet::move()
{
	position_x += velocity_x * speed;
	position_y += velocity_y * speed;
}

float Bullet::getMaxLifespan()
{
	return max_lifespan;
}

int Bullet::getAliveTime()
{
	return 1000.0f * ((float)clock() - (float)spawn_time) / CLOCKS_PER_SEC;
}

void Bullet::draw()
{
	al_draw_scaled_rotated_bitmap(image, offset_x, offset_y, position_x, position_y, 1, 1, rotation, NULL);
}