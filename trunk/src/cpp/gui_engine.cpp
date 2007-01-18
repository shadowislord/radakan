#include "gui_engine.hpp"

using namespace std;
using namespace tsl;

GUI_Engine ::
	GUI_Engine (Ogre :: RenderWindow & window, string log_file_name) :
	Object ("Gui engine"),
	Singleton <GUI_Engine> ("Gui engine")
{
	assert (Object :: is_initialized ());

	renderer = new CEGUI :: OgreCEGUIRenderer (& window);

	#if CEGUI_VERSION_MINOR > 4
		//	assuming cegui-0.5
		system = new CEGUI :: System (renderer, NULL, NULL, NULL, "", log_file_name);
	#else
		system = new CEGUI :: System (renderer, CEGUI :: String (log_file_name) . data ());
	#endif
	
	window_manager = & CEGUI :: WindowManager :: getSingleton ();

	CEGUI :: Logger :: getSingleton () . setLoggingLevel (CEGUI :: Informative);

	CEGUI :: SchemeManager :: getSingleton () . loadScheme ("TaharezLookSkin.scheme");
	system -> setDefaultMouseCursor ("TaharezLook", "MouseArrow");

	system -> setDefaultFont ("BlueHighway-12");

	root_window = window_manager -> loadWindowLayout ("tsl.config");
	system -> setGUISheet (root_window);

	CEGUI :: Event :: Subscriber event_suscriber (& GUI_Engine :: handle_button, this);

//	!!!	Somehow doesn't work...
//	CEGUI :: GlobalEventSet :: getSingleton () . subscribeEvent
//						(CEGUI :: PushButton :: EventClicked, event_suscriber);
	
	root_window -> getChild ("welcome-button") -> subscribeEvent
						(CEGUI :: PushButton :: EventClicked, event_suscriber);
	root_window -> getChild ("howdy-button2") -> subscribeEvent
						(CEGUI :: PushButton :: EventClicked, event_suscriber);
	root_window -> getChild ("quit-button") -> subscribeEvent
						(CEGUI :: PushButton :: EventClicked, event_suscriber);

	assert (is_initialized ());
}

GUI_Engine ::
	~GUI_Engine ()
{
	assert (is_initialized ());
}

//	virtual
bool GUI_Engine ::
	is_initialized ()
	const
{
	return warn <GUI_Engine> (Object :: is_initialized ());
}

//	static
string GUI_Engine ::
	get_type_name ()
{
	return "GUI engine";
}

void GUI_Engine ::
	set_scene_manager (Ogre :: SceneManager & new_scene_manager)
{
	renderer -> setTargetSceneManager (& new_scene_manager);
}

void GUI_Engine ::
	set_mouse_position (pair <float, float> new_position)
{
	system -> injectMousePosition (new_position . first, new_position . second);
}

void GUI_Engine ::
	left_mouse_button_click ()
{
	system -> injectMouseButtonDown (CEGUI :: LeftButton);
	system -> injectMouseButtonUp (CEGUI :: LeftButton);
}

bool GUI_Engine ::
	render ()
	const
{
	assert (is_initialized ());

	system -> renderGUI ();

	return true;
}

void GUI_Engine ::
	show (string message)
{
	root_window -> getChild ("text") -> setText (message);
	trace () << "new message: '" << message << "'" << endl;
}

bool GUI_Engine ::
	handle_button (const CEGUI :: EventArgs & e)
{
	assert (is_initialized ());

	CEGUI :: WindowEventArgs * event = (CEGUI :: WindowEventArgs *)(& e);
	if (event == NULL)
	{
		show ("Unknown event type...");
	}
	else
	{
		show ("The '" + string (event -> window -> getText () . c_str ()) + "' button was clicked.");
	}

	return true;
}
