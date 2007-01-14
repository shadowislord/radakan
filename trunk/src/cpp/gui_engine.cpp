#include "gui_engine.hpp"

using namespace std;
using namespace tsl;

template <> GUI_Engine * Ogre :: Singleton <GUI_Engine> :: ms_Singleton = NULL;

GUI_Engine ::
	GUI_Engine (Ogre :: RenderWindow & window, string log_file_name) :
	Object ("Gui engine")
{
	assert (Object :: is_initialized ());

	renderer = new CEGUI :: OgreCEGUIRenderer (& window);

//	CEGUI :: System :: setDefaultXMLParserName ("XercesParser");
	system = new CEGUI :: System (renderer, CEGUI :: String (log_file_name) . data ());

	window_manager = & CEGUI :: WindowManager :: getSingleton ();

	CEGUI :: Logger :: getSingleton () . setLoggingLevel (CEGUI :: Informative);

	CEGUI :: SchemeManager :: getSingleton () . loadScheme ("TaharezLookSkin.scheme");
	system -> setDefaultMouseCursor ("TaharezLook", "MouseArrow");

	system -> setDefaultFont ("BlueHighway-12");

	root_window = window_manager -> loadWindowLayout ("tsl.config");
	system -> setGUISheet (root_window);

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
	debug () << "new mouse position: " << new_position . first << " " << new_position . second << endl;
	system -> injectMousePosition (new_position . first, new_position . second);
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
