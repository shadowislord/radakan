//
//	Author: Weastmanm
//

#include "input_event.hpp"

Input_Event::
	Input_Event (string key, float new_x, float new_y):
	Object::
	Object (key)

{
	x = new_x;
	y = new_y;
}

//	virtual
Input_Event::
	~Input_Event
	()
{

}

//	string
//		Input_Event::
//		 getKeyPressed ()
//	{
//		return "";
//	}

float Input_Event::
	get_x()
{
	return x;
}

float Input_Event::
	get_y()
{
	return y;
}
