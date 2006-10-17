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

	CEGUI :: SchemeManager :: getSingleton () . loadScheme ("schemes/TaharezLook.scheme");

	system -> setDefaultMouseCursor ("TaharezLook", "MouseArrow");

//	system -> setDefaultFont ("Tahoma-12");

	root_window = system -> setGUISheet
	(
		window_manager -> createWindow ("DefaultWindow", "Sheet")
	);

	Window * test_window = window_manager -> createWindow
									("TaharezLook/Button");
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
	debug () << "a" << endl;
	bool result = Engine :: is_initialized ();
	debug () << "b" << endl;
	return result;
}

void GUI_Engine ::
	render ()
	const
{
	debug () << "c" << endl;

	assert (is_initialized ());
	assert (system != NULL);
	system -> renderGUI ();
}
