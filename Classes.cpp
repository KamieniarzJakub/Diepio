#define ScreenWidth 1920
#define ScreenHeight 1080

class Object {
public:
	std::string name;
	int health = 100;
	int damage = 10;
	float position_x;
	float position_y;
	std::string image_path;
	ALLEGRO_BITMAP* image = al_load_bitmap("missing/Tank.png");
	float rotation = 0;

	Object(std::string set_name, std::string set_image_path)
	{
		name = set_name;
		image_path = set_image_path;
		position_x = ScreenWidth / 4.0;
		position_y = ScreenHeight / 4.0;
	}

	void draw()
	{
		al_draw_scaled_rotated_bitmap(image, 25, 25, position_x, position_y, 1, 1, 0, NULL);
	}
};

class Polygon {
	//protected:
public:
	std::string name;
	int health = 100;
	int damage = 10;
	float position_x = ScreenWidth / 4.0;
	float position_y = ScreenHeight / 4.0;
	//std::string image = "Tank.png";
	float rotation = 0;

public:
	int exp = 500;

	void spawn() //
	{

	}
};

class Tank : Object {
	//protected:
public:

	std::string name;
	int health = 100;
	int damage = 10;
	float position_x = ScreenWidth / 2.0;
	float position_y = ScreenHeight / 2.0;
	std::string image_path = "images/Tank.png";
	float rotation = 0;

	using Object::Object;
	Tank() : Object("player", "/images/Tank.png") {
		std::cout << "FSDGF\n";
	}
	ALLEGRO_BITMAP* image = al_load_bitmap("images/Tank.png");

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
