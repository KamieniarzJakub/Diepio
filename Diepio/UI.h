#include <time.h>
#include <cmath>
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include "Tank.h"

#define ScreenWidth 1920
#define ScreenHeight 1080

#define PI 3.141592

class UI {
private:
	time_t level_up_show_timer = -2000;
	bool stats;
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

public:
	UI();
	//always showing stats switch
	void show_stats();
	
	//animation timer
	void start_level_up_timer();

	//drawing rotated text (not implemented in default allegro library)
	void al_draw_rotated_text(ALLEGRO_FONT* font, ALLEGRO_COLOR color, float x, float y, const char* text);
	
	//drawing level up text
	void draw_Level_Up();

	//Drawing player score
	void draw_level(int exp, int level, int next_level_exp[]);

	//drawing stats menu
	void draw_stats(int levels[], int upgrade_points, int level);
};