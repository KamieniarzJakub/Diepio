#include "Mouse.h"
Mouse::Mouse()
{
	Mouse::set_position(0, 0);
}

float Mouse::get_pos_x()
{
	return position_x;
}
float Mouse::get_pos_y()
{
	return position_y;
}
void Mouse::set_position(float x_value, float y_value)
{
	position_x = x_value;
	position_y = y_value;
}
