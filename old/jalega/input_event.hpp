#ifndef INPUT_EVENT_HPP
#define INPUT_EVENT_HPP

#include "world.hpp"

using namespace std;

class Input_Event:
	public Object
{
    public:
        Input_Event (string key, float new_x, float new_y);
        Input_Event (int new_button, int new_state, float new_x, float new_y);
        ~Input_Event ();
//		string getKeyPressed ();  //returns a string of the key that was pressed. Can add modifiers to it, and so on	-	use ' * input_event ' instead
        int get_button ();
        int get_state ();
        float get_x ();
        float get_y ();

    private:
		int button;
		int state;	//	button state
        float x;
        float y;
//		string key_pressed;	-	use ' * input_event ' instead
};

#endif
