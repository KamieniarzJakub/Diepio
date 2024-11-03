#pragma once
#include <string>
#include <cmath>
#include <vector>
#include <list>
#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#define ScreenWidth 1920
#define ScreenHeight 1080

class Object {
protected:
	std::string name;
	clock_t spawn_time;
	clock_t last_hit;
	clock_t last_heal;
	int health;
	int max_health;
	int health_regen;
	int heal_time;
	float body_damage;
	float position_x;
	float position_y;
	ALLEGRO_BITMAP* image;
	float offset_x;
	float offset_y;
	float rotation;
	std::string killed_by;

public:
	Object(std::string set_name, const char* image_path);

	~Object();

	//setting defualt values of variables
	void newGame();

	//getter for name 
	std::string getName();

	//getter for spawn_time
	clock_t getSpawnTime();

	//getter for health
	int getHealth();
	//getter for max_health
	int getMaxHealth();
	//getter for heatlth_regen
	int getHealthRegen();
	//getter for body_damage
	float getBodyDamage();

	//getters of position x and position y
	float get_pos_x();
	float get_pos_y();

	//setters for position x and position y
	void set_pos_x(float x);
	void set_pos_y(float y);

	//getter and setter for killer_name
	std::string getKillerName();
	void setKillerName(std::string name);

	//object drawing
	void draw();

	//checking damage cooldown
	bool canDealDamaged(clock_t current_time, int delay);

	//healing
	void heal();

	//object taking damage
	//true - object is dead
	bool tryToKill(int body_damage);

	//dealing damage to other object
	void dealDamage(Object other, int body_damage);

	//circle collision detection
	bool Collision(Object other);

private:
	//getter of health/max_health
	float getHpPercentage();
};