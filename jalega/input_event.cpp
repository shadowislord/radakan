#include "input_event.hpp"

Input_Event::
	Input_Event (string key, float new_x, float new_y):
	Object::
	Object (key)

{
	button = - 1;
	state = - 1;
	x = new_x;
	y = new_y;
}

Input_Event::
	Input_Event (int new_button, int new_state, float new_x, float new_y):
	Object::
	Object ("mouse")

{
	button = new_button;
	state = new_state;
	x = new_x;
	y = new_y;
}

//	virtual
Input_Event::
	~Input_Event
	()
{

}

int Input_Event::
	get_button
	()
{
	return button;
}

int Input_Event::
	get_state
	()
{
	return state;
}

float Input_Event::
	get_x
	()
{
	return x;
}

float Input_Event::
	get_y
	()
{
	return y;
}
