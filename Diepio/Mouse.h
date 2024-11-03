#pragma once
class Mouse {
private:
	float position_x;
	float position_y;

public:
	Mouse();
	//getter of position_x
	float get_pos_x();
	//getter of position_y
	float get_pos_y();

	//setter of mouse position
	void set_position(float x_value, float y_value);
};