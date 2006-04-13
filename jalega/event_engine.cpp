#include "event_engine.hpp"

//	constructor
Event_Engine::
	Event_Engine (void_func new_quit):
	Object::
	Object ("Event Engine")
{
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

//	avoid thrashing this procedure:
	#ifdef WIN32
		Sleep (100);
	#else
		usleep (100);
	#endif

	
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
			shr->font_index = event->at (0) - '1';
			shr->post_redisplay ();
			break;
		}
		default:
		{
			break;
		}
	}

	delete event;
}
