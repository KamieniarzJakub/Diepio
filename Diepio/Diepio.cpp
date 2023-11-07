#include <iostream>
#include <string>
#include <cmath>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h> //ONLY FOR TESTING
#include <allegro5/allegro_image.h>


#define ScreenWidth 1920
#define ScreenHeight 1080


class Tank {
//protected:
public:
	std::string name;
	int health = 100;
	int damage = 10;
	float position_x = ScreenWidth / 2.0;
	float position_y = ScreenHeight / 2.0;
	//std::string image = "Tank.png";
	float rotation = 0;

public:
	ALLEGRO_BITMAP* image = al_load_bitmap("Tank.png");

	int level = 1;
	int exp = 0;
	float health_regen = 1.0;
	float bullet_speed = 10.0;
	int bullet_damage = 10;
	int bullet_penetration = 10; //NOT IN USE YET
	float reload_time = 1.0;
	float movement_speed = 5.0;

//protected:
	void draw()
	{
		
	}
	void getDamage(int damage)
	{
		health -= damage;
	}
	void dealDamage(int damage)
	{

	}

public: //protected?
	void shoot()
	{

	}
	void move()
	{

	}
	void upgrade()
	{

	}
	void repawn()
	{

	}
};


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
	

	ALLEGRO_COLOR electricBlue = al_map_rgb(44, 117, 255);
	ALLEGRO_COLOR yellow = al_map_rgb(255, 255, 0);
	ALLEGRO_COLOR playerColor = electricBlue;

	Tank player;

	ALLEGRO_BITMAP* player_tank = al_load_bitmap("Tank.png");
	ALLEGRO_BITMAP* player_bullet = al_load_bitmap("bullet.png");

	ALLEGRO_FONT* font = al_load_font("Orbitron Black.ttf", 36, NULL);
	al_draw_text(font, al_map_rgb(44, 117, 255), ScreenWidth / 2, ScreenHeight / 2, ALLEGRO_ALIGN_CENTER, "DIEP.IO");
	//al_flip_display();
	//al_rest(5.0);
	
	bool done = false;
	double mouse_x = 0, mouse_y = 0;
	double bullet_x=player.position_x, bullet_y=player.position_y;
	double theta=0;
	double bulletVelocity[] = { 0, 0 };
	double moveSpeed = 5;

	const float FPS = 60.0;
	ALLEGRO_TIMER* timer = al_create_timer(1.0/FPS);
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_start_timer(timer);

	//game loop
	while (!done)
	{
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);
		if (events.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			mouse_x = events.mouse.x;
			mouse_y = events.mouse.y;
			player.rotation = atan2((mouse_y - player.position_y),(mouse_x - player.position_x));
		}
		else if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (events.mouse.button & 1)
			{
				bullet_x = player.position_x;
				bullet_y = player.position_y;
				double vector_length = std::sqrt((mouse_x - player.position_x) * (mouse_x - player.position_x) + (mouse_y - player.position_y) * (mouse_y - player.position_y));
				bulletVelocity[0] = double(mouse_x- player.position_x)/vector_length*10.0;
				bulletVelocity[1] = double(mouse_y- player.position_y)/vector_length*10.0;
				playerColor = electricBlue;
			}	
			else if (events.mouse.button & 2)
			{
				playerColor = yellow;
				
			}
		}
		if (events.type == ALLEGRO_EVENT_TIMER)
		{
			al_get_keyboard_state(&keyState);
			if (al_key_down(&keyState, ALLEGRO_KEY_DOWN) || al_key_down(&keyState, ALLEGRO_KEY_S)) player.position_y += moveSpeed;
			else if (al_key_down(&keyState, ALLEGRO_KEY_UP) || al_key_down(&keyState, ALLEGRO_KEY_W)) player.position_y -= moveSpeed;
			if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT) || al_key_down(&keyState, ALLEGRO_KEY_D)) player.position_x += moveSpeed;
			else if (al_key_down(&keyState, ALLEGRO_KEY_LEFT) || al_key_down(&keyState, ALLEGRO_KEY_A)) player.position_x -= moveSpeed;
			bullet_x += bulletVelocity[0];
			bullet_y += bulletVelocity[1];
		}
		if (events.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (events.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
			}
		}
		else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			done = true;
		//al_draw_scaled_rotated_bitmap(player, 75, 50, x, y, 1, 1, theta, NULL);
		al_draw_scaled_rotated_bitmap(player_tank, 75, 50, player.position_x, player.position_y, 1, 1, player.rotation, NULL);
		
		al_draw_scaled_rotated_bitmap(player_bullet, 25, 25, bullet_x, bullet_y, 1, 1, 0, NULL);
		
		al_draw_rectangle(mouse_x - 10, mouse_y-10, mouse_x + 10, mouse_y + 10, playerColor, 2.0);
		al_draw_line(player.position_x, player.position_y, mouse_x, mouse_y, yellow, 3);
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}

	al_destroy_timer(timer);
	al_destroy_font(font);
	al_destroy_bitmap(player.image);
	al_destroy_bitmap(player_bullet);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}