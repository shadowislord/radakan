#include "gui_engine.hpp"

using namespace std;
using namespace sl;

template <> GUI_Engine * Ogre :: Singleton <GUI_Engine> :: ms_Singleton = NULL;

GUI_Engine ::
	GUI_Engine (Ogre :: RenderWindow * window, string log_file_name) :
	Object ("Gui engine")
{
	assert (Object :: is_initialized ());
	assert (window != NULL);

	renderer = new CEGUI :: OgreCEGUIRenderer (window);

	system = new CEGUI :: System (renderer, CEGUI :: String (log_file_name) . data ());

	window_manager = & CEGUI :: WindowManager :: getSingleton ();

	CEGUI :: Logger :: getSingleton () . setLoggingLevel (CEGUI :: Informative);

	CEGUI :: SchemeManager :: getSingleton () . loadScheme ("TaharezLookSkin.scheme");
	system -> setDefaultMouseCursor ("TaharezLook", "MouseArrow");

	system -> setDefaultFont ("BlueHighway-12");

	root_window = window_manager -> createWindow ("DefaultWindow", "root");
	system -> setGUISheet (root_window);

	CEGUI :: Window * test_window = window_manager -> createWindow
												("TaharezLook/Button", "test");
	root_window -> addChildWindow (test_window);
	test_window -> setPosition (CEGUI :: Point (0.7f, 0.0f));
	test_window -> setSize (CEGUI :: Size (0.3f, 0.1f));
	test_window -> setText ("Hello World!");

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

bool GUI_Engine ::
	render ()
	const
{
	assert (is_initialized ());
	assert (system != NULL);
	system -> renderGUI ();

	return true;
}
