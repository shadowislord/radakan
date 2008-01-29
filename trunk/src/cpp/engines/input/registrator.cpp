/*
	Engines :: Input
	{
		Registrator
		Mouse
			Data
			Reader
		Command
			Data
			Reader
	}
*/

#include "engines/input/command_data.hpp"
#include "engines/input/mouse_data.hpp"
#include "engines/input/registrator.hpp"
#include "engines/log.hpp"
#include "engines/render_engine.hpp"

#include <OgreRenderWindow.h>
#include <OgreStringConverter.h>
#include <OgreWindowEventUtilities.h>

#ifndef RADAKAN_WINDOWS
	#include <X11/Xlib.h>
	void checkX11Events ();
#endif

#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISEvents.h>

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;
using namespace Radakan :: Engines :: Input;

//	static
string Registrator ::
	get_class_name ()
{
	return "Engines :: Input :: Registrator";
}

//	static
string Registrator ::
	to_string (OIS :: MouseButtonID id)
{
	if (id == OIS :: MB_Left)
	{
		return Mouse_Data :: left_mouse_button;
	}
	else if (id == OIS :: MB_Middle)
	{
		return Mouse_Data :: middle_mouse_button;
	}
	else if (id == OIS :: MB_Right)
	{
		return Mouse_Data :: right_mouse_button;
	}
	return "other";
}

//	static
string Registrator ::
	convert (string key)
{
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

Registrator ::
	Registrator () :
	Object ("registrator", true)	//	Here 'true' means 'prevent automatic destruction'.
{
	Engines :: Log :: trace (me, Registrator :: get_class_name ());

	#if OIS_VERSION_MAJOR < 1
		Engines :: Log :: error (me) << "An old version of OIS (" << OIS_VERSION_MAJOR
			<< "." << OIS_VERSION_MINOR << "." << OIS_VERSION_PATCH
			<< ") was detected. Update to OIS-1.0-RC1 or later." << endl;
		abort ();
	#endif

	unsigned long window_handle;

	Render_Engine :: get () -> get_window ()
		-> getCustomAttribute ("WINDOW", & window_handle);

	//	The input/output system needs to know how to interact with
	//	the system window.
	OIS :: ParamList param_list;
	param_list . insert (make_pair (string ("WINDOW"), Ogre :: StringConverter :: toString (window_handle)));
	#ifdef RADAKAN_WINDOWS
		//	param_list . insert
		//		(make_pair (string ("w32_keyboard"), string ("DISCL_NONEXCLUSIVE")));
		//	param_list . insert
		//		(make_pair (string ("w32_mouse"), string ("DISCL_NONEXCLUSIVE")));
	#else
		param_list . insert (make_pair (string ("x11_keyboard_grab"), string ("false")));
		param_list . insert (make_pair (string ("XAutoRepeatOn"), string ("true")));
		param_list . insert (make_pair (string ("x11_mouse_grab"), string ("false")));
	#endif

    input_manager = boost :: shared_ptr <OIS :: InputManager>
    (
    	OIS :: InputManager :: createInputSystem (param_list),
    	OIS :: InputManager :: destroyInputSystem
    );

	//	The final parameter refers to "buffered".
	keyboard . reset
	(
		boost :: polymorphic_downcast <OIS :: Keyboard *>
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
		boost :: polymorphic_downcast <OIS :: Mouse *>
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
	mouse -> getMouseState () . width
		= Render_Engine :: get () -> get_window () -> getWidth ();
	mouse -> getMouseState () . height
		= Render_Engine :: get () -> get_window () -> getHeight ();

	assert (is_initialized ());
}

Registrator ::
	~Registrator ()
{
	Engines :: Log :: trace (me, Registrator :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();

	input_manager -> destroyInputObject (keyboard . get ());
	input_manager -> destroyInputObject (mouse . get ());

	//	TODO solve the linking error caused by the next line.
	//	input_manager -> destroyInputSystem (input_manager);

	keyboard . reset ();
	mouse . reset ();
}

//	virtual
bool Registrator ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());
	assert (input_manager);
	assert (mouse);
	assert (keyboard);

	return true;
}

//	virtual
void Registrator ::
	call (Reference <Messages :: Button_Event> message)
{
	Engines :: Log :: trace (me, Registrator :: get_class_name (), "call", message . get_name ());
	assert (is_initialized ());
	assert (message . points_to_object ());

	Engines :: Input :: Command_Data :: get () -> clicked_button
		= message . get_name (true);
}

//	virtual
void Registrator ::
	call (Reference <Messages :: List_Event> message)
{
	Engines :: Log :: trace (me, Registrator :: get_class_name (), "call", message . get_name ());
	assert (is_initialized ());
	assert (message . points_to_object ());

	Engines :: Input :: Command_Data :: get () -> selected_communication_option
		= message . get_name (true);
}

void Registrator ::
	capture ()
{
	assert (is_initialized ());

	keyboard -> capture ();

	//	'mouse -> capture ()' does nothing if the mouse didn't move this turn.
	//	So we manually set the relative key position.
	Engines :: Input :: Mouse_Data :: get () -> relative_mouse_position
		= Ogre :: Vector3 (0, 0, 0);
	mouse -> capture ();

	if (Render_Engine :: get () -> get_window () -> isClosed ())
	{
		Engines :: Input :: Command_Data :: get () -> clicked_button = "quit";
	}

	Ogre :: WindowEventUtilities :: messagePump ();
}

//	virtual
bool Registrator ::
	keyPressed (const OIS :: KeyEvent & key_event)
{
	assert (is_initialized ());

	string key_string = convert (keyboard -> getAsString (key_event . key));
	Engines :: Input :: Command_Data :: get () -> pressed_keys . insert (key_string);
	Engines :: Log :: log (me) << "key '" << key_string << "' is pressed." << endl;

	return true;
}

//	virtual
bool Registrator ::
	keyReleased (const OIS :: KeyEvent & key_event)
{
	assert (is_initialized ());

	string key_string = convert (keyboard -> getAsString (key_event . key));
	Engines :: Input :: Command_Data :: get () -> pressed_keys . erase (key_string);
	Engines :: Log :: log (me) << "key '" << key_string << "' was released." << endl;

	return true;
}

//	virtual
bool Registrator ::
	mouseMoved (const OIS :: MouseEvent & mouse_event)
{
	assert (is_initialized ());

	const OIS :: Axis & x = mouse_event . state . X;
	const OIS :: Axis & y = mouse_event . state . Y;
	const OIS :: Axis & z = mouse_event . state . Z;
	Engines :: Input :: Mouse_Data :: get () -> absolute_mouse_position
		= Ogre :: Vector3 (x . abs, y . abs, z . abs);
	Engines :: Input :: Mouse_Data :: get () -> relative_mouse_position
		= Ogre :: Vector3 (x . rel, y . rel, z . rel);

	return true;
}

//	virtual
bool Registrator ::
	mousePressed (const OIS :: MouseEvent &, OIS :: MouseButtonID id)
{
	assert (is_initialized ());

	Engines :: Input :: Mouse_Data :: get () -> pressed_mouse_buttons
		. insert (to_string (id));

	return true;
}

//	virtual
bool Registrator ::
	mouseReleased (const OIS :: MouseEvent &, OIS :: MouseButtonID id)
{
	assert (is_initialized ());

	Engines :: Input :: Mouse_Data :: get () -> pressed_mouse_buttons
		. erase (to_string (id));

	return true;
}
