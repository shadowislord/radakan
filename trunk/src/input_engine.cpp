#include "input_engine.hpp"

using namespace std;

Input_Engine ::
	Input_Engine () :
	Engine ("Input Engine")
{
	assert (Engine :: is_initialized ());
	
	//	Define pointers as NULL so that we can be
	//	sure that their status is un-initialised.

	input_manager = NULL;
	mouse = NULL;
	keyboard = NULL;

	going_forward = false;
	going_backward = false;
	going_left = false;
	going_right = false;

	assert (is_initialized ());
}

Input_Engine ::
	~Input_Engine ()
{
	assert (is_initialized ());
	
	input_manager -> destroyInputObject (keyboard);
	input_manager -> destroyInputObject (mouse);
	input_manager -> destroyInputSystem ();

	// Set to NULL so we can query if they have been
	// initialised.
	input_manager = NULL;
	keyboard = NULL;
	mouse = NULL;
}

void Input_Engine ::
	start_listening (Ogre :: RenderWindow * window)
{
	assert (is_initialized ());
	
	OIS::ParamList param_list;

	size_t window_handle_temp = 0;
	ostringstream window_handle;

	// Each operating system uses a different mechanism to
	// refer to the main rendering window.
	#if	 defined OIS_WIN32_PLATFORM
		window->getCustomAttribute("HWND", &window_handle_temp);
	#elif defined OIS_LINUX_PLATFORM
		window->getCustomAttribute("GLXWINDOW", &window_handle_temp);
	#endif

	window_handle << window_handle_temp;

	// The input/output system needs to know how to interact with
	// the system window.
	param_list.insert (make_pair (string("WINDOW"), window_handle.str()));
	input_manager = OIS :: InputManager :: createInputSystem (param_list);

	// The final parameter refers to "buffered". If this is causing us a problem,
	// enable it.
	keyboard = static_cast <OIS :: Keyboard *> (input_manager -> createInputObject (OIS :: OISKeyboard, false));
	mouse = static_cast <OIS :: Mouse *> (input_manager -> createInputObject (OIS :: OISMouse, false));

	// Reference methods keyPressed, keyReleased
	keyboard -> setEventCallback (this);

	// Reference methods mouseMoved, mousePressed, mouseReleased
	mouse -> setEventCallback (this);
}

//	virtual
bool Input_Engine ::
	is_initialized () const
{
	if ((input_manager == NULL) || (mouse == NULL) || (keyboard == NULL))
	{
		return false;
	}
	else
	{
		return Engine :: is_initialized();
	}
}

//	virtual
Event * Input_Engine ::
	process (Event * event)
{
	assert (is_initialized ());

	if (going_forward)
	{
		event_queue.push (new Move_Event (Move_Event :: forward));
	}
	if (going_backward)
	{
		event_queue.push (new Move_Event (Move_Event :: backward));
	}
	if (going_left)
	{
		event_queue.push (new Move_Event (Move_Event :: left));
	}
	if (going_right)
	{
		event_queue.push (new Move_Event (Move_Event :: right));
	}

	if (event_queue.empty ())
	{
		return NULL;
	}
	else
	{
		Event * event = event_queue.front();
		event_queue.pop();
		return event;
	}
}

//	virtual
bool Input_Engine ::
	keyPressed (const OIS :: KeyEvent & key_event)
{
	assert (is_initialized ());

//	'Key -> action' mapping will happen here.

	switch (key_event.key)
	{
		case OIS :: KC_W:
		{
			going_forward = true;
			break;
		}

		case OIS :: KC_S:
		{
			going_backward = true;
			break;
		}

		case OIS :: KC_A:
		{
			going_left = true;
			break;
		}

		case OIS :: KC_D:
		{
			going_right = true;
			break;
		}

		case OIS :: KC_ESCAPE:
		{
			event_queue.push (new Exit_Event ());
			break;
		}

		default:
		{
			break;
		}
	}

	return true;
}

//	virtual
bool Input_Engine ::
	keyReleased (const OIS :: KeyEvent & key_event)
{
	assert (is_initialized ());

//	'Key -> action' mapping will happen here.

	switch (key_event.key)
	{
		case OIS :: KC_W:
		{
			going_forward = false;
			break;
		}

		case OIS :: KC_S:
		{
			going_backward = false;
			break;
		}

		case OIS :: KC_A:
		{
			going_left = false;
			break;
		}

		case OIS :: KC_D:
		{
			going_right = false;
			break;
		}

		default:
		{
			break;
		}
	}

	return true;
}

//	virtual
bool Input_Engine ::
	mouseMoved (const OIS :: MouseEvent & mouse_event)
{
	assert (is_initialized ());
	
	return false;
}

//	virtual
bool Input_Engine ::
	mousePressed (const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id)
{
	assert (is_initialized ());
	
	return false;
}

//	virtual
bool Input_Engine ::
	mouseReleased (const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id)
{
	assert (is_initialized ());
	
	return false;
}
