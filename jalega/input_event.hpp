//
//	Author: Weastmanm
//

#ifndef INPUT_EVENT_HPP
#define INPUT_EVENT_HPP

#include "object.hpp"

using namespace std;

class Input_Event:
	public Object
{
    public:
        Input_Event (string key, float new_x, float new_y);
        ~Input_Event ();
//		string getKeyPressed ();  //returns a string of the key that was pressed. Can add modifiers to it, and so on	-	use ' * input_event ' instead
        float get_x ();	//	returns the x position of where the mouse is.
        float get_y ();	//	returns the y position of where the mouse is.
    private:
        float x;
        float y;
//		string key_pressed;	-	use ' * input_event ' instead
};

#endif
