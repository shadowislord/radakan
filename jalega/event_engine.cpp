#include "event_engine.hpp"

//	constructor
Event_Engine::
	Event_Engine (void_func new_quit, void_func new_refresh):
	Object::
	Object ("Event Engine")
{
	refresh = new_refresh;
	quit = new_quit;
}

//	destructor
	Event_Engine::
	 ~Event_Engine ()
{

}

void Event_Engine::
	process (Input_Event * event)
{
	assert (event != NULL);

// avoid thrashing this procedure
	usleep (100);
	
	switch (event->at (0))
	{
		case 27:	// Esc - Quit
		{
			delete event;
			quit ();
		}
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		{
			font_index = event->at (0) - '1';
			refresh ();
			break;
		}
		default:
		{
			break;
		}
	}

	delete event;
}
