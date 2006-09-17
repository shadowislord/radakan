#include "input_engine.hpp"

Input_Engine ::
	Input_Engine () :
	Engine ("Input Engine")
{
	// Define pointers as NULL so that we can be
	// sure that their status is un-initialised.

	input_manager  = NULL;

	mouse		  = NULL;
	keyboard	   = NULL;
}

Input_Engine ::
	~Input_Engine ()
{
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
	OIS::ParamList param_list;

	size_t window_handle_temp = 0;
	std::ostringstream window_handle;

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
	param_list.insert(std::make_pair(std::string("WINDOW"), window_handle.str()));
	input_manager = OIS::InputManager::createInputSystem(param_list);

	// The final parameter refers to "buffered". If this is causing us a problem,
	// enable it.
	keyboard  =  static_cast <OIS :: Keyboard *> (input_manager -> createInputObject (OIS :: OISKeyboard, false ));
	mouse = static_cast <OIS :: Mouse *> (input_manager -> createInputObject (OIS :: OISMouse, false));


		// Reference methods keyPressed, keyReleased
		keyboard -> setEventCallback (this);

		// Reference methods mouseMoved, mousePressed, mouseReleased
		mouse	-> setEventCallback (this);
}

//	virtual
bool Input_Engine::is_initialized () const
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
	return NULL;
}

//	virtual
bool Input_Engine ::
	keyPressed (const OIS :: KeyEvent & key_event)
{
	return false;
}

//	virtual
bool Input_Engine ::
	keyReleased (const OIS :: KeyEvent & key_event)
{
	return false;
}

//	virtual
bool Input_Engine ::
	mouseMoved (const OIS :: MouseEvent & mouse_event)
{
	return false;
}

//	virtual
bool Input_Engine ::
	mousePressed (const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id)
{
	return false;
}

//	virtual
bool Input_Engine ::
	mouseReleased (const OIS :: MouseEvent & mouse_event, OIS :: MouseButtonID id)
{
	return false;
}
