#include <iostream>
#include <string>
#include <cmath>
#include <list>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h> //ONLY FOR TESTING
#include <allegro5/allegro_image.h>

#include "Classes.cpp"


#define ScreenWidth 1920
#define ScreenHeight 1080



int main()
{

	if (!al_init())
	{
		al_show_native_message_box(NULL, NULL, NULL, "Could not initialize Allegro 5", NULL, NULL);
		return -1;
	}

	//al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW | ALLEGRO_RESIZABLE);
	//al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
	ALLEGRO_DISPLAY* display = al_create_display(ScreenWidth, ScreenHeight);
	al_set_window_position(display, -10, 0);
	al_set_window_title(display, "Diep.io");
	al_hide_mouse_cursor(display);

	if (!display)
	{
		al_show_native_message_box(display, "Sample title", "Display settings", "Display window was not created successfully", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon(); 
	al_init_image_addon();

	al_install_keyboard();
	al_install_mouse();

	ALLEGRO_KEYBOARD_STATE keyState;
	ALLEGRO_MOUSE_STATE mouseState;
	

	ALLEGRO_COLOR electricBlue = al_map_rgb(44, 117, 255);
	ALLEGRO_COLOR yellow = al_map_rgb(255, 255, 0);
	ALLEGRO_COLOR green = al_map_rgb(0, 255, 0);
	ALLEGRO_COLOR playerColor = electricBlue;

	srand(clock());

	GameManager gameManager;
	Mouse mouse;
	Tank player;
	UI canvas;
	
	//Polygon rectangle("Rectangle", "images/Rectangle.png");
	//Polygon pentagon("Pentagon", "images/Pentagon.png");
	std::list<Bullet> bullets;
	std::list<Polygon> polygons;

	ALLEGRO_FONT* font = al_load_font("Orbitron Black.ttf", 36, NULL);
	al_draw_text(font, al_map_rgb(44, 117, 255), ScreenWidth / 2, ScreenHeight / 2, ALLEGRO_ALIGN_CENTER, "DIEP.IO");
	//al_flip_display();
	//al_rest(5.0);
	
	bool quit = false;
	bool game_in_progress = true;
	const float FPS = 60.0;
	ALLEGRO_TIMER* timer = al_create_timer(1.0/FPS);
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_start_timer(timer);


	while (!quit)
	{
		//game loop
		while (game_in_progress)
		{
			ALLEGRO_EVENT events;
			al_wait_for_event(event_queue, &events);
			if (events.type == ALLEGRO_EVENT_MOUSE_AXES)
			{
				mouse.position_x = events.mouse.x;
				mouse.position_y = events.mouse.y;
			}
			else if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (events.mouse.button & 1)
				{

					playerColor = electricBlue;
				}
				else if (events.mouse.button & 2)
				{
					playerColor = yellow;
					player.upgrade();
				}
			}
			if (events.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				if (player.upgrade_points > 0)
					player.upgradeStat(events.keyboard.keycode);
				switch (events.keyboard.keycode)
				{
				case ALLEGRO_KEY_TAB:
					canvas.STATS_ON = !canvas.STATS_ON;
					break;
				case ALLEGRO_KEY_ESCAPE:
					game_in_progress = false;
					break;
				}
			}
			else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				game_in_progress = false;
			if (events.type == ALLEGRO_EVENT_TIMER)
			{
				al_get_keyboard_state(&keyState);
				al_get_mouse_state(&mouseState);
				if (al_key_down(&keyState, ALLEGRO_KEY_DOWN) || al_key_down(&keyState, ALLEGRO_KEY_S)) player.position_y += player.movement_speed;
				else if (al_key_down(&keyState, ALLEGRO_KEY_UP) || al_key_down(&keyState, ALLEGRO_KEY_W)) player.position_y -= player.movement_speed;
				if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT) || al_key_down(&keyState, ALLEGRO_KEY_D)) player.position_x += player.movement_speed;
				else if (al_key_down(&keyState, ALLEGRO_KEY_LEFT) || al_key_down(&keyState, ALLEGRO_KEY_A)) player.position_x -= player.movement_speed;
				if ((al_mouse_button_down(&mouseState, 1) || al_key_down(&keyState, ALLEGRO_KEY_SPACE)) && player.canShoot())
				{
					Bullet bullet(player.bullet_speed, player.bullet_damage, player.bullet_penetration, player.position_x, player.position_y, mouse.position_x, mouse.position_y);
					bullets.push_front(bullet);
				}
				for (auto it = bullets.begin(); it != bullets.end(); ++it)
				{
					it->move();
					it->heal();
				}
				for (auto it = polygons.begin(); it != polygons.end(); ++it)
				{
					it->rotate();
					it->heal();
				}

				if (gameManager.canSpawnObstacle())
				{
					gameManager.last_time_obstacle_spawned = clock();
					Polygon triangle("Triangle", "images/Triangle.png", 1000, 10);
					Polygon square("Rectangle", "images/Rectangle.png", 2000, 25);
					Polygon polygon("Pentagon", "images/Pentagon.png", 5000, 130);
					if (rand() % 2)
						polygons.push_front(triangle);
					else if (rand() % 2)
						polygons.push_front(square);
					else
						polygons.push_front(polygon);
				}
				auto it_poly = polygons.begin();
				while (it_poly != polygons.end())
				{
					bool died = false;

					auto it = bullets.begin();
					while (it != bullets.end())
					{
						if (it_poly->Collision(*it))
						{
							if (it_poly->getDamage(it->damage))
							{
								died = true;
								break;
							}
							else if (it->getDamage(it_poly->damage))
							{
								it = bullets.erase(it);
							}

						}
						if (it != bullets.end()) it++;
					}
					if (it_poly->Collision(player))
					{
						if (it_poly->getDamage(player.damage))
							died = true;
						else if (player.getDamage(it_poly->damage))
						{
							player.killed_by = it_poly->name;
							player.dead_time = clock();
							std::cout << it_poly->name << "\n";
							game_in_progress = false;
						}
					}
					if (died)
					{
						player.exp += it_poly->exp;
						it_poly = polygons.erase(it_poly);
						gameManager.numer_of_obstacles--;
					}
					if (it_poly != polygons.end()) it_poly++;
				}
			}

			//al_draw_scaled_rotated_bitmap(player, 75, 50, x, y, 1, 1, theta, NULL);
			//al_draw_scaled_rotated_bitmap(player.image, 75, 50, player.position_x, player.position_y, 1, 1, player.rotation, NULL);

			//triangle.draw();
			//rectangle.draw();
			//pentagon.draw();
			//al_draw_filled_circle(triangle.position_x, triangle.position_y, std::max(triangle.offset_x, triangle.offset_y), green);
			//al_draw_filled_circle(rectangle.position_x, rectangle.position_y, std::max(rectangle.offset_x, rectangle.offset_y), green);
			//al_draw_filled_circle(pentagon.position_x, pentagon.position_y, std::max(pentagon.offset_x, pentagon.offset_y), green);
			//al_draw_line(triangle.position_x - triangle.health / 2, triangle.position_y + triangle.offset_y + 15, triangle.position_x + triangle.health / 2, triangle.position_y + triangle.offset_y + 15, green, 10);
			//al_draw_line(rectangle.position_x - rectangle.health / 2, rectangle.position_y + rectangle.offset_y + 15, rectangle.position_x + rectangle.health / 2, rectangle.position_y + rectangle.offset_y + 15, green, 10);
			//al_draw_line(pentagon.position_x - pentagon.health / 2, pentagon.position_y + pentagon.offset_y + 15, pentagon.position_x + pentagon.health / 2, pentagon.position_y + pentagon.offset_y + 15, green, 10);

			player.rotation = atan2((mouse.position_y - player.position_y), (mouse.position_x - player.position_x));
			player.heal();
			player.draw();
			for (auto it = polygons.begin(); it != polygons.end(); ++it)
			{
				it->draw();
			}
			for (auto it = bullets.begin(); it != bullets.end(); ++it)
			{
				it->draw();
			}
			if (bullets.size() && bullets.back().getAliveTime() > bullets.back().alive_time)
				bullets.pop_back();

			if (player.levelUp())
			{
				canvas.level_up_show_timer = clock();
			}
			canvas.draw_Level_Up();
			canvas.draw_level(player.exp, player.level, player.exp_for_next_level_up);
			canvas.draw_stats(player.stats_levels, player.upgrade_points, player.level);

			canvas.debug_stats(player.health, player.health_regen, player.max_health, player.damage, player.bullet_speed, player.bullet_penetration, player.bullet_damage, player.reload_time, player.movement_speed, player.exp, player.level, player.exp_for_next_level_up);

			al_draw_rectangle(mouse.position_x - 10, mouse.position_y - 10, mouse.position_x + 10, mouse.position_y + 10, playerColor, 2.0);
			al_draw_line(player.position_x, player.position_y, mouse.position_x, mouse.position_y, yellow, 3);
			al_flip_display();
			al_clear_to_color(al_map_rgb(205, 205, 205));
		}

		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);
		if (events.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (events.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				quit = true;
				break;
			}
		}
		else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			quit = true;
		if (events.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			mouse.position_x = events.mouse.x;
			mouse.position_y = events.mouse.y;
		}
		al_draw_text(font, al_map_rgb(0, 0, 0), ScreenWidth / 2, ScreenHeight / 2-200, ALLEGRO_ALIGN_CENTER, "You were killed by:");
		al_draw_text(font, al_map_rgb(0, 0, 0), ScreenWidth / 2, ScreenHeight / 2-150, ALLEGRO_ALIGN_CENTER, player.killed_by.c_str());
		al_draw_textf(font, al_map_rgb(0, 0, 0), ScreenWidth / 2-300, ScreenHeight / 2-50, ALLEGRO_ALIGN_LEFT, "Score: %d", player.exp);
		al_draw_textf(font, al_map_rgb(0, 0, 0), ScreenWidth / 2-300, ScreenHeight / 2, ALLEGRO_ALIGN_LEFT, "Level: %d", player.level);
		//al_draw_textf(font, al_map_rgb(0, 0, 0), ScreenWidth / 2-300, ScreenHeight / 2+50, ALLEGRO_ALIGN_LEFT, "Time: %f m %f s", 1000.0*float(player.dead_time-player.spawn_time)/CLOCKS_PER_SEC/60000), (float(player.dead_time-player.spawn_time)/CLOCKS_PER_SEC);
		std::cout << player.dead_time - player.spawn_time << "     " << (player.dead_time - player.spawn_time) / CLOCKS_PER_SEC << "    " << ((player.dead_time - player.spawn_time) / CLOCKS_PER_SEC) << "\n";
		//al_draw_rectangle(mouse.position_x - 10, mouse.position_y - 10, mouse.position_x + 10, mouse.position_y + 10, playerColor, 2.0);
		al_flip_display();
		//al_clear_to_color(al_map_rgb(205, 205, 205));
		al_show_mouse_cursor(display);
	}
	al_destroy_timer(timer);
	al_destroy_font(font);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}