#include <CEGUIImagesetManager.h>
#include <CEGUISchemeManager.h>
#include <CEGUISystem.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <OgreCEGUIRenderer.h>

#include "engines/gui_engine.hpp"
#include "engines/input_engine.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"
#include "play_state_gui.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;

//	static
string GUI_Engine ::
	get_class_name ()
{
	return "GUI_Engine";
}

GUI_Engine ::
	GUI_Engine
	(
		boost :: shared_ptr <Ogre :: RenderWindow> window,
		boost :: shared_ptr <Ogre :: SceneManager> scene_manager
	) :
	Object ("gui engine")
{
	Engines :: Log :: trace (me, GUI_Engine :: get_class_name (), "", "~window~", "~scene_manager~");

	try
	{
		//	The following line results in an error (at line 73 of CEGUIString.cpp) for me. --Tinus
		renderer . reset (new CEGUI :: OgreCEGUIRenderer (window . get ()));
	}
	catch (exception & e)
	{
		Log :: error (me) << "OgreCEGUIRenderer exception: " << e . what () << endl;
		abort ();
	}

	Engines :: Log :: trace
		(me, GUI_Engine :: get_class_name (), "", "~window~", "~scene_manager~", "AA");
	
	renderer -> setTargetSceneManager (scene_manager . get ());

	Engines :: Log :: trace
		(me, GUI_Engine :: get_class_name (), "", "~window~", "~scene_manager~", "A");
	system . reset (new CEGUI :: System
		(renderer . get (), NULL, NULL, NULL, "", Settings :: get () -> radakan_path + "/log/cegui.txt"));

	CEGUI :: ImagesetManager :: getSingletonPtr () -> createImageset ("logo.imageset");

	Engines :: Log :: trace
		(me, GUI_Engine :: get_class_name (), "", "~window~", "~scene_manager~", "B");
	CEGUI :: SchemeManager :: getSingleton () . loadScheme ("TaharezLookSkin.scheme");
	system -> setDefaultMouseCursor ("TaharezLook", "MouseArrow");

	Engines :: Log :: trace
		(me, GUI_Engine :: get_class_name (), "", "~window~", "~scene_manager~", "C");
	system -> setDefaultFont ("BlueHighway-12");

	Engines :: Log :: trace
		(me, GUI_Engine :: get_class_name (), "", "~window~", "~scene_manager~", "D");

	assert (is_initialized ());
}

GUI_Engine ::
	~GUI_Engine ()
{
	Engines :: Log :: trace (me, GUI_Engine :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool GUI_Engine ::
	is_initialized ()
	const
{
	assert (Singleton <GUI_Engine> :: is_initialized ());
	assert (State_Machine <GUI> :: is_initialized ());
	assert (renderer);
	assert (system);

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

template <class T> Reference <T> GUI_Engine ::
	create_gui (string configuration_file)
{
	assert (is_initialized ());

	boost :: shared_ptr <CEGUI :: Window> window
	(
		CEGUI :: WindowManager :: getSingleton () . loadWindowLayout (configuration_file)
	);

	Reference <T> result (new T (configuration_file, window));

	result -> Observable <Object> :: register_observer (Input_Engine :: get ());

	if (! has_active_state ())
	{
		set_active_state (result);
	}

	return result;
}

void GUI_Engine ::
	set_active_gui (Reference <GUI> gui)
{
	assert (is_initialized ());
	assert (gui -> is_initialized ());

	State_Machine <GUI> :: set_active_state (gui);

	system -> setGUISheet (gui -> root_window . get ());
}

template Reference <GUI> GUI_Engine :: create_gui (string configuration_file);
template Reference <Play_State_GUI> GUI_Engine :: create_gui (string configuration_file);
