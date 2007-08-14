#include "input_engine.hpp"
#include "log.hpp"
#include <OgreStringConverter.h>

#ifndef RADAKAN_WINDOWS
	#include <X11/Xlib.h>
	void checkX11Events ();
#endif

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;

//	static
const string Input_Engine ::
	get_class_name ()
{
	return "Input_Engine";
}

//	static
const bool Input_Engine ::
	absolute (false);
	
//	static
const bool Input_Engine ::
	relative (true);
	
//	static
const string Input_Engine ::
	left_mouse_button ("left");
	
//	static
const string Input_Engine ::
	middle_mouse_button ("middle");
	
//	static
const string Input_Engine ::
	right_mouse_button ("right");

Input_Engine ::
	Input_Engine (Ogre :: RenderWindow & window) :
	Object ("input engine"),
	Singleton <Input_Engine> ()
{
	Engines :: Log :: trace (me, Input_Engine :: get_class_name (), "", "~window~");

	#if OIS_VERSION_MAJOR < 1
		Engines :: Log :: error (me) << "An old version of OIS (" << OIS_VERSION_MAJOR
			<< "." << OIS_VERSION_MINOR << "." << OIS_VERSION_PATCH
			<< ") was detected. Update to OIS-1.0-RC1 or later." << endl;
		abort ();
	#endif

	unsigned long window_handle;

	window . getCustomAttribute ("WINDOW", & window_handle);

	//	The input/output system needs to know how to interact with
	//	the system window.
	OIS :: ParamList param_list;
	param_list . insert (make_pair (string ("WINDOW"), Ogre :: StringConverter :: toString (window_handle)));

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
	Engines :: Log :: trace (me, Input_Engine :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();

	input_manager -> destroyInputObject (keyboard);
	input_manager -> destroyInputObject (mouse);
	
	//	TODO solve the linking error caused by the next line.
	//	input_manager -> destroyInputSystem (input_manager);

	//	Set to NULL so we can query if they have been initialised.
	keyboard = NULL;
	mouse = NULL;
}

//	virtual
bool Input_Engine ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());
	assert (input_manager != NULL);
	assert (mouse != NULL);
	assert (keyboard != NULL);
	assert (absolute == ! relative);
	assert (left_mouse_button != middle_mouse_button);
	assert (right_mouse_button != left_mouse_button);
	assert (middle_mouse_button != right_mouse_button);

	return true;
}

//	virtual
void Input_Engine ::
	call (const Object & message)
{
	Engines :: Log :: trace (me, Input_Engine :: get_class_name (), "call", message . name);
	assert (is_initialized ());

	gui_button = message . name;

	Engines :: Log :: show ("The '" + gui_button + "' button was clicked.");
}

void Input_Engine ::
	capture ()
{
	assert (is_initialized ());

	keyboard -> capture ();

	//	'mouse -> capture ()' does nothing if the mouse didn't move this turn.
	//	So we manually set the relative key position.
	relative_mouse_position = Ogre :: Vector3 (0, 0, 0);
	mouse -> capture ();

	gui_button = "";	//	reset it, in case of a unhandeled button
	GUI_Engine :: get () . set_mouse_position (get_mouse_position ());
	if (get_mouse_button (left_mouse_button, true))
	{
		GUI_Engine :: get () . left_mouse_button_click ();
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
			Engines :: Log :: log (me) << "key '" << key << "' was reset." << endl;
		}
		return true;
	}
	return false;
}

bool Input_Engine ::
	get_gui_button (string button, bool reset)
{
	assert (is_initialized ());

	if (gui_button == button)
	{
		string to_be = "is";
		if (reset)
		{
			gui_button = "";
			to_be = "was";
		}
		Engines :: Log :: log (me) << "GUI button '" << button << "' " << to_be << " pressed." << endl;
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
		string to_be = "is";
		if (reset)
		{
			mouse_buttons [button] = false;
			to_be = "was";
		}
		Engines :: Log :: log (me) << "Mouse button '" << button << "' " << to_be << " pressed." << endl;
		return true;
	}
	return false;
}

const Ogre :: Vector3 & Input_Engine ::
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
	Engines :: Log :: log (me) << "key '" << key_string << "' is pressed." << endl;

	return true;
}

//	virtual
bool Input_Engine ::
	keyReleased (const OIS :: KeyEvent & key_event)
{
	assert (is_initialized ());

	string key_string = convert (keyboard -> getAsString (key_event . key));
	keys [key_string] = false;
	Engines :: Log :: log (me) << "key '" << key_string << "' was released." << endl;

	return true;
}

//	virtual
bool Input_Engine ::
	mouseMoved (const OIS :: MouseEvent & mouse_event)
{
	assert (is_initialized ());

	const OIS :: Axis & x = mouse_event . state . X;
	const OIS :: Axis & y = mouse_event . state . Y;
	const OIS :: Axis & z = mouse_event . state . Z;
	absolute_mouse_position = Ogre :: Vector3 (x . abs, y . abs, z . abs);
	relative_mouse_position = Ogre :: Vector3 (x . rel, y . rel, z . rel);

//	The assertions below fail if you move the mouse outside the window.
//	Engines :: Log :: log (me) << prev . first << " + " << relative_mouse_position . first << " ?= " <<  absolute_mouse_position . first << endl;
//	assert (prev . first + relative_mouse_position . first == absolute_mouse_position . first);
//	Engines :: Log :: log (me) << prev . second << " + " << relative_mouse_position . second << " ?= " <<  absolute_mouse_position . second << endl;
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
	Engines :: Log :: log (me) << "converting - in: " << key << endl;

	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		//	Convert 'NUM X' to 'X':
		if (key . find ("NUM ") != string :: npos)
		{
			assert (key . size () == 5);
			key = string (1, key . at (4));
		}
		if (key == "Esc")
		{
			key = "escape";
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

	for (unsigned int i = 0; i < key . length (); i ++)
	{
		key [i] = tolower (key [i]);
	}

	Engines :: Log :: log (me) << "converting - out: " << key << endl;
	return key;
}
