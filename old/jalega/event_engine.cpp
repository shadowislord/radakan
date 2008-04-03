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
	assert (event->is_initialized ());

//	changing the coordinates from left-top based to normal center based:
	float x = event->get_x () - shr->get_width () / 2;
	float y = shr->get_height () / 2 - event->get_y () - 1;

	debug () << "process " << * event << ": " << event->get_button () <<
		 " " << event->get_state () << " " << x << " " << y << endl;

//	avoid thrashing this procedure:
	#ifdef WIN32
		Sleep (100);
	#else
		usleep (100);
	#endif

	if (* event == "mouse")
	{
		if ((event->get_button () == GLUT_LEFT_BUTTON) &&
			(event->get_state () == GLUT_UP))
		{
/*			Obstacle * house = new Obstacle ("house", D3 (127, 77, 0), D3 (25, 25, 25));
			if (tsl->add_obstacle (house))
			{
				delete house;
			}
			else
			{
				shr->post_redisplay ();
			}*/
		}
		delete event;
		return;
	}

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
