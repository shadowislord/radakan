#include "engines/gui_engine.hpp"
#include "engines/input_engine.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"
#include "play_state_gui.hpp"

#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	#include <CEGUIImagesetManager.h>
	#include <CEGUISchemeManager.h>
	#include <CEGUISystem.h>
	#include <CEGUIWindow.h>
	#include <CEGUIWindowManager.h>
	#include <OgreCEGUIRenderer.h>
#endif

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
	Object ("gui engine", true)	//	Here 'true' means 'prevent automatic destruction'.
{
	Engines :: Log :: trace (me, GUI_Engine :: get_class_name (), "", "~window~", "~scene_manager~");

	#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
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
	#endif

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
	#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
		assert (renderer);
		assert (system);
	#endif
	
	return true;
}

void GUI_Engine ::
	set_mouse_position (const Ogre :: Vector3 & new_position)
{
	assert (is_initialized ());

	#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
		system -> injectMousePosition (new_position . x, new_position . y);
	#endif
}

void GUI_Engine ::
	left_mouse_button_click ()
{
	assert (is_initialized ());

	#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
		system -> injectMouseButtonDown (CEGUI :: LeftButton);
		system -> injectMouseButtonUp (CEGUI :: LeftButton);
	#endif
}

void GUI_Engine ::
	render ()
	const
{
	assert (is_initialized ());

	#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
		system -> renderGUI ();
	#endif
}

template <class T> Reference <T> GUI_Engine ::
	create_gui (string configuration_file)
{
	assert (is_initialized ());

	#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
		boost :: shared_ptr <CEGUI :: Window> window
		(
			CEGUI :: WindowManager :: getSingleton () . loadWindowLayout (configuration_file)
		);
	#endif

	Reference <T> result (new T (configuration_file
	#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
		, window
	#endif
	));

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

	#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
		system -> setGUISheet (gui -> root_window . get ());
	#endif
}

template Reference <GUI> GUI_Engine :: create_gui (string configuration_file);
template Reference <Play_State_GUI> GUI_Engine :: create_gui (string configuration_file);
