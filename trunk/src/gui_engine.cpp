#include "gui_engine.hpp"

using namespace std;
using namespace CEGUI;

extern string path;

GUI_Engine ::
	GUI_Engine (Ogre :: RenderWindow * window) :
	Engine ("Gui engine")
{
	assert (Object :: is_initialized ());
	assert (window != NULL);

	renderer = new OgreCEGUIRenderer (window/*, Ogre :: RENDER_QUEUE_OVERLAY,
         false, 3000, Ogre :: ST_EXTERIOR_REAL_FAR*/);

	system = new System (renderer, (utf8 *) (path + "/logs/cegui.txt") . c_str ());

	window_manager = & WindowManager :: getSingleton ();

	CEGUI :: Logger :: getSingleton () . setLoggingLevel (CEGUI :: Informative);

	/*CEGUI::SchemeManager::getSingleton().loadScheme((utf8 *)"TaharezLook.scheme");

	mGuiSystem->setDefaultMouseCursor((utf8 *)"TaharezLook", (utf8 *)"MouseArrow");

	mGuiSystem->setDefaultFont((utf8 *)"Tahoma-12");*/

	root_window = system -> setGUISheet
	(
		window_manager -> createWindow ("DefaultGUISheet", "Root")
	);

	Window * test_window = window_manager -> createWindow
									("TaharezLook/FrameWindow", "testWindow");
	test_window -> setPosition (Point (0.25f, 0.25f));
	test_window -> setSize (Size (0.5f, 0.5f));
	test_window -> setText ("Hello World!");

	root_window -> addChildWindow (test_window);
	
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
	return Engine :: is_initialized ();
}

void GUI_Engine ::
	render ()
	const
{
//	!!!	system -> renderGUI ();
}
