/*
Author: Weastmanm
*/

#ifndef INPUTEVENT_H
#define INPUTEVENT_H

#include <string>

using namespace std;

class InputEvent{
    public:
        InputEvent();
        ~InputEvent();
        string getKeyPressed();  //returns a string of the key that was pressed. Can add modifiers to it, and so on
        float getMouseX(); //returns the x position of where the mouse is. It should always be sent with an event.
        float getMouseY(); //same as getMouseX()
    private:
        float x;
        float y;
        string keypressed;
};

#endif
