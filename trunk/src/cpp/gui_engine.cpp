#include "gui_engine.hpp"
#include <CEGUIWindowManager.h>
#include <CEGUISchemeManager.h>

using namespace std;
using namespace tsl;

GUI_Engine ::
	GUI_Engine
	(
		Ogre :: RenderWindow & window,
		Ogre :: SceneManager & scene_manager,
		string log_file_name
	) :
	Object ("GUI engine")
{
	log (debugging) << get_class_name () << " (~window~, ~scene_manager~, " << log_file_name << ")" << endl;
	assert (GUI_Listener :: is_instantiated ());
	assert (Singleton <GUI_Engine> :: is_initialized ());
	assert (Data_State_Machine <GUI> :: is_initialized ());

	log (debugging) << get_class_name () << " (~window~, ~scene_manager~, " << log_file_name << ") A" << endl;
	renderer = new CEGUI :: OgreCEGUIRenderer (& window, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, & scene_manager);

	log (debugging) << get_class_name () << " (~window~, ~scene_manager~, " << log_file_name << ") BC" << endl;
	system = new CEGUI :: System (renderer, NULL, NULL, NULL, "", log_file_name);

	log (debugging) << get_class_name () << " (~window~, ~scene_manager~, " << log_file_name << ") D" << endl;
	CEGUI :: SchemeManager :: getSingleton () . loadScheme ("TaharezLookSkin.scheme");
	system -> setDefaultMouseCursor ("TaharezLook", "MouseArrow");

	log (debugging) << get_class_name () << " (~window~, ~scene_manager~, " << log_file_name << ") F" << endl;
	system -> setDefaultFont ("BlueHighway-12");

	log (debugging) << get_class_name () << " (~window~, ~scene_manager~, " << log_file_name << ") G" << endl;
	assert (is_initialized ());
}

GUI_Engine ::
	~GUI_Engine ()
{
	log (debugging) << "~" << get_class_name () << " ()" << endl;
	assert (is_initialized ());
}

//	virtual
bool GUI_Engine ::
	is_initialized ()
	const
{
	assert (GUI_Listener :: is_instantiated ());
	assert (Singleton <GUI_Engine> :: is_initialized ());
	assert (Data_State_Machine <GUI> :: is_initialized ());

	return true;
}

//	static
string GUI_Engine ::
	get_class_name ()
{
	return "GUI_Engine";
}

void GUI_Engine ::
	set_mouse_position (const Ogre :: Vector3 & new_position)
{
	assert (is_initialized ());

	system -> injectMousePosition (new_position . x, new_position . y);
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

	get_active_state () . update_message ();
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
				configuration_file,
				* CEGUI :: WindowManager :: getSingleton () . loadWindowLayout (configuration_file),
				GUI_Listener :: get ()
			)
		);

	//	If this is the first gui, it's automatically used as active state.
	bool check = add (result);
	assert (check);
	
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
	assert (contains (gui));

	if (get_active_state () != gui)
	{
		log (debugging) << "Changing to GUI: " << gui << endl;
		set_active_state (gui);
		system -> setGUISheet (& gui . root_window);
	}
}
