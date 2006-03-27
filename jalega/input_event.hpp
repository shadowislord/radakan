/*
Author: Weastmanm
*/

#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#include <string>

using namespace std;

class input_event{
    public:
        input_event();
        ~input_event();
        string getKeyPressed();  //returns a string of the key that was pressed. Can add modifiers to it, and so on
        float getMouseX(); //returns the x position of where the mouse is. It should always be sent with an event.
        float getMouseY(); //same as getMouseX()
    private:
        float x;
        float y;
        string keypressed;
};

#endif
