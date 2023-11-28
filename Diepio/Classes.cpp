#include <chrono>
#include <map>
#include <cmath>
#include <vector>
#include <time.h>

#define ScreenWidth 1920
#define ScreenHeight 1080

#define PI 3.141592

class GameManager {
public:
	const int MAX_EXP = INT_MAX;
	const int MAX_OBSTACLES = 15;
	const int SPAWN_TIME = 500;
	time_t delta_time;
	int numer_of_obstacles = 0;
	bool game_in_progress = true;
	bool show_debug = false;

	time_t last_time_obstacle_spawned;
	GameManager() {
		delta_time = clock();
		game_in_progress = true;
		last_time_obstacle_spawned = clock();
	}
	bool canSpawnObstacle()
	{
		return (1000.0 * (clock() - last_time_obstacle_spawned) / CLOCKS_PER_SEC)>SPAWN_TIME && numer_of_obstacles<MAX_OBSTACLES ? ++numer_of_obstacles  : false;
	}
};

class Mouse {
public:
	float position_x=0;
	float position_y=0;
};

class Object {
public:
	std::string name;
	clock_t spawn_time;
	int health = 1000;
	int max_health = 1000;
	int health_regen = 10;
	clock_t last_heal;
	int heal_time = 2000;
	clock_t last_hit;
	float damage = 10;
	float position_x;
	float position_y;
	float offset_x = 0;
	float offset_y = 0;
	ALLEGRO_BITMAP* image;
	float rotation = 0;

	Object(std::string set_name, const char* image_path)
	{
		spawn_time = clock();
		name = set_name;
		position_x = rand() % ScreenWidth;
		position_y = rand() % ScreenHeight;
		image = al_load_bitmap(image_path);
		offset_x = al_get_bitmap_width(image) / 2.0;
		offset_y = al_get_bitmap_height(image) / 2.0;
		last_hit = clock();
		last_heal = clock();
	}

	~Object()
	{

	}

	void draw()
	{
		al_draw_scaled_rotated_bitmap(image, offset_x, offset_y, position_x, position_y, 1, 1, rotation, NULL);
		if (health < max_health)
		{
			al_draw_line(position_x - getHpPercentage() * 50, position_y + offset_y + 15, position_x + getHpPercentage() * 50, position_y + offset_y + 15, al_map_rgba(255 * (1.0 - getHpPercentage()), 255.0 * getHpPercentage(), 0, 200), 10);
		}
	}

	bool canDealDamaged(clock_t current_time, int delay)
	{
		return (((1000.0 * (current_time-spawn_time) / CLOCKS_PER_SEC) > delay) ? true : false);
	}

	void heal()
	{
		if ((1000.0 * (clock() - last_heal) / CLOCKS_PER_SEC) > heal_time && health < max_health)
		{
			health = fmin(health + health_regen, max_health);
			last_heal = clock();
		}
	}

	bool getDamage(int damage) //true - object dead
	{
		if (1000.0 * (clock() - last_hit) / CLOCKS_PER_SEC < 10)
			return false;
		last_hit = clock();
		for (int i = 0; i < damage; i++)
			health--;
		if (health <= 0)
			return true;
		return false;
	}

	void dealDamage(Object other, int damage)
	{
		other.getDamage(damage);
	}

	bool Collision(Object other)
	{
		if (sqrt(pow(position_x - other.position_x, 2) + pow(position_y - other.position_y, 2)) < std::max(offset_x, offset_y) + std::max(other.offset_x, other.offset_y))
			return true;
		return false;
	}

private:
	float getHpPercentage()
	{
		return (float)health / (float)max_health;
	}
};

class Polygon : public Object{
public:
	int exp;
	float rotation_speed = 0.005;
	bool died = false;

	using Object::Object;
	Polygon(std::string name, const char* image_path, int maximum_health, int exp_points) : Object(name, image_path) {
		max_health = maximum_health;
		health = maximum_health;
		exp = exp_points;
	}

	void rotate()
	{
		rotation += rotation_speed;
	}
};

class Tank : public Object {
public:
	const int MAX_LEVEL = 50;
	int level;
	int exp;
	int exp_for_next_level_up[50] = {0};
	int upgrade_points;
	int stats_levels[8];
	float bullet_speed;
	float bullet_damage;
	float bullet_penetration;
	int reload_time;
	float movement_speed;
	clock_t last_shoot;
	clock_t dead_time;
	std::string killed_by = "";

	using Object::Object;
	Tank() : Object("player", "images/Tank.png") {
		last_shoot = clock() - reload_time;
		exp_for_next_level_up[1] = 4;
		exp_for_next_level_up[2] = 13;
		for (int i = 3; i < 50; i++)
		{
			exp_for_next_level_up[i] = 0.33*pow(i,3)-4.4*i*i+48.8*i-88;
			std::cout << "LEVEL " << i << "\t" << exp_for_next_level_up[i] << "\n";
		}
		reset_stats();
	}

	void reset_stats()
	{
		level = 1;
		exp = 0;
		upgrade_points = 0;
		for (int i = 0; i < 8; i++) stats_levels[i] = 0;
		max_health = 1000;
		health = max_health;
		health_regen = 20;
		damage = 25;
		bullet_speed = 1.0;
		bullet_damage = 15;
		bullet_penetration = 1.0;
		reload_time = 500;
		movement_speed = 5.0;
		spawn_time = clock();
	}
	void dead_stats()
	{
		health = max_health;
		bullet_speed = 1.0;
		bullet_damage = 10;
		bullet_penetration = 1.0;
		reload_time = 500;
		movement_speed = 0;
		position_x = ScreenWidth / 2 + 300;
		position_y = ScreenHeight / 2;
	}

	bool canShoot()
	{
		if (1000.0 * (clock() - last_shoot) / CLOCKS_PER_SEC > reload_time)
		{
			last_shoot = clock();
			return true;
		}
		return false;

	}
	void move()
	{

	}
	bool levelUp()
	{
		if (exp < exp_for_next_level_up[level] || level >= MAX_LEVEL)
			return false;
		level++;
		if(level<MAX_LEVEL)
			upgrade_points++;
		return true;
	}
	void upgrade()
	{
		bullet_damage *= 1.1;
	}

	void upgradeStat(int keycode)
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
				damage *= 2;
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
};

class Bullet : public Object {
public:
	float velocity_x = 0;
	float velocity_y = 0;
	float speed;
	float alive_time = 1000;
	std::vector<Object*> damaged_objects;


	Bullet(float bullet_speed, int bullet_damage, int bullet_penetration, float player_x, float player_y, float mouse_x, float mouse_y) : Object("Bulet", "images/bullet.png") {
		max_health = 100 * bullet_penetration;
		health = 100;
		damage = bullet_damage;
		speed = bullet_speed;
		//alive_time *= bullet_penetration;
		
		position_x = player_x;
		position_y = player_y;
		double vector_length = std::sqrt((mouse_x - player_x) * (mouse_x - player_x) + (mouse_y - player_y) * (mouse_y - player_y));
		velocity_x = double(mouse_x - player_x) / vector_length * 10.0;
		velocity_y = double(mouse_y - player_y) / vector_length * 10.0;
	}

	void move()
	{
		position_x += velocity_x * speed;
		position_y += velocity_y * speed;
	}

	int getAliveTime()
	{
		return 1000.0 * (clock() - spawn_time) / CLOCKS_PER_SEC;
	}

	bool update_damage()
	{
		//if (bul)
	}
	void draw()
	{
		al_draw_scaled_rotated_bitmap(image, offset_x, offset_y, position_x, position_y, 1, 1, rotation, NULL);
	}
};

class Restart : public Object
{
public:
	Restart() : Object("Bulet", "images/missing.png")
	{
		position_x = ScreenWidth / 2;
		position_y = ScreenHeight * 0.75;
	};
};

class UI {
public:
	time_t level_up_show_timer= -2000;

	bool STATS_ON = false;

	float width = 50;
	float height = 30;
	float radius = 15;
	float padding = 5;
	float offset_x = 25;
	float offset_y = ScreenHeight - 316;
	float thick = 2;

	int back_alpha = 175;
	int color_alpha = 150;
	ALLEGRO_COLOR back_color = al_map_rgba(0, 0, 0, back_alpha);
	ALLEGRO_COLOR exp_color = al_map_rgba(108, 255, 108, back_alpha);
	ALLEGRO_COLOR white = al_map_rgba(255, 255, 255, back_alpha);
	ALLEGRO_COLOR stats_colors[8] = { al_map_rgba(238, 182, 144, color_alpha), al_map_rgba(236, 108, 240, color_alpha), al_map_rgba(154, 108, 240, color_alpha), al_map_rgba(108, 150, 240, color_alpha), al_map_rgba(240, 217, 208, color_alpha), al_map_rgba(240, 108, 108, color_alpha), al_map_rgba(152, 240, 108, color_alpha),al_map_rgba(108, 240, 236, color_alpha) };
	ALLEGRO_FONT* title_font = al_load_font("Orbitron Black.ttf", 64, NULL);
	ALLEGRO_FONT* middle_font = al_load_font("Orbitron Black.ttf", 36, NULL);
	ALLEGRO_FONT* stat_font = al_load_font("Orbitron Black.ttf", 20, NULL);
	ALLEGRO_FONT* debug_font = al_load_font("Orbitron Black.ttf", 25, NULL);

	void al_draw_rotated_text(ALLEGRO_FONT* font, ALLEGRO_COLOR color, float x, float y, const char* text)
	{
		ALLEGRO_TRANSFORM original;
		ALLEGRO_TRANSFORM rotated;
		al_identity_transform(&rotated);
		al_rotate_transform(&rotated,  PI / 9);
		al_use_transform(&rotated);
		al_draw_text(font, color, x, y, ALLEGRO_ALIGN_CENTRE, text);
		al_identity_transform(&original);
		al_use_transform(&original);
	}

	void draw_Level_Up()
	{
		if (1000.0 * (clock() - level_up_show_timer) / CLOCKS_PER_SEC < 2000)
			al_draw_textf(title_font, al_map_rgba(255, 255, 255, 50), ScreenWidth / 2, ScreenHeight /2, ALLEGRO_ALIGN_CENTER, "LEVEL UP");
	}

	void draw_level(int exp, int level, int next_level_exp[])
	{
		al_draw_filled_rounded_rectangle(ScreenWidth / 2.0 - 300, ScreenHeight - 75, ScreenWidth / 2.0 + 300, ScreenHeight - 25, 25, 25, back_color);
		al_draw_filled_rounded_rectangle(ScreenWidth / 2.0 - 300 + 2*thick, ScreenHeight - 75 + 2*thick, ScreenWidth / 2.0 - 300 + 50 -2*thick + 550 * fmin(((float)exp-next_level_exp[level-1])/((float)next_level_exp[level]-next_level_exp[level-1]), 1), ScreenHeight - 25 - 2*thick, 25-2*thick, 25-2*thick, exp_color);
		al_draw_textf(middle_font, white, ScreenWidth / 2, ScreenHeight - 65, ALLEGRO_ALIGN_CENTER, "Score: %d", exp);
		
	}

	void draw_stats(int levels[], int upgrade_points, int level)
	{
		if (upgrade_points > 0 || STATS_ON)
		{
			const char* labels[] = { "Health Regen", "Max Health", "Body Damage", "Bullet Speed", "Bullet Penetration", "Bullet Damage", "Reload", "Movement Speed" };
			al_draw_textf(middle_font, white, offset_x, offset_y - height - 2 * thick, ALLEGRO_ALIGN_LEFT, "Tank lvl %d", level);
			if(level!=50)
				al_draw_textf(middle_font, white, offset_x + 5 * width, offset_y - height - 2 * thick, ALLEGRO_ALIGN_LEFT, "x%d", upgrade_points);
			else
				al_draw_text(middle_font, white, offset_x + 5 * width, offset_y - height - 2 * thick, ALLEGRO_ALIGN_LEFT, "MAX");
			for (int stat = 0; stat < 8; stat++)
			{
				al_draw_filled_rounded_rectangle(offset_x, offset_y + stat * (height + 4 * thick), offset_x + 6 * width + 2 * thick, offset_y + stat * (height + 4 * thick) + height, radius, radius, back_color);
				//1
				if (levels[stat])
				{
					al_draw_filled_pieslice(offset_x + radius, offset_y + stat * (height + 4 * thick) + radius, radius - thick, 0.5 * PI, PI, stats_colors[stat]);
					al_draw_filled_rectangle(offset_x + radius, offset_y + stat * (height + 4 * thick) + thick, offset_x + width, offset_y + stat * (height + 4 * thick) + height - thick, stats_colors[stat]);
				}
				//2-5
				for (int i = 1; i < (levels[stat] == 6 ? 5 : levels[stat]); i++)
					al_draw_filled_rectangle(offset_x + i * width + 2 * thick, offset_y + stat * (height + 4 * thick) + thick, offset_x + (i + 1) * width, offset_y + stat * (height + 4 * thick) + height - thick, stats_colors[stat]);
				//6
				if (levels[stat] == 6)
				{
					al_draw_filled_rectangle(offset_x + 5 * width + 2 * thick, offset_y + stat * (height + 4 * thick) + thick, offset_x + 6 * width - radius + 2 * thick, offset_y + stat * (height + 4 * thick) + height - thick, stats_colors[stat]);
					al_draw_filled_pieslice(offset_x + 6 * width - radius + 2 * thick, offset_y + stat * (height + 4 * thick) + radius, radius - thick, 1.5 * PI, PI, stats_colors[stat]);
				}
				//LABELS
				al_draw_text(stat_font, white, offset_x + 3 * width + thick, offset_y + stat * (height + 4 * thick) + height / 4.0, ALLEGRO_ALIGN_CENTER, labels[stat]);
				al_draw_textf(stat_font, white, offset_x + 5.5 * width + thick, offset_y + stat * (height + 4 * thick) + height / 4.0, ALLEGRO_ALIGN_CENTER, "[%d]", stat + 1);
			}
		}
		
	}
	void debug_stats(int player_health, int health_regen, int max_health, int body_damage, float bullet_speed, float bullet_penetration, int bullet_damage, int reload, float movement_speed, int exp, int level, int next_level_exp[])
	{
		//health_regen
		al_draw_textf(debug_font, al_map_rgb(0, 0, 0), 25, 25, ALLEGRO_ALIGN_LEFT, "[1] Heath Regen:    %d", health_regen);

		//max_health
		al_draw_textf(debug_font, al_map_rgb(0, 0, 0), 25, 50, ALLEGRO_ALIGN_LEFT, "[2] Max Health:    %d", max_health);

		//body_damage
		al_draw_textf(debug_font, al_map_rgb(0, 0, 0), 25,75, ALLEGRO_ALIGN_LEFT, "[3] Body Damage:    %d", body_damage);

		//bullet_speed
		al_draw_textf(debug_font, al_map_rgb(0, 0, 0), 25, 100, ALLEGRO_ALIGN_LEFT, "[4] Bullet Speed:    %.2f", bullet_speed);

		//bullet_penetration
		al_draw_textf(debug_font, al_map_rgb(0, 0, 0), 25, 125, ALLEGRO_ALIGN_LEFT, "[5] Bullet Penetration:    %.2f", bullet_penetration);

		//bullet_damage
		al_draw_textf(debug_font, al_map_rgb(0, 0, 0), 25, 150, ALLEGRO_ALIGN_LEFT, "[6] Bullet Damage:    %d", bullet_damage);

		//reload
		al_draw_textf(debug_font, al_map_rgb(0, 0, 0), 25, 175, ALLEGRO_ALIGN_LEFT, "[7] Reload:    %d", reload);

		//movement_speed
		al_draw_textf(debug_font, al_map_rgb(0, 0, 0), 25,200, ALLEGRO_ALIGN_LEFT, "[8] Movement Speed:    %.2f", movement_speed);

		//PLAYER_HEALTH
		al_draw_textf(debug_font, al_map_rgb(0, 0, 0), 25, 250, ALLEGRO_ALIGN_LEFT, "HEALTH:    %d/%d", player_health, max_health);

		//EXP to level up
		al_draw_textf(debug_font, al_map_rgb(0, 0, 0), 25, 275, ALLEGRO_ALIGN_LEFT, "EXP to levelUP:    %d", next_level_exp[level] - exp);
	}
};