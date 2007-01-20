#include "gui_engine.hpp"

using namespace std;
using namespace tsl;

GUI_Engine ::
	GUI_Engine
	(
		Ogre :: RenderWindow & window,
		string log_file_name,
		GUI_Listener & new_gui_listener
	) :
	Object ("Gui engine"),
	Singleton <GUI_Engine> ("Gui engine"),
	State_Machine <GUI_Engine> (* this),
	gui_listener (new_gui_listener)
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
	get_class_name ()
{
	return "GUI_Engine";
}

void GUI_Engine ::
	set_scene_manager (Ogre :: SceneManager & new_scene_manager)
{
	assert (is_initialized ());

	renderer -> setTargetSceneManager (& new_scene_manager);
}

void GUI_Engine ::
	set_mouse_position (pair <float, float> new_position)
{
	assert (is_initialized ());

	system -> injectMousePosition (new_position . first, new_position . second);
}

void GUI_Engine ::
	left_mouse_button_click ()
{
	assert (is_initialized ());

	system -> injectMouseButtonDown (CEGUI :: LeftButton);
	system -> injectMouseButtonUp (CEGUI :: LeftButton);
}

bool GUI_Engine ::
	render ()
{
	assert (is_initialized ());

	get_active_state () . run ();
	system -> renderGUI ();

	return true;
}

GUI & GUI_Engine ::
	create_gui (string configuration_file)
{
	assert (is_initialized ());

	GUI & result =
		* (
			new GUI
			(
				* this,
				* window_manager -> loadWindowLayout (configuration_file),
				gui_listener
			)
		);

	//	If this is the first gui, it's automatically used as active state.
	add (result . to_type <State <GUI_Engine> > ());
	if (get_active_state () == result)
	{
		system -> setGUISheet (& result . root_window);
	}

	return result;
}

void GUI_Engine ::
	activate (GUI & gui)
{
	assert (is_initialized ());
	assert (gui . is_initialized ());
	assert (contains (gui, false));

	if (get_active_state () != gui)
	{
		change_active_state (gui);
		system -> setGUISheet (& gui . root_window);
	}
}

string GUI_Engine ::
	run ()
{
	assert (is_initialized ());

	return nothing;
}
