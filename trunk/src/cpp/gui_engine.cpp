#include "gui_engine.hpp"

using namespace std;
using namespace sl;

template <> GUI_Engine * Ogre :: Singleton <GUI_Engine> :: ms_Singleton = NULL;

GUI_Engine ::
	GUI_Engine (Ogre :: RenderWindow & window, string log_file_name) :
	Object ("Gui engine")
{
	assert (Object :: is_initialized ());

	renderer = new CEGUI :: OgreCEGUIRenderer (& window);

//	CEGUI :: System :: setDefaultXMLParserName ("XercesParser");
	system = new CEGUI :: System (renderer, NULL, NULL, NULL, CEGUI :: String (), CEGUI :: String (log_file_name));

	window_manager = & CEGUI :: WindowManager :: getSingleton ();

	CEGUI :: Logger :: getSingleton () . setLoggingLevel (CEGUI :: Informative);

	CEGUI :: SchemeManager :: getSingleton () . loadScheme ("TaharezLookSkin.scheme");
	system -> setDefaultMouseCursor ("TaharezLook", "MouseArrow");

	system -> setDefaultFont ("BlueHighway-12");

	root_window = window_manager -> loadWindowLayout("data/gui/config/tslrpg.conf");
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

bool GUI_Engine ::
	render ()
	const
{
	assert (is_initialized ());

	system -> renderGUI ();

	return true;
}
