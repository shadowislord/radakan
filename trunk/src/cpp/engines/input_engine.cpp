#include "base.hpp"

#include <OgreRenderWindow.h>
#include <OgreStringConverter.h>

#ifndef RADAKAN_WINDOWS
	#include <X11/Xlib.h>
	void checkX11Events ();
#endif

#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISEvents.h>

#include "engines/gui_engine.hpp"
#include "engines/input_engine.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"
#include "messages/conversation_message.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;

//	static
string Input_Engine ::
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
	Input_Engine (boost :: shared_ptr <Ogre :: RenderWindow> window) :
	Object ("input engine", true)	//	Here 'true' means 'prevent automatic destruction'.
{
	Engines :: Log :: trace (me, Input_Engine :: get_class_name (), "", "~window~");

	#if OIS_VERSION_MAJOR < 1
		Engines :: Log :: error (me) << "An old version of OIS (" << OIS_VERSION_MAJOR
			<< "." << OIS_VERSION_MINOR << "." << OIS_VERSION_PATCH
			<< ") was detected. Update to OIS-1.0-RC1 or later." << endl;
		abort ();
	#endif

	unsigned long window_handle;

	window -> getCustomAttribute ("WINDOW", & window_handle);

	//	The input/output system needs to know how to interact with
	//	the system window.
	OIS :: ParamList param_list;
	param_list . insert (make_pair (string ("WINDOW"), Ogre :: StringConverter :: toString (window_handle)));
	#ifdef RADAKAN_WINDOWS
		param_list . insert (make_pair (string ("w32_keyboard"), string("DISCL_NONEXCLUSIVE")));
		param_list . insert (make_pair (string ("w32_mouse"), string ("DISCL_NONEXCLUSIVE")));
	#else
		param_list . insert (make_pair (string ("x11_keyboard_grab"), string("false")));
		param_list . insert (make_pair (string ("XAutoRepeatOn"), string("true")));
		param_list . insert (make_pair (string ("x11_mouse_grab"), string("false")));
	#endif

    input_manager = boost :: shared_ptr <OIS :: InputManager> (OIS :: InputManager :: createInputSystem (param_list), OIS :: InputManager :: destroyInputSystem);

	//	The final parameter refers to "buffered".
	keyboard . reset
	(
		dynamic_cast <OIS :: Keyboard *>
		(
			input_manager -> createInputObject
			(
				OIS :: OISKeyboard, true
				//	I (Tinus) set this to buffered again, as otherwise get_key (...)
				//	doesn't work.
			)
		)
	);
	mouse . reset
	(
		dynamic_cast <OIS :: Mouse *>
		(
			input_manager -> createInputObject
			(
				OIS :: OISMouse, true
			)
		)
	);

	//	Reference methods keyPressed, keyReleased
	keyboard -> setEventCallback (this);

	//	Reference methods mouseMoved, mousePressed, mouseReleased
	mouse -> setEventCallback (this);
	mouse -> getMouseState () . width = window -> getWidth ();
	mouse -> getMouseState () . height = window -> getHeight ();

	assert (is_initialized ());
}

Input_Engine ::
	~Input_Engine ()
{
	Engines :: Log :: trace (me, Input_Engine :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();

	input_manager -> destroyInputObject (keyboard . get ());
	input_manager -> destroyInputObject (mouse . get ());

	//	TODO solve the linking error caused by the next line.
	//	input_manager -> destroyInputSystem (input_manager);

	//	Set to NULL so we can query if they have been initialised.
	keyboard . reset ();
	mouse . reset ();
}

//	virtual
bool Input_Engine ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());
	assert (input_manager);
	assert (mouse);
	assert (keyboard);

	assert (absolute == ! relative);
	assert (left_mouse_button != middle_mouse_button);
	assert (right_mouse_button != left_mouse_button);
	assert (middle_mouse_button != right_mouse_button);

	return true;
}

//	virtual
void Input_Engine ::
	call (const Reference <Messages :: Message <Items :: Characters :: Character> > & message)
{
	Engines :: Log :: trace (me, Input_Engine :: get_class_name (), "call", message . get_name ());
	assert (is_initialized ());
	assert (message . points_to_object ());

	conversation_option = message;
}

//	virtual
void Input_Engine ::
	call (const Reference <Object> & message)
{
	Engines :: Log :: trace (me, Input_Engine :: get_class_name (), "call", message . get_name ());
	assert (is_initialized ());
	assert (message . points_to_object ());

	gui_command = message . get_name (true);
	//	assert (is_lower_case (gui_command));	//	Doesn't work.
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

	GUI_Engine :: get () -> set_mouse_position (get_mouse_position ());
	if (is_mouse_button_pressed (left_mouse_button, true))
	{
		GUI_Engine :: get () -> left_mouse_button_click ();
	}
}

const Reference <Messages :: Message <Items :: Characters :: Character> > Input_Engine ::
	get_conversation_option ()
{
	assert (is_initialized ());

	if (conversation_option . points_to_object ())
	{
		const Reference <Messages :: Message <Items :: Characters :: Character> > result = conversation_option;

		conversation_option . reset_pointee ();

		Engines :: Log :: show ("The " + result . get_name (true) + " option was clicked.");

		return result;
	}
	else
	{
		return conversation_option;	//	refers to 'NULL'
	}
}

bool Input_Engine ::
	is_mouse_button_pressed (string button, bool reset)
{
	assert (is_initialized ());

	if (0 < pressed_mouse_buttons . count (button))
	{
		string to_be = "is";
		if (reset)
		{
			pressed_mouse_buttons . erase (button);
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

bool Input_Engine ::
	has_command (string command_name, bool reset)
{
	Engines :: Log :: trace (me, Input_Engine :: get_class_name (), "has_command", command_name);
	assert (is_initialized ());

	if (gui_command == command_name)
	{
		if (reset)
		{
			gui_command = "";
		}
	}
	else
	{
		//	Is there a key shortcut for the command?
		map <string, string> :: iterator key_binding
			= Settings :: get () -> key_bindings . find (command_name);
		if (key_binding == Settings :: get () -> key_bindings . end ())
		{
			return false;
		}
		else
		{
			//	Is that key shortcut pressed?
			string & key = key_binding -> second;
			if (0 == pressed_keys . count (key))
			{
				return false;
			}
			
			if (reset)
			{
				pressed_keys . erase (key);
			}
		}
	}
	
	return true;
}

//	virtual
bool Input_Engine ::
	keyPressed (const OIS :: KeyEvent & key_event)
{
	assert (is_initialized ());

	string key_string = convert (keyboard -> getAsString (key_event . key));
	pressed_keys . insert (key_string);
	Engines :: Log :: log (me) << "key '" << key_string << "' is pressed." << endl;

	return true;
}

//	virtual
bool Input_Engine ::
	keyReleased (const OIS :: KeyEvent & key_event)
{
	assert (is_initialized ());

	string key_string = convert (keyboard -> getAsString (key_event . key));
	pressed_keys . erase (key_string);
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

	return true;
}

//	virtual
bool Input_Engine ::
	mousePressed (const OIS :: MouseEvent &, OIS :: MouseButtonID id)
{
	assert (is_initialized ());

	pressed_mouse_buttons . insert (to_string (id));

	return true;
}

//	virtual
bool Input_Engine ::
	mouseReleased (const OIS :: MouseEvent &, OIS :: MouseButtonID id)
{
	assert (is_initialized ());

	pressed_mouse_buttons . erase (to_string (id));

	return true;
}

bool Input_Engine ::
	is_mouse_button (string button)
{
	return (button == left_mouse_button)
		|| (button == middle_mouse_button)
		|| (button == right_mouse_button);
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

	return key;
}
