#include "input_engine.hpp"

using namespace std;
using namespace tsl;

template <> Input_Engine * Ogre :: Singleton <Input_Engine> :: ms_Singleton = NULL;
const string Input_Engine :: left_mouse_button = "left";
const string Input_Engine :: middle_mouse_button = "middle";
const string Input_Engine :: right_mouse_button = "right";

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
	#ifdef TSL_WIN32
		window . getCustomAttribute ("HWND", & window_handle_temp);
	#else
		window . getCustomAttribute ("GLXWINDOW", & window_handle_temp);
	#endif

	window_handle << window_handle_temp;

	//	The input/output system needs to know how to interact with
	//	the system window.
	param_list . insert (make_pair (string ("WINDOW"), window_handle . str ()));

	input_manager = OIS :: InputManager :: createInputSystem (param_list);

	//	The final parameter refers to "buffered".
	keyboard = dynamic_cast <OIS :: Keyboard *>
	(
		input_manager -> createInputObject
		(
			OIS :: OISKeyboard, true
			//	I (Tinus) set this to buffered again, as otherwise get_key (...)
			//	doesn't work.
		)
	);
	mouse = dynamic_cast <OIS :: Mouse *>
	(
		input_manager -> createInputObject
		(
			OIS :: OISMouse, true
		)
	);

	//	Reference methods keyPressed, keyReleased
	keyboard -> setEventCallback (this);

	//	Reference methods mouseMoved, mousePressed, mouseReleased
	mouse -> setEventCallback (this);
	mouse -> getMouseState () . width = window . getWidth ();
	mouse -> getMouseState () . height = window . getHeight ();

	assert (is_initialized ());
}

Input_Engine ::
	~Input_Engine ()
{
	assert (is_initialized ());

	input_manager -> destroyInputObject (keyboard);
	input_manager -> destroyInputObject (mouse);
	#if OIS_VERSION_MAJOR < 1
		//	assuming ois-0.7.2
		input_manager -> destroyInputSystem ();
	#else
		//	!!! this gives a linking error
		//	input_manager -> destroyInputSystem (input_manager);
	#endif

	//	Set to NULL so we can query if they have been initialised.
	keyboard = NULL;
	mouse = NULL;
}

//	virtual
bool Input_Engine ::
	is_initialized ()
	const
{
	return warn <Input_Engine> (Object :: is_initialized () && (input_manager != NULL) && (mouse != NULL) && (keyboard != NULL));
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

	keyboard -> capture ();
	if (! keyboard -> buffered ())
	{
//		handleNonBufferedKeys ();
	}
	mouse -> capture ();
	if (! mouse -> buffered ())
	{
//		handleNonBufferedMouse ();
	}
}

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

void Input_Engine ::
	end_of_turn ()
{
	assert (is_initialized ());

	relative_mouse_position = pair <float, float> (0, 0);
}

pair <float, float> Input_Engine ::
	get_mouse_position (bool relative)
	const
{
	assert (is_initialized ());

	if (relative)
	{
		return relative_mouse_position;
	}

	return absolute_mouse_position;
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

	#ifdef TSL_DEBUG
		pair <float, float> prev = absolute_mouse_position;
	#endif
	
	#if OIS_VERSION_MAJOR < 1
		//	assuming ois-0.7.2
		relative_mouse_position = pair <float, float>
				(mouse_event . state . relX, mouse_event . state . relY);

		absolute_mouse_position = pair <float, float>
				(mouse_event . state . abX, mouse_event . state . abY);
	#else
		error () << "OIS 1" << endl;

		error () << "OIS 1.8" << endl;
		int a = mouse_event . state . X . rel;	//	!!!	This line makes TSL crash...
		error () << "OIS: " << a << endl;
		error () << "OIS 1.9" << endl;
		relative_mouse_position = pair <float, float>
				(mouse_event . state . X . rel, mouse_event . state . Y . rel);
		error () << "OIS 2" << endl;

		absolute_mouse_position = pair <float, float>
				(mouse_event . state . X . abs, mouse_event . state . Y . abs);
		error () << "OIS 3" << endl;
	#endif

//	The assertions below fail if you move the mouse outside the window.
//	debug () << prev . first << " + " << relative_mouse_position . first << " ?= " <<  absolute_mouse_position . first << endl;
//	assert (prev . first + relative_mouse_position . first == absolute_mouse_position . first);
//	debug () << prev . second << " + " << relative_mouse_position . second << " ?= " <<  absolute_mouse_position . second << endl;
//	assert (prev . second + relative_mouse_position . second == absolute_mouse_position . second);

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
	is_mouse_button (string button)
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
		//	Convert capitals to lower-case:
		if (key . size () == 1)
		{
			int temp = int (key . at (0));
			if ((64 < temp) && (temp < 90))
			{
				key . erase ();
				key . push_back (char (temp + 32));
			}
		}
		//	Convert 'NUM X' to 'X':
		if (key . find ("NUM ") != string :: npos)
		{
			assert (key . size () == 5);
			key = string (1, key . at (4));
		}
		if (key == "Esc")
		{
			key = "Escape";
		}
	#else	//	assuming linux
		//	Convert the keypad codes to numbers:
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
