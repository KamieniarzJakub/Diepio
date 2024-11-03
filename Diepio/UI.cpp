#include "UI.h"
UI::UI()
{
	stats = false;
	level_up_show_timer = -2000;
	width = 50;
	height = 30;
	radius = 15;
	padding = 5;
	offset_x = 25;
	offset_y = ScreenHeight - 316;
	thick = 2;
	back_alpha = 175;
	color_alpha = 150;
	back_color = al_map_rgba(0, 0, 0, back_alpha);
	exp_color = al_map_rgba(108, 255, 108, back_alpha);
	white = al_map_rgba(255, 255, 255, back_alpha);
	stats_colors[0] = al_map_rgba(238, 182, 144, color_alpha);
	stats_colors[1] = al_map_rgba(236, 108, 240, color_alpha);
	stats_colors[2] = al_map_rgba(154, 108, 240, color_alpha);
	stats_colors[3] = al_map_rgba(108, 150, 240, color_alpha);
	stats_colors[4] = al_map_rgba(240, 217, 208, color_alpha);
	stats_colors[5] = al_map_rgba(240, 108, 108, color_alpha);
	stats_colors[6] = al_map_rgba(152, 240, 108, color_alpha);
	stats_colors[7] = al_map_rgba(108, 240, 236, color_alpha);
	title_font = al_load_font("Orbitron Black.ttf", 64, NULL);
	middle_font = al_load_font("Orbitron Black.ttf", 36, NULL);
	stat_font = al_load_font("Orbitron Black.ttf", 20, NULL);
	debug_font = al_load_font("Orbitron Black.ttf", 25, NULL);
}

	
void UI::show_stats()
{
	stats = !stats;
}

void UI::start_level_up_timer()
{
	level_up_show_timer = clock();
}

void UI::al_draw_rotated_text(ALLEGRO_FONT* font, ALLEGRO_COLOR color, float x, float y, const char* text)
{
	ALLEGRO_TRANSFORM original;
	ALLEGRO_TRANSFORM rotated;
	al_identity_transform(&rotated);
	al_rotate_transform(&rotated, PI / 9);
	al_use_transform(&rotated);
	al_draw_text(font, color, x, y, ALLEGRO_ALIGN_CENTRE, text);
	al_identity_transform(&original);
	al_use_transform(&original);
}

void UI::draw_Level_Up()
{
	if (1000.0 * (clock() - level_up_show_timer) / CLOCKS_PER_SEC < 2000)
		al_draw_textf(title_font, al_map_rgba(255, 255, 255, 50), ScreenWidth / 2, ScreenHeight / 2, ALLEGRO_ALIGN_CENTER, "LEVEL UP");
}

void UI::draw_level(int exp, int level, int next_level_exp[])
{
	al_draw_filled_rounded_rectangle(ScreenWidth / 2.0 - 300, ScreenHeight - 75, ScreenWidth / 2.0 + 300, ScreenHeight - 25, 25, 25, back_color);
	al_draw_filled_rounded_rectangle(ScreenWidth / 2.0 - 300 + 2.0 * thick, ScreenHeight - 75 + 2 * thick, ScreenWidth / 2.0 - 300 + 50 - 2.0 * thick + 550 * fmin(((float)exp - next_level_exp[level - 1]) / ((float)next_level_exp[level] - next_level_exp[level - 1]), 1), ScreenHeight - 25 - 2 * thick, 25 - 2 * thick, 25 - 2 * thick, exp_color);
	al_draw_textf(middle_font, white, ScreenWidth / 2, ScreenHeight - 65, ALLEGRO_ALIGN_CENTER, "Score: %d", exp);
}

void UI::draw_stats(int levels[], int upgrade_points, int level)
{
	if (upgrade_points > 0 || stats)
	{
		const char* labels[] = { "Health Regen", "Max Health", "Body damage", "Bullet Speed", "Bullet Penetration", "Bullet damage", "Reload", "Movement Speed" };
		al_draw_textf(middle_font, white, offset_x, offset_y - height - 2 * thick, ALLEGRO_ALIGN_LEFT, "Tank lvl %d", level);
		if (level != 50)
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

	
