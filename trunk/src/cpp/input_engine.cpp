#include "input_engine.hpp"

using namespace std;
using namespace sl;

template <> Input_Engine * Ogre :: Singleton <Input_Engine> :: ms_Singleton = NULL;

Input_Engine ::
	Input_Engine (Ogre :: RenderWindow & window) :
	Object ("Input Engine")
{
	assert (Object :: is_initialized ());
	
	OIS :: ParamList param_list;

	size_t window_handle_temp = 0;
	ostringstream window_handle;

	// Each operating system uses a different mechanism to
	// refer to the main rendering window.
	#ifdef SL_WIN32
		window . getCustomAttribute ("HWND", & window_handle_temp);
	#else
		window . getCustomAttribute ("GLXWINDOW", & window_handle_temp);
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
		return warn <Input_Engine> (false);
	}
	else
	{
		return warn <Input_Engine> (Object :: is_initialized ());
	}
}

//	static
string Input_Engine ::
	get_type_name ()
{
	return "input engine";
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

	keyboard -> capture ();
	if (! keyboard -> buffered ())
	{
//		handleNonBufferedKeys ();
	}
}

//	bool Input_Engine ::
//		is_key_down (OIS :: KeyCode key_code)
//	{
//		return keyboard -> isKeyDown (key_code);
//	}

bool Input_Engine ::
	get_key (string key, bool reset)
{
	assert (is_initialized ());

	if (keys [key])
	{
		if (reset)
		{
			keys [key] = false;
			trace () << "key '" << key << "' was reset." << endl;
		}
		return true;
	}
	return false;
}

bool Input_Engine ::
	get_mouse_button (string button, bool reset)
{
	assert (is_initialized ());
	
	if (mouse_buttons [button])
	{
		trace () << "Mouse button '" << button << "' was pressed." << endl;
		if (reset)
		{
			mouse_buttons [button] = false;
		}
		return true;
	}
	return false;
}

float Input_Engine ::
	get_mouse_height (bool relative)
	const
{
	assert (is_initialized ());

	if (relative)
	{
		return relative_mouse_height;
	}
	
	return absolute_mouse_height;
}

float Input_Engine ::
	get_mouse_width (bool relative)
	const
{
	assert (is_initialized ());
	
	if (relative)
	{
		return relative_mouse_width;
	}

	return absolute_mouse_width;
}

//	virtual
bool Input_Engine ::
	keyPressed (const OIS :: KeyEvent & key_event)
{
	assert (is_initialized ());

	string key_string = convert (keyboard -> getAsString (key_event . key));
	keys [key_string] = true;
	trace () << "key '" << key_string << "' was pressed." << endl;

	return true;
}

//	virtual
bool Input_Engine ::
	keyReleased (const OIS :: KeyEvent & key_event)
{
	assert (is_initialized ());

	string key_string = convert (keyboard -> getAsString (key_event . key));
	keys [key_string] = false;
	trace () << "key '" << key_string << "' was released." << endl;

	return true;
}

//	virtual
bool Input_Engine ::
	mouseMoved (const OIS :: MouseEvent & mouse_event)
{
	assert (is_initialized ());

	relative_mouse_width = mouse_event . state . relX;
	relative_mouse_height = mouse_event . state . relY;

	absolute_mouse_width = mouse_event . state . abX;
	absolute_mouse_height = mouse_event . state . abY;

	return true;
}

//	virtual
bool Input_Engine ::
	mousePressed (const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id)
{
	assert (is_initialized ());

	mouse_buttons [to_string (id)] = true;
	
	return true;
}

//	virtual
bool Input_Engine ::
	mouseReleased (const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id)
{
	assert (is_initialized ());
	
	mouse_buttons [to_string (id)] = false;
	
	return true;
}

bool Input_Engine ::
	is_mouse_button (string button) const
{
	return (button == left_mouse_button) || (button == middle_mouse_button) || (button == right_mouse_button);
}

string Input_Engine ::
	to_string (OIS :: MouseButtonID id)
{
	assert (is_initialized ());
	
	if (id == OIS :: MB_Left)
	{
		return left_mouse_button;
	}
	else if (id == OIS :: MB_Middle)
	{
		return middle_mouse_button;
	}
	else if (id == OIS :: MB_Right)
	{
		return right_mouse_button;
	}
	return "other";
}

string Input_Engine ::
	convert (string key)
{
	assert (is_initialized ());
	trace () << "converting - in: " << key << endl;
	
	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		//	Converting capitals to lower-case:
		if (key . size () == 1)
		{
			int temp = int (key . at (0));
			if ((96 < temp) && (temp < 123))
			{
				key . erase ();
				key . push_back (char (temp - 32));
			}
		}
		//	Converting 'NUM X' to 'X':
		//if (key . find ("NUM ") != key . end ())
		//solving vs 2005 error C2678: binary '!=' : no operator found which takes a left-hand operand of type '__w64 unsigned int' (or there is no acceptable conversion)
		if (key . find ("NUM ") != string::npos)
		{
			assert (key . size () == 5);
			key = string (1, key . at (4));
		}
		if (key == "ESC")
		{
			key = "Escape";
		}
	#else	//	assuming linux
		//	Converting keypad codes to numbers:
		if (key == "KP_Insert")
		{
			key = "0";
		}
		if (key == "KP_End")
		{
			key = "1";
		}
		if (key == "KP_Down")
		{
			key = "2";
		}
		if (key == "KP_Next")
		{
			key = "3";
		}
		if (key == "KP_Left")
		{
			key = "4";
		}
		if (key == "KP_Begin")
		{
			key = "5";
		}
		if (key == "KP_Right")
		{
			key = "6";
		}
		if (key == "KP_Home")
		{
			key = "7";
		}
		if (key == "KP_Up")
		{
			key = "8";
		}
		if (key == "KP_Prior")
		{
			key = "9";
		}
	
	#endif
	
	trace () << "converting - out: " << key << endl;
	return key;
}
