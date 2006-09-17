
#include "keyboard_event.hpp"

Keyboard_Event::Keyboard_Event () : Event("Key Pressed"),
                                    Object("Keyboard Event")
{
}

Keyboard_Event::~Keyboard_Event ()
{
}

bool Keyboard_Event::is_initialized () const
{
    return Event::is_initialized();
}

void Keyboard_Event::Set_Event(char evt)
{
    event = evt;
}
char Keyboard_Event::Get_Event(char evt)
{
    return event;
}

