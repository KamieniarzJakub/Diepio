#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "Classes.cpp"
#include "GameManager.h"
#include "Mouse.h"
#include "UI.h"
#include "Object.h"
#include "Tank.h"
#include "Enemy.h"
#include "Polygon.h"
#include "Bullet.h"

#define ScreenWidth 1920
#define ScreenHeight 1080



int main()
{

	if (!al_init())
	{
		al_show_native_message_box(NULL, NULL, NULL, "Could not initialize Allegro 5", NULL, NULL);
		return -1;
	}

	//al_set_new_display_flags(ALLEGRO_FULLSCREEN);
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
	Tank player("Player", "images/Tank.png");
	Enemy enemy(player.getLevel());
	UI canvas;
	Object reset("RESTART", "images/RESET.png");
	
	std::list<Bullet> bullets;
	std::list<Bullet> enemy_bullets;
	std::list<Polygon> polygons;

	ALLEGRO_FONT* font = al_load_font("Orbitron Black.ttf", 36, NULL);
	al_draw_text(font, al_map_rgb(44, 117, 255), ScreenWidth / 2, ScreenHeight / 2, ALLEGRO_ALIGN_CENTER, "DIEP.IO");
	al_flip_display();
	al_rest(1.0);
	
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
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);
		if (events.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			mouse.set_position(events.mouse.x, events.mouse.y);
		}
		//game loop
		if (true)
		{
			if (events.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				if (player.getUpgradePoints())
					player.upgradeStat(events.keyboard.keycode);
				switch (events.keyboard.keycode)
				{
				case ALLEGRO_KEY_TAB:
					canvas.show_stats();
					break;
				case ALLEGRO_KEY_ESCAPE:
					quit = true;
					break;
				}
			}
			else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				quit = true;
			if (events.type == ALLEGRO_EVENT_TIMER)
			{
				al_get_keyboard_state(&keyState);
				al_get_mouse_state(&mouseState);
				if ((al_key_down(&keyState, ALLEGRO_KEY_DOWN) || al_key_down(&keyState, ALLEGRO_KEY_S)) && player.get_pos_y() < ScreenHeight)  player.set_pos_y(player.get_pos_y()+player.getMovementSpeed());
				else if ((al_key_down(&keyState, ALLEGRO_KEY_UP) || al_key_down(&keyState, ALLEGRO_KEY_W)) && player.get_pos_y() > 0) player.set_pos_y(player.get_pos_y() - player.getMovementSpeed());
				if ((al_key_down(&keyState, ALLEGRO_KEY_RIGHT) || al_key_down(&keyState, ALLEGRO_KEY_D)) && player.get_pos_x() < ScreenWidth) player.set_pos_x(player.get_pos_x() + player.getMovementSpeed());
				else if ((al_key_down(&keyState, ALLEGRO_KEY_LEFT) || al_key_down(&keyState, ALLEGRO_KEY_A)) && player.get_pos_x()>0) player.set_pos_x(player.get_pos_x()-player.getMovementSpeed());
				if ((al_mouse_button_down(&mouseState, 1) || al_key_down(&keyState, ALLEGRO_KEY_SPACE)) && player.canShoot())
				{
					Bullet bullet(player, mouse.get_pos_x(), mouse.get_pos_y(), "images/Bullet.png");
					bullets.push_front(bullet);
				}
				//enemy movement
				if (enemy.isON())
				{
					if (enemy.getDistance(player.get_pos_x(), player.get_pos_y()) > 300)
						enemy.move(player.get_pos_x(), player.get_pos_y());
					enemy.setRotation(atan2((player.get_pos_y() - enemy.get_pos_y()), (player.get_pos_x() - enemy.get_pos_x())));
					if (enemy.getDistance(player.get_pos_x(), player.get_pos_y()) < 1000 && enemy.canShoot())
					{
						Bullet enemy_bullet(enemy, player.get_pos_x(), player.get_pos_y(), "images/EnemyBullet.png");
						enemy_bullets.push_front(enemy_bullet);
					}
				}
				for (auto it = bullets.begin(); it != bullets.end(); ++it)
				{
					it->move();
					it->heal();
				}
				for (auto it_enemy = enemy_bullets.begin(); it_enemy != enemy_bullets.end(); ++it_enemy)
				{
					it_enemy->move();
					it_enemy->heal();
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
					Polygon square("Square", "images/Square.png", 2500, 25);
					Polygon pentagon("Pentagon", "images/Pentagon.png", 10000, 100);
					Polygon hexagon("Hexagon", "images/Hexagon.png", 20000, 200);
					Polygon octagon("Octagon", "images/Octagon.png", 50000, 500);
					Polygon decagon("Decagon", "images/Decagon.png", 100000, 1000);
					if (rand() % 2 + player.getLevel() == 45)
						polygons.push_front(decagon);
					else if (rand() % 100 + player.getLevel() > 100)
						polygons.push_front(octagon);
					else if (rand() % 50 + 2 * player.getLevel() > 75)
						polygons.push_front(hexagon);
					else if (rand() % 50 < 10)
						polygons.push_front(pentagon);
					else if (rand() % 10 < 4)
						polygons.push_front(square);
					else
						polygons.push_front(triangle);
				}
				auto it_poly = polygons.begin();
				while (it_poly != polygons.end() && game_in_progress)
				{
					//player bullets
					auto it = bullets.begin();
					while (it != bullets.end())
					{
						if (it_poly->Collision(*it))
						{
							if (it_poly->tryToKill(it->getBodyDamage()))
							{
								it_poly->Kill();
								it_poly->setKillerName(player.getName());
								break;
							}
							else if (it->tryToKill(it_poly->getBodyDamage()))
							{
								it = bullets.erase(it);
							}
						}
						if (it != bullets.end()) it++;
					}
					//enemy bullets
					auto it_enemy = enemy_bullets.begin();
					while (it_enemy != enemy_bullets.end())
					{
						if (it_poly->Collision(*it_enemy))
						{
							if (it_poly->tryToKill(it_enemy->getBodyDamage()))
							{
								it_poly->Kill();
								break;
							}
							else if (it_enemy->tryToKill(it_poly->getBodyDamage()))
							{
								it_enemy = enemy_bullets.erase(it_enemy);
							}
						}
						if (it_enemy != enemy_bullets.end()) it_enemy++;
					}
					//player collision
					if (it_poly->Collision(player))
					{
						if (it_poly->tryToKill(player.getBodyDamage()))
							it_poly->Kill();
						else if (player.tryToKill(it_poly->getBodyDamage()))
						{
							player.setKillerName(it_poly->getName());
							game_in_progress = false;
							player.setDeadStats();
						}
					}
					//enemy collsion
					if (it_poly->Collision(enemy))
					{
						if (it_poly->tryToKill(enemy.getBodyDamage()))
							it_poly->Kill();
						if (it_poly->canDealDamaged(clock(), 1000) && enemy.tryToKill(it_poly->getBodyDamage()))
						{
							enemy.setKillerName(it_poly->getName());
							enemy.reset_stats(player.getLevel());
						}
						
					}
					if (it_poly->isDead())
					{
						if (it_poly->getKillerName() == player.getName())
							player.setExp(player.getExp() + it_poly->getExp());
						it_poly = polygons.erase(it_poly);
						gameManager.numer_of_obstacles--;
					}
					if (it_poly != polygons.end()) it_poly++;
				}
				//player bullets collision with enemy
				auto it = bullets.begin();
				while (it != bullets.end() && game_in_progress)
				{
					if (it->Collision(enemy))
					{
						if (enemy.tryToKill(it->getBodyDamage()))
						{
							enemy.reset_stats(player.getLevel());
						}
						else if (it->tryToKill(enemy.getBodyDamage()))
						{
							it = bullets.erase(it);
						}
					}
					if (it != bullets.end()) it++;
				}
				auto it_enemy = enemy_bullets.begin();
				while (it_enemy != enemy_bullets.end() && game_in_progress)
				{
					if (it_enemy->Collision(player))
					{
						if (player.tryToKill(it_enemy->getBodyDamage()))
						{
							player.setKillerName(enemy.getName());
							game_in_progress = false;
							player.setDeadStats();
						}
						else if (it_enemy->tryToKill(player.getBodyDamage()))
						{
							it_enemy = enemy_bullets.erase(it_enemy);
						}
					}
					if (it_enemy != enemy_bullets.end()) it_enemy++;
				}
				if (player.Collision(enemy))
				{
					if (enemy.tryToKill(player.getBodyDamage()))
					{
						enemy.reset_stats(player.getLevel());
					}
					else if (player.tryToKill(enemy.getBodyDamage()))
					{
						player.setKillerName(enemy.getName());
						game_in_progress = false;
						player.setDeadStats();
					}
				}
			}

			player.setRotation(atan2((mouse.get_pos_y() - player.get_pos_y()), (mouse.get_pos_x() - player.get_pos_x())));
			for (auto it_poly = polygons.begin(); it_poly != polygons.end(); ++it_poly)
			{
				it_poly->draw();
			}
			for (auto it_enemy = enemy_bullets.begin(); it_enemy != enemy_bullets.end(); ++it_enemy)
			{
				it_enemy->draw();
			}
			if (enemy_bullets.size() && enemy_bullets.back().getAliveTime() > enemy_bullets.back().getMaxLifespan())
				enemy_bullets.pop_back();
			if(enemy.isON())
				enemy.draw();
			if (game_in_progress)
			{
				if (player.levelUp())
				{
					canvas.start_level_up_timer();
				}
				player.heal();
				canvas.draw_Level_Up();
				canvas.draw_level(player.getExp(), player.getLevel(), player.getExpForNextLevel());
				canvas.draw_stats(player.getStatsLevels(), player.getUpgradePoints(), player.getLevel());
				al_draw_rectangle(mouse.get_pos_x() - 10, mouse.get_pos_y() - 10, mouse.get_pos_x() + 10, mouse.get_pos_y() + 10, playerColor, 2.0);
				//al_draw_line(player.position_x, player.position_y, mouse.position_x, mouse.position_y, yellow, 3);
				//al_flip_display();
			}
			al_hide_mouse_cursor(display);
		}
		if(!game_in_progress)
		{
			al_draw_filled_rectangle(0, 0, ScreenWidth, ScreenHeight, al_map_rgba(5, 5, 5, 150));
			al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth / 2, ScreenHeight / 2 - 200, ALLEGRO_ALIGN_CENTER, "You were killed by:");
			al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth / 2, ScreenHeight / 2 - 150, ALLEGRO_ALIGN_CENTER, player.getKillerName().c_str());
			al_draw_textf(font, al_map_rgb(255, 255, 255), ScreenWidth / 2 - 300, ScreenHeight / 2 - 50, ALLEGRO_ALIGN_LEFT, "Score: %d", player.getExp());
			al_draw_textf(font, al_map_rgb(255, 255, 255), ScreenWidth / 2 - 300, ScreenHeight / 2, ALLEGRO_ALIGN_LEFT, "Level: %d", player.getLevel());
			al_draw_textf(font, al_map_rgb(255, 255, 255), ScreenWidth / 2 - 300, ScreenHeight / 2 + 50, ALLEGRO_ALIGN_LEFT, "Time: %d m %d s", (player.getDeadTime() - player.getSpawnTime()) / CLOCKS_PER_SEC / 60, (player.getDeadTime() - player.getSpawnTime()) / CLOCKS_PER_SEC % 60);
			//player.position_x = ScreenWidth / 2 + 400;
			//player.position_y = ScreenHeight / 2;
			//al_draw_rectangle(mouse.position_x - 10, mouse.position_y - 10, mouse.position_x + 10, mouse.position_y + 10, playerColor, 2.0);
			reset.set_pos_x(ScreenWidth / 2);
			reset.set_pos_y(ScreenHeight * 0.75);
			reset.draw();
			auto it = bullets.begin();
			while (it != bullets.end())
			{
				if (it->Collision(reset) && reset.tryToKill(it->getBodyDamage()))
				{
					reset.newGame();
					game_in_progress = true;
					player.reset_stats();
					polygons.clear();
					bullets.clear();
					enemy_bullets.clear();
					gameManager.numer_of_obstacles = 0;
					if(enemy.isON())
						enemy.reset_stats(player.getLevel());
					break;
				}
				if (it != bullets.end()) it++;
			}
			
			al_show_mouse_cursor(display);
		}

		for (auto it = bullets.begin(); it != bullets.end(); ++it)
		{
			it->draw();
		}
		if (bullets.size() && bullets.back().getAliveTime() > bullets.back().getMaxLifespan())
			bullets.pop_back();
			

		player.draw();
			
		al_flip_display();
		al_clear_to_color(al_map_rgb(205, 205, 205));
	}
	al_destroy_timer(timer);
	al_destroy_font(font);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}