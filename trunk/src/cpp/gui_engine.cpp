#include "gui_engine.hpp"
#include "input_engine.hpp"
#include <CEGUIWindowManager.h>
#include <CEGUISchemeManager.h>

using namespace std;
using namespace TSL;
using namespace TSL :: Engines;

//	static
const string GUI_Engine ::
	get_class_name ()
{
	return "GUI_Engine";
}

GUI_Engine ::
	GUI_Engine
	(
		Ogre :: RenderWindow & window,
		Ogre :: SceneManager & scene_manager,
		string log_file_name
	) :
	Object ("gui engine")
{
	Engines :: Log :: trace (me, GUI_Engine :: get_class_name (), "", "~window~", "~scene_manager~", log_file_name);

	Engines :: Log :: trace (me, GUI_Engine :: get_class_name (), "", "~window~", "~scene_manager~", log_file_name, "A");
	renderer = new CEGUI :: OgreCEGUIRenderer (& window, Ogre :: RENDER_QUEUE_OVERLAY, false, 0, & scene_manager);

	Engines :: Log :: trace (me, GUI_Engine :: get_class_name (), "", "~window~", "~scene_manager~", log_file_name, "B");
	system = new CEGUI :: System (renderer, NULL, NULL, NULL, "", log_file_name);

	Engines :: Log :: trace (me, GUI_Engine :: get_class_name (), "", "~window~", "~scene_manager~", log_file_name, "C");
	CEGUI :: SchemeManager :: getSingleton () . loadScheme ("TaharezLookSkin.scheme");
	system -> setDefaultMouseCursor ("TaharezLook", "MouseArrow");

	Engines :: Log :: trace (me, GUI_Engine :: get_class_name (), "", "~window~", "~scene_manager~", log_file_name, "D");
	system -> setDefaultFont ("BlueHighway-12");

	Engines :: Log :: trace (me, GUI_Engine :: get_class_name (), "", "~window~", "~scene_manager~", log_file_name, "E");

	assert (is_initialized ());
}

GUI_Engine ::
	~GUI_Engine ()
{
	Engines :: Log :: trace (me, GUI_Engine :: get_class_name (), "~");
	assert (is_initialized ());

	forget_dependencies ();
}

//	virtual
bool GUI_Engine ::
	is_initialized ()
	const
{
	assert (Singleton <GUI_Engine> :: is_initialized ());
	assert (State_Machines :: Data_State_Machine <GUI> :: is_initialized ());

	return true;
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

void GUI_Engine ::
	render ()
	const
{
	assert (is_initialized ());

	system -> renderGUI ();
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
				* CEGUI :: WindowManager :: getSingleton () . loadWindowLayout (configuration_file)
			)
		);

	result . register_observer (Input_Engine :: get ());

	//	If this is the first gui, it's automatically used as active state.
	bool check = add (result);
	assert (check);

	return result;
}

void GUI_Engine ::
	set_active_state (GUI & gui)
{
	assert (is_initialized ());
	assert (gui . is_initialized ());

	State_Machines :: State_Machine <GUI> :: set_active_state (gui);

	system -> setGUISheet (& gui . root_window);
}
