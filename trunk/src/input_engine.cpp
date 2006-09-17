#include "input_engine.hpp"

using namespace std;

Input_Engine ::
	Input_Engine (Ogre :: RenderWindow * window) :
	Engine ("Input Engine")
{
	assert (Engine :: is_initialized ());
	
	going_forward = false;
	going_backward = false;
	going_left = false;
	going_right = false;

	OIS :: ParamList param_list;

	size_t window_handle_temp = 0;
	ostringstream window_handle;

	// Each operating system uses a different mechanism to
	// refer to the main rendering window.
	#ifdef SL_WIN32
		window->getCustomAttribute("HWND", &window_handle_temp);
	#else
		window->getCustomAttribute("GLXWINDOW", &window_handle_temp);
	#endif

	window_handle << window_handle_temp;

	//	The input/output system needs to know how to interact with
	//	the system window.
	param_list.insert (make_pair (string("WINDOW"), window_handle.str()));
	
	OIS :: InputManager :: createInputSystem (param_list);

	// The final parameter refers to "buffered". If this is causing us a problem,
	// enable it.
	keyboard = dynamic_cast <OIS :: Keyboard *>
	(
		OIS :: InputManager :: getSingletonPtr () -> createInputObject
		(
			OIS :: OISKeyboard, false
		)
	);
	mouse = dynamic_cast <OIS :: Mouse *>
	(
		OIS :: InputManager :: getSingletonPtr () -> createInputObject
		(
			OIS :: OISMouse, false
		)
	);

	// Reference methods keyPressed, keyReleased
	keyboard -> setEventCallback (this);

	// Reference methods mouseMoved, mousePressed, mouseReleased
	mouse -> setEventCallback (this);

	assert (is_initialized ());
}

Input_Engine ::
	~Input_Engine ()
{
	assert (is_initialized ());
	
	OIS :: InputManager :: getSingletonPtr () -> destroyInputObject (keyboard);
	OIS :: InputManager :: getSingletonPtr () -> destroyInputObject (mouse);
	OIS :: InputManager :: destroyInputSystem ();

	// Set to NULL so we can query if they have been
	// initialised.
	keyboard = NULL;
	mouse = NULL;
}

//	virtual
bool Input_Engine ::
	is_initialized () const
{
	if ((OIS :: InputManager :: getSingletonPtr () == NULL) || (mouse == NULL) || (keyboard == NULL))
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
