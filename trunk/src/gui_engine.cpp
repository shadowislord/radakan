#include "gui_engine.hpp"

using namespace std;
using namespace CEGUI;

extern string path;

GUI_Engine ::
	GUI_Engine (Ogre :: RenderWindow * window, Ogre :: SceneManager * scene_manager) :
	Object ("Gui engine")
{
	assert (Object :: is_initialized ());
	assert (window != NULL);

	debug () << "GUI A" << endl;

	renderer = new OgreCEGUIRenderer (window, Ogre :: RENDER_QUEUE_OVERLAY,
													false, 3000, scene_manager);
	//	This only works for me in debug mode. --Tinus

	debug () << "GUI B" << endl;
	system = new System (renderer, String (path + "/logs/cegui.txt") . data ());

	debug () << "GUI C" << endl;
	window_manager = & WindowManager :: getSingleton ();

	debug () << "GUI D" << endl;
	CEGUI :: Logger :: getSingleton () . setLoggingLevel (CEGUI :: Informative);

	debug () << "GUI E" << endl;
	CEGUI :: SchemeManager :: getSingleton () . loadScheme ("TaharezLookSkin.scheme");
	system -> setDefaultMouseCursor ("TaharezLook", "MouseArrow");

	debug () << "GUI F" << endl;
	system -> setDefaultFont ("BlueHighway-12");

	debug () << "GUI G" << endl;
	root_window = window_manager -> createWindow ("DefaultWindow", "root");
	system -> setGUISheet (root_window);

	debug () << "GUI H" << endl;
	Window * test_window = window_manager -> createWindow
												("TaharezLook/Button", "test");
	debug () << "GUI I" << endl;
	root_window -> addChildWindow (test_window);
	debug () << "GUI J" << endl;
	test_window -> setPosition (Point (0.25f, 0.25f));
	test_window -> setSize (Size (0.5f, 0.5f));
	test_window -> setText ("Hello World!");

	debug () << "GUI K" << endl;
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
	return Object :: is_initialized ();
}

void GUI_Engine ::
	render ()
	const
{
	assert (is_initialized ());
	assert (system != NULL);
//	system -> renderGUI ();
}
