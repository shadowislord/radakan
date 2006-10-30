#include "input_engine.hpp"

using namespace std;
using namespace sl;

Input_Engine ::
	Input_Engine (Ogre :: RenderWindow * window) :
	Object ("Input Engine")
{
	assert (Object :: is_initialized ());
	
	OIS :: ParamList param_list;

	size_t window_handle_temp = 0;
	ostringstream window_handle;

	// Each operating system uses a different mechanism to
	// refer to the main rendering window.
	#ifdef SL_WIN32
		window -> getCustomAttribute ("HWND", & window_handle_temp);
	#else
		window -> getCustomAttribute ("GLXWINDOW", & window_handle_temp);
	#endif

	window_handle << window_handle_temp;

	//	The input/output system needs to know how to interact with
	//	the system window.
	param_list . insert (make_pair (string ("WINDOW"), window_handle . str ()));
	
	OIS :: InputManager :: createInputSystem (param_list);

	//	The final parameter refers to "buffered".
	//	If this is causes a problem, disable it.
	keyboard = dynamic_cast <OIS :: Keyboard *>
	(
		OIS :: InputManager :: getSingletonPtr () -> createInputObject
		(
			OIS :: OISKeyboard, true
			//	I (Tinus) set this to buffered again, as otherwise get_key (...)
			//	doesn't work.
		)
	);
	mouse = dynamic_cast <OIS :: Mouse *>
	(
		OIS :: InputManager :: getSingletonPtr () -> createInputObject
		(
			OIS :: OISMouse, true
		)
	);

	//	Reference methods keyPressed, keyReleased
	keyboard -> setEventCallback (this);

	//	Reference methods mouseMoved, mousePressed, mouseReleased
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

	//	Set to NULL so we can query if they have been initialised.
	keyboard = NULL;
	mouse = NULL;
}

//	virtual
bool Input_Engine ::
	is_initialized ()
	const
{
	if ((OIS :: InputManager :: getSingletonPtr () == NULL) || (mouse == NULL) || (keyboard == NULL))
	{
		return false;
	}
	else
	{
		return Object :: is_initialized();
	}
}

void Input_Engine ::
	capture ()
{
	assert (is_initialized ());
	
	mouse -> capture ();
	if (! mouse -> buffered ())
	{
//		handleNonBufferedMouse ();
	}

	keyboard->capture ();
	if (! keyboard -> buffered ())
	{
//		handleNonBufferedKeys ();
	}
}

//	bool Input_Engine ::
//		is_key_down (OIS :: KeyCode key_code)
//	{
//		return keyboard->isKeyDown (key_code);
//	}

bool Input_Engine ::
	get_key (string key, bool reset)
{
	assert (is_initialized ());

	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		if (key == "Escape")
		{
			key = "Esc";
		}
		if (key_win . size () == 1)
		{
			int temp = int (key . at (0));
			if ((96 < temp) && (temp < 123))
			{
				key . erase ();
				key . push_back (char (temp - 32));
			}
		}
	#endif

	if (keys [key])
	{
		debug () << "Key '" << key << "' was pressed." << endl;
		if (reset)
		{
			keys [key] = false;
		}
		return true;
	}
	return false;
}

bool Input_Engine ::
	get_mouse_buttons (int button, bool reset)
{
	assert (is_initialized ());
	
	bool result = mouse_buttons [button];
	if (reset)
	{
		mouse_buttons [button] = false;
	}
	return result;
}

int Input_Engine ::
	get_mouse_height ()
	const
{
	assert (is_initialized ());
	
	return mouse_height;
}

int Input_Engine ::
	get_mouse_width ()
	const
{
	assert (is_initialized ());
	
	return mouse_width;
}

//	virtual
bool Input_Engine ::
	keyPressed (const OIS :: KeyEvent & key_event)
{
	assert (is_initialized ());

	keys [keyboard -> getAsString (key_event . key)] = true;

	return true;
}

//	virtual
bool Input_Engine ::
	keyReleased (const OIS :: KeyEvent & key_event)
{
	assert (is_initialized ());

	keys [keyboard -> getAsString (key_event . key)] = false;

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
